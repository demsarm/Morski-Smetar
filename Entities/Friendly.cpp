//
// Created by martind on 20.2.2024.
//

#include "Friendly.hpp"
#include "../Config/Config.hpp"
#include "../Tools/Random.hpp"

SDL_Color Friendly::sonar_color = {255, 255, 255, 255};

/**
 * @brief Default constructor for the Friendly class
 */
Friendly::Friendly() : GameObject(){
	SDL_Rect tmp = {Random::value() < 0.3 ? Random::randint(WindowData::SCREEN_WIDTH / 12, WindowData::SCREEN_WIDTH / 4 - 70) :
					    Random::randint((int) (WindowData::SCREEN_WIDTH * 21 / 30), (int) (WindowData::SCREEN_WIDTH * 0.9) - 70),
	                 Random::randint(WindowData::SCREEN_HEIGHT / 10, WindowData::SCREEN_HEIGHT * 9 / 10 - 60)
					 , 70, 60};
	setRect(tmp);
	if (rect.x < WindowData::SCREEN_WIDTH / 3){
		type = Type::LAND;
	} else {
		type = Type::WATER;
	}
	axis = (Axis)(Random::randint(0, 2));
	direction = (Direction)(Random::randint(0, 2));
	if (type == WATER){
		setPath("Assets/Fishing-Boat.png");
	} else {
		setPath("Assets/Tractor.png");
	}
}

/**
 * @brief Constructor for the Friendly class
 * @param x x coordinate of the Friendly object
 * @param y y coordinate of the Friendly object
 */
[[maybe_unused]] Friendly::Friendly(int x, int y) {
	rect = {x, y, 70, 60};
	if (rect.x < WindowData::SCREEN_WIDTH / 3){
		type = Type::LAND;
	} else {
		type = Type::WATER;
	}
	axis = (Axis)(Random::randint(0, 2));
	direction = (Direction)(Random::randint(0, 2));
}
/**
 * @brief Updates the state of the Friendly object.
 *
 * This function updates the position of the Friendly object.
 */
void Friendly::Update() {
	if (axis == X){
		rect.x += direction == Direction::POSITIVE ? 1 : -1;
	} else {
		rect.y += direction == Direction::POSITIVE ? 1 : -1;
	}
	if ((type == LAND && rect.x <= 0) || (type == WATER && rect.x <= WindowData::SCREEN_WIDTH / 3)){
		direction = Direction::POSITIVE;
	} else if ((type == LAND && rect.x >= WindowData::SCREEN_WIDTH / 3 - 70) || (type == WATER && rect.x >= WindowData::SCREEN_WIDTH - 70)){
		direction = Direction::NEGATIVE;
	}
	if (rect.y <= 0){
		direction = Direction::POSITIVE;
	} else if (rect.y >= WindowData::SCREEN_HEIGHT - 60){
		direction = Direction::NEGATIVE;
	}
}

/**
 * @brief Returns the axis of the Friendly object
 * @return the axis of the Friendly object
 */
[[nodiscard]] Friendly::Axis Friendly::getAxis() const {
	return axis;
}

/**
 * @brief Sets the axis of the Friendly object
 * @param axis the new axis of the Friendly object
 */
[[maybe_unused]] void Friendly::setAxis(Friendly::Axis newaxis) {
	axis = newaxis;
}

/**
 * @brief Returns the direction of the Friendly object
 * @return the direction of the Friendly object
 */
[[nodiscard]] Friendly::Direction Friendly::getDirection() const {
	return direction;
}

/**
 * @brief Sets the direction of the Friendly object
 * @param newdirection the new direction of the Friendly object
 */
[[maybe_unused]] void Friendly::setDirection(Friendly::Direction newdirection) {
	direction = newdirection;
}
/**
 * @brief Returns the type of the Friendly object
 * @return the type of the Friendly object
 */
[[maybe_unused]] Friendly::Type Friendly::getType() const {
	return type;
}

/**
 * @brief Sets the sonar color of the Friendly object
 * @param newcolor the new sonar color of the Friendly object
 */
void Friendly::setSonarColor(SDL_Color newcolor) {
	Friendly::sonar_color = newcolor;
}

/**
 * @brief Returns the sonar color of the Friendly object
 * @return the sonar color of the Friendly object
 */
SDL_Color Friendly::getSonarColor() {
	return sonar_color;
}
