#ifndef SDL_GAME_PLAYER_HPP
#define SDL_GAME_PLAYER_HPP

#include "GameObject.hpp"

#include <thread>

#include <iostream>

class Player : public GameObject {
	int speed = 10;
	bool facing_right = true;
public:
	Player();
	Player(const SDL_Rect& rect, const Design& design);
	
	[[nodiscard]] bool isFacingRight() const;
	[[nodiscard]] bool isFacingLeft() const;
	
	[[maybe_unused]] Player &operator=(const Player &other);
	[[maybe_unused]] Player(const Player &other);
	~Player() override;
	
	void Update() override;
};


#endif //SDL_GAME_PLAYER_HPP
