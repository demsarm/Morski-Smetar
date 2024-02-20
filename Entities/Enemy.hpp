#ifndef MORSKI_SMETAR_ENEMY_HPP
#define MORSKI_SMETAR_ENEMY_HPP


#include <vector>
#include "GameObject.hpp"
#include "Player.hpp"
#include "Trash.hpp"

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
		IDLE,
		POLLUTING
	};
private:
	Axis axis;
	Direction direction;
	State state;
	
	void Idle();
	void Attack();
	void Retreat();
	void Pollute();
	
	int attack_cd;
	
	static Player * player;
	static std::vector<Enemy> * enemies;
	static std::vector<Trash> * trashes;
public:
	bool operator!=(const Enemy & other) const;
	Enemy();
	void Update() override;
	
	static void setEnemies(std::vector<Enemy> * enemiesptr);
	static void setPlayer(Player * playerptr);
	static void setTrashes(std::vector<Trash> * trashesptr);
	
	Axis getAxis();
	
	void setAxis(Enemy::Axis new_axis);
};


#endif //MORSKI_SMETAR_ENEMY_HPP
