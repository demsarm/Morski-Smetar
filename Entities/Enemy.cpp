//
// Created by martind on 19.2.2024.
//

#include "Enemy.hpp"

#include "../Tools/Random.hpp"
#include "../Config/Config.hpp"
#include "../Tools/Overloads.hpp"

std::vector<Enemy> * Enemy::enemies = nullptr;
std::vector<Trash> * Enemy::trashes = nullptr;
Player * Enemy::player = nullptr;


bool Enemy::operator!=(const Enemy &other) const {
	return rect != other.rect;
}

/**
 * @brief Default constructor for the Enemy class
 */
Enemy::Enemy() : GameObject() {
	attack_cd = 0;
	state = IDLE;
	axis = (Axis) Random::randint(0, 2);
	direction = (Direction) Random::randint(0, 2);
}

/**
 * @brief Constructor for the Enemy class
 * @param enemiesptr pointer to the vector of enemies
 */
void Enemy::setEnemies(std::vector<Enemy> * enemiesptr) {
	Enemy::enemies = enemiesptr;
}
/**
 * @brief Constructor for the Enemy class
 * @param playerptr pointer to the player
 */
void Enemy::setPlayer(Player * playerptr) {
	Enemy::player = playerptr;
}

/**
 * @brief Updates the state of the Enemy object.
 *
 * This function checks the current state of the Enemy object and calls the corresponding function to handle that state.
 * The states can be IDLE, ATTACK, RETREAT, or POLLUTING.
 *
 * If the state is IDLE, the Idle() function is called.
 * If the state is ATTACK, the Attack() function is called.
 * If the state is RETREAT, the Retreat() function is called.
 * If the state is POLLUTING, the Pollute() function is called.
 */
void Enemy::Update() {
	if (state == IDLE) {
		Idle();
	} else if (state == ATTACK) { // state == ATTACK
		Attack();
	} else if (state == RETREAT) { // state == RETREAT
		Retreat();
	} else if (state == POLLUTING) { // state == POLLUTING
		Pollute();
	}
}

/**
 * @brief The Idle state of the Enemy object.
 *
 * This function is called when the state of the Enemy object is IDLE.
 * It checks if the player is within the sight range of the enemy and if the player is disembarked.
 * If the player is within the sight range and disembarked, the state is changed to ATTACK and the Attack() function is called.
 * If the player is not within the sight range, the enemy moves in a random direction.
 * If the enemy reaches the edge of the screen, it changes direction.
 */
void Enemy::Idle() {
	if (Random::randint(2000) <= 1){
		state = POLLUTING;
		Pollute();
	} else {
		if (axis == X) {
			rect.x += direction == POSITIVE ? 1 : -1;
		} else { // axis == Y
			rect.y += direction == POSITIVE ? 1 : -1;
		}
		if (rect.x < 0 || rect.x > WindowData::SCREEN_WIDTH / 3 - rect.w) {
			direction = direction == POSITIVE ? NEGATIVE : POSITIVE;
		}
		if (rect.y < 0 || rect.y > WindowData::SCREEN_HEIGHT - rect.h) {
			direction = direction == POSITIVE ? NEGATIVE : POSITIVE;
		}
		
		if (sqrt(pow(player->getRect().x - rect.x, 2) + pow(player->getRect().y - rect.y, 2)) <
		    (double) WindowData::SCREEN_WIDTH / 6) {
			if (player->getState() == Player::DISEMBARKED) {
				state = ATTACK;
				attack_cd = Config::ENEMY_ATTACK_CD;
				Attack();
			} else {
				state = RETREAT;
				Retreat();
			}
		}
	}
}

/**
 * @brief The Attack state of the Enemy object.
 *
 * This function is called when the state of the Enemy object is ATTACK.
 * It checks if the player is within the sight range of the enemy and if the player is disembarked.
 * If the player is within the sight range and disembarked, the enemy moves towards the player.
 * If the player is not within the sight range, the state is changed to RETREAT and the Retreat() function is called.
 * If the player is within the sight range but the attack cooldown is not 0, the enemy moves towards the player.
 * If the enemy reaches the edge of the screen, it changes direction.
 */
