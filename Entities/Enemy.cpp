//
// Created by martind on 19.2.2024.
//

#include "Enemy.hpp"

#include "../Tools/Random.hpp"
#include "../Config/Config.hpp"
#include "../Tools/Overloads.hpp"

std::vector<Enemy> * Enemy::enemies = nullptr;
Player * Enemy::player = nullptr;

bool Enemy::operator!=(const Enemy &other) const {
	return rect != other.rect;
}

Enemy::Enemy() : GameObject() {
	attack_cd = 0;
	state = IDLE;
	axis = (Axis) Random::randint(0, 2);
	direction = (Direction) Random::randint(0, 2);
}

void Enemy::setEnemies(std::vector<Enemy> * enemiesptr) {
	Enemy::enemies = enemiesptr;
}
void Enemy::setPlayer(Player * playerptr) {
	Enemy::player = playerptr;
}

void Enemy::Update() {
	if (state == IDLE) {
		Idle();
	} else if (state == ATTACK) { // state == ATTACK
		Attack();
	} else { // state == RETREAT
		Retreat();
	}
}

void Enemy::Idle() {
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
	
	if (sqrt(pow(player->getRect().x - rect.x, 2) + pow(player->getRect().y - rect.y, 2)) < (double)WindowData::SCREEN_WIDTH / 6) {
		state = ATTACK;
		attack_cd = Config::ENEMY_ATTACK_CD;
		Attack();
	}
}

void Enemy::Attack() {
	if (player == nullptr) {
		return;
	} // just for safety (in case player is not set yet)
	
	if (sqrt(pow(player->getRect().x - rect.x, 2) + pow(player->getRect().y - rect.y, 2)) < (double)Config::ENEMY_SIGHT_RANGE) {
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


