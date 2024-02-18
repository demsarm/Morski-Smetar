#include "Player.hpp"

#include "../Config/Config.hpp"
#include "../Tools/Collision.hpp"
#include "../Tools/Overloads.hpp"
/**
 * Default constructor for the Player class.
 * Initializes the input handler thread which continuously checks for keyboard events.
 * The input handler thread runs as long as input_handler_running is true.
 * The move speed of the player is determined by whether the left shift key is pressed.
 * If the left shift key is pressed, the move speed is 20, otherwise it is 10.
 * The player moves up when the 'W' key is pressed, down when the 'S' key is pressed,
 * left when the 'A' key is pressed, and right when the 'D' key is pressed.
 */
Player::Player() : facing_right(true) {}

[[maybe_unused]] Player::Player(const SDL_Rect& rect, const Design& design) : facing_right(true) {
	this->rect = rect;
	this->design = design;
}

/**
 * Copy constructor for the Player class.
 * @param other The Player object to copy
 */
[[maybe_unused]] Player::Player(const Player &other) : GameObject(other) {
	this->rect = other.rect;
	this->design = other.design;
}

/**
 * Destructor for the Player class.
 */
Player::~Player() = default;

[[maybe_unused]] bool Player::isFacingRight() const {
	return facing_right;
}

[[maybe_unused]] bool Player::isFacingLeft() const {
	return !facing_right;
}

Player &Player::operator=(const Player &other) {
	if (this != &other) {
		this->rect = other.rect;
		this->design = other.design;
	}
	return *this;
}

void Player::Update() {
	const Uint8 *kbd_state = SDL_GetKeyboardState(nullptr);
	if (kbd_state[SDL_SCANCODE_W]) {
		if (rect.y > 0) {
			move(0, -speed);
		}
	}
	if (kbd_state[SDL_SCANCODE_S]) {
		if (rect.y < WindowData::SCREEN_HEIGHT - rect.h) {
			move(0, speed);
		}
	}
	if (kbd_state[SDL_SCANCODE_A]) {
		facing_right = false;
		if (rect.x > WindowData::SCREEN_WIDTH / 3 + 10 && state == EMBARKED) { // The + 10 is there for some wiggle room
			move(-speed, 0);
		}
		if (rect.x > 0 && state == DISEMBARKED) {
			move(-speed, 0);
		}
		
	}
	if (kbd_state[SDL_SCANCODE_D]) {
		facing_right = true;
		if (rect.x < WindowData::SCREEN_WIDTH - rect.w && state == EMBARKED) {
			move(speed, 0);
		}
		if (rect.x < WindowData::SCREEN_WIDTH / 3 - rect.w - 10 && state == DISEMBARKED) {
			move(speed, 0);
		}
		
	}
	
	if (isColliding(getRect(), (SDL_Rect){WindowData::SCREEN_WIDTH / 3, 0, WindowData::SCREEN_WIDTH / 6, WindowData::SCREEN_HEIGHT})) {
		if (kbd_state[SDL_SCANCODE_E] && switch_delay <= 0) {
			state = DISEMBARKED;
			boat_position = getRect();
			setRect({WindowData::SCREEN_WIDTH/3 - (getRect().x - WindowData::SCREEN_WIDTH/3 + 80), getRect().y, 80, 115});
			setPath("Assets/Player.png");
			switch_delay = 300;
		}
	}
	if (isColliding(getRect() + (SDL_Rect){getRect().w, 0, 0, 0}, {boat_position.x - WindowData::SCREEN_WIDTH/12, boat_position.y - 50, boat_position.w + WindowData::SCREEN_WIDTH/6, boat_position.h + 100})) {
		if (kbd_state[SDL_SCANCODE_E] && switch_delay <= 0) {
			state = EMBARKED;
			setRect(boat_position);
			boat_position = {0, 0, 0, 0};
			setPath("Assets/Boat.png");
			switch_delay = 300;
		}
	
	}
	if (switch_delay > 0) {
		--switch_delay;
	}
	
}

Player::PlayerState Player::getState() const {
	return state;
}

SDL_Rect Player::getBoatPosition() const {
	return boat_position;
}