void Enemy::Attack() {
	if (player == nullptr) {
		return;
	} // just for safety (in case player is not set yet)
	
	if (sqrt(pow(player->getRect().x - rect.x, 2) + pow(player->getRect().y - rect.y, 2)) < (double)Config::ENEMY_SIGHT_RANGE && player->getState() == Player::DISEMBARKED) {
		bool att = false;
		for (auto &other : *enemies) {
			if (other != *this && sqrt(pow(other.getRect().x - rect.x, 2) + pow(other.getRect().y - rect.y, 2)) < (double)Config::ENEMY_LINK_RANGE) {
				att = true;
				break;
			}
		}
		if (!att) {
			state = RETREAT;
			Retreat();
		} else if (attack_cd > 0) {
			rect.x += player->getRect().x > rect.x ? 1 : -1;
			rect.y += player->getRect().y > rect.y ? 1 : -1;
			if (rect.x < 0) {
				rect.x = 0;
			} else if (rect.x > WindowData::SCREEN_WIDTH / 3 - rect.w) {
				rect.x = WindowData::SCREEN_WIDTH / 3 - rect.w;
			}
			if (rect.y < 0) {
				rect.y = 0;
			} else if (rect.y > WindowData::SCREEN_HEIGHT - rect.h) {
				rect.y = WindowData::SCREEN_HEIGHT - rect.h;
			}
		} else {
			--attack_cd;
		}
	} else {
		state = IDLE;
	}
}

/**
 * @brief The Retreat state of the Enemy object.
 *
 * This function is called when the state of the Enemy object is RETREAT.
 * It checks if the player is within the sight range of the enemy and if the player is disembarked.
 * If the player is within the sight range and disembarked, the state is changed to ATTACK and the Attack() function is called.
 * If the player is not within the sight range, the enemy moves in a random direction.
 * If the enemy reaches the edge of the screen, it changes direction.
 * If the enemy is not within the sight range of the player, the state is changed to IDLE and the Idle() function is called.
 */
void Enemy::Retreat() {
	if (player == nullptr) {
		return;
	} // just for safety (in case player is not set yet)
	
	if (sqrt(pow(player->getRect().x - rect.x, 2) + pow(player->getRect().y - rect.y, 2)) > (double)Config::ENEMY_SIGHT_RANGE) {
		state = IDLE;
		axis = (Axis) Random::randint(0, 2);
		direction = (Direction) Random::randint(0, 2);
	} else {
		bool run = true;
		for (auto &other : *enemies) {
			if (other != *this && sqrt(pow(other.getRect().x - rect.x, 2) + pow(other.getRect().y - rect.y, 2)) < (double)Config::ENEMY_LINK_RANGE) {
				run = false;
				break;
			}
		}
		if (!run) {
			state = ATTACK;
			attack_cd = Config::ENEMY_ATTACK_CD;
//			Attack(); // this is not necessary because the next time the Update() is called, it will call Attack() - also it would be a recursive call, so this is kind of safer
		} else {
			rect.x += player->getRect().x > rect.x ? -1 : 1;
			rect.y += player->getRect().y > rect.y ? -1 : 1;
			if (rect.x < 0) {
				rect.x = 0;
			} else	if (rect.x > WindowData::SCREEN_WIDTH / 3 - rect.w) {
				rect.x = WindowData::SCREEN_WIDTH / 3 - rect.w;
			}
			if (rect.y < 0) {
				rect.y = 0;
			} else if (rect.y > WindowData::SCREEN_HEIGHT - rect.h) {
				rect.y = WindowData::SCREEN_HEIGHT - rect.h;
			}
		}
	}
}

/**
 * @brief Returns the axis of the Enemy object.
 * @return the axis of the Enemy object
 */
Enemy::Axis Enemy::getAxis() {
	return axis;
}

/**
 * @brief Sets the axis of the Enemy object.
 * @param new_axis the new axis of the Enemy object
 */
void Enemy::setAxis(Enemy::Axis new_axis) {
	axis = new_axis;
}

/**
 * @brief The Pollute state of the Enemy object.
 *
 * This function is called when the state of the Enemy object is POLLUTING.
 * It checks if the enemy is within a certain range of the screen width.
 * If the enemy is within the range and the trash vector is not null, a new trash object is created and added to the trash vector.
 * The new trash object is positioned relative to the enemy's position.
 * After creating the trash object, the state of the enemy is changed to IDLE.
 * If the enemy is not within the range, it moves towards the range.
 */
void Enemy::Pollute() {
	if (rect.x > WindowData::SCREEN_WIDTH / 4 - rect.w) {
		if (trashes == nullptr) {
			return;
		} else {
			Trash trash(WindowData::SCREEN_WIDTH/3 + 10, rect.y + 7);
			// enemy is 65 tall, trash is 50 tall
			// 65 / 2 - 50 /2 = 15/2 = ~7
			trashes->push_back(trash);
		}
		state = IDLE;
	}
	else {
		rect.x += 1;
	}
}

/**
 * @brief Sets the trashes vector to the given vector.
 * @param trashesptr the pointer to the vector of trashes
 */
void Enemy::setTrashes(std::vector<Trash> *trashesptr) {
	trashes = trashesptr;
}


