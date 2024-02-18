#include "Player.hpp"

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
	const Uint8 *state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_W]) {
		move(0, -speed);
	}
	if (state[SDL_SCANCODE_S]) {
		move(0, speed);
	}
	if (state[SDL_SCANCODE_A]) {
		facing_right = false;
		move(-speed, 0);
	}
	if (state[SDL_SCANCODE_D]) {
		facing_right = true;
		move(speed, 0);
	}
}