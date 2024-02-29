#ifndef SDL_GAME_PLAYER_HPP
#define SDL_GAME_PLAYER_HPP

#include "GameObject.h"

#include <thread>

#include <iostream>

class Player : public GameObject {
public: // This is why C# handles public and private better
	enum PlayerState {
		EMBARKED,
		DISEMBARKED
	};
	
private:
	int speed = 5;
	bool facing_right = true;
	PlayerState state = EMBARKED;
	int switch_delay = 0;
	SDL_Rect boat_position = {0, 0, 0, 0};
	SDL_Color sonar_color;
public:
	[[nodiscard]] [[maybe_unused]] PlayerState getState() const;
	void setState(PlayerState state);
	
	Player();
	[[maybe_unused]] Player(const SDL_Rect& rect, const Design& design);
	
	[[nodiscard]] SDL_Rect getBoatPosition() const;
	[[nodiscard]] [[maybe_unused]] SDL_Color getSonarColor() const;
	
	[[maybe_unused]] void setSonarColor(const SDL_Color& color);
	[[maybe_unused]] void setBoatPosition(const SDL_Rect& rect);
	
	[[nodiscard]] bool isFacingRight() const;
	[[nodiscard]] bool isFacingLeft() const;
	
	[[maybe_unused]] Player &operator=(const Player &other);
	[[maybe_unused]] Player(const Player &other);
	~Player() override;
	
	void Update() override;
};


#endif //SDL_GAME_PLAYER_HPP
