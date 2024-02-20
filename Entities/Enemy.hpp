#ifndef MORSKI_SMETAR_ENEMY_HPP
#define MORSKI_SMETAR_ENEMY_HPP


#include <vector>
#include "GameObject.hpp"
#include "Player.hpp"

class Enemy : public GameObject {
public:
	enum Axis {
		X,
		Y
	};
	enum Direction {
		POSITIVE,
		NEGATIVE
	};
	enum State {
		ATTACK,
		RETREAT,
		IDLE
	};
private:
	Axis axis;
	Direction direction;
	State state;
	
	void Idle();
	void Attack();
	void Retreat();
	
	int attack_cd;
	
	static Player * player;
	static std::vector<Enemy> * enemies;
public:
	bool operator!=(const Enemy & other) const;
	Enemy();
	void Update() override;
	
	static void setEnemies(std::vector<Enemy> * enemiesptr);
	static void setPlayer(Player * playerptr);
	
	Axis getAxis();
	
	void setAxis(Enemy::Axis new_axis);
};


#endif //MORSKI_SMETAR_ENEMY_HPP
