#ifndef SDL_GAME_PLAYER_HPP
#define SDL_GAME_PLAYER_HPP

#include "GameObject.hpp"

#include <thread>

#include <iostream>

class Player : public GameObject {
public: // This is why C# handles public and private better
	enum PlayerState {
		EMBARKED,
		DISEMBARKED
	};
	
private:
	int speed = 10;
	bool facing_right = true;
	PlayerState state = EMBARKED;
	int switch_delay = 0;
	SDL_Rect boat_position = {0, 0, 0, 0};
public:
	[[nodiscard]] PlayerState getState() const;
	
	Player();
	Player(const SDL_Rect& rect, const Design& design);
	
	SDL_Rect getBoatPosition() const;
	
	[[nodiscard]] bool isFacingRight() const;
	[[nodiscard]] bool isFacingLeft() const;
	
	[[maybe_unused]] Player &operator=(const Player &other);
	[[maybe_unused]] Player(const Player &other);
	~Player() override;
	
	void Update() override;
};


#endif //SDL_GAME_PLAYER_HPP
