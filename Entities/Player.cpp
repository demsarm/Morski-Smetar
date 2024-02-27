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
Player::Player() : facing_right(true), sonar_color(){}

/**
 * Constructor for the Player class.
 * @param rect The rectangle to initialize the player with
 * @param design The design to initialize the player with
 */
[[maybe_unused]] Player::Player(const SDL_Rect& rect, const Design& design) : facing_right(true) {
	this->rect = rect;
	this->design = design;
	sonar_color = {255, 255, 255, 255};
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

/**
 * @biref Tells you if the player is facing right
 */
[[maybe_unused]] bool Player::isFacingRight() const {
	return facing_right;
}

/**
 * @biref Tells you if the player is facing left
 */
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

/**
 * @brief Updates the state of the Player object based on keyboard inputs.
 *
 * This function checks the current state of the keyboard and updates the Player object accordingly.
 * The player can move up, down, left, or right based on the 'W', 'S', 'A', and 'D' keys respectively.
 * The speed of the player is determined by the 'speed' variable.
 * The player's state can be either EMBARKED or DISEMBARKED.
 * If the player is EMBARKED, they can move within the entire screen width.
 * If the player is DISEMBARKED, they can only move within a third of the screen width.
 * The player's state can be switched between EMBARKED and DISEMBARKED by pressing the 'E' key.
 * The switch can only occur if the player is colliding with a certain area of the screen and if the switch delay is 0.
 * After a switch, the switch delay is set to 300.
 */
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
	if (isColliding(getRect() + (SDL_Rect){getRect().w, 0, 0, 0},
					{boat_position.x - WindowData::SCREEN_WIDTH/8,
					    boat_position.y - 50,
						boat_position.w + WindowData::SCREEN_WIDTH/4,
						boat_position.h + 100})) {
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

/**
 * @brief Returns the state of the Player object.
 * @return The state of the Player object
 */
[[maybe_unused]] Player::PlayerState Player::getState() const {
	return state;
}

/**
 * @brief Returns the boat position of the Player object.
 * @return The boat position of the Player object
 */
[[maybe_unused]] SDL_Rect Player::getBoatPosition() const {
	return boat_position;
}

/**
 * @brief Sets the sonar color of the Player object.
 * @param color The color to set the sonar to
 */
[[maybe_unused]] void Player::setSonarColor(const SDL_Color &color) {
	sonar_color = color;
}

/**
 * @brief Returns the sonar color of the Player object.
 * @return The sonar color of the Player object
 */
[[maybe_unused]] SDL_Color Player::getSonarColor() const {
	return sonar_color;
}

/**
 * @brief Sets the boat position
 * @param rect The new boat position
 * @warning This may mess with the game's logic, so avoid this if you don't know what you are doing
 */
void Player::setBoatPosition(const SDL_Rect &rect) {
	boat_position = rect;
}

/**
 * @brief Sets the state of the Player object.
 * @param new_state The new state to set the Player object to
 */
void Player::setState(Player::PlayerState new_state) {
	state = new_state;
}
