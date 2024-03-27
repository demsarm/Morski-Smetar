#include "Trash.h"

#include "../Tools/Random.h"
#include "../Config/Config.h"

/**
 * @brief Default constructor for the Trash class
 */
Trash::Trash() : GameObject() {
	type = (Type)Random::randint(0, 3);
	design.path = trash_types[(int)type];
	
	initial_y = rect.y;
	offset = Random::randint(0, 50);
}

/**
 * @brief Constructor for the Trash class
 * @param x x coordinate of the Trash object
 * @param y y coordinate of the Trash object
 */
Trash::Trash(int x, int y) : GameObject() {
	rect = {x, y, 50, 50};
	type = (Type)Random::randint(0, 3);
	design.path = trash_types[(int)type];
	
	initial_y = rect.y;
	offset = Random::randint(0, 50);
}

/**
 * @brief Copy constructor for the Trash class
 * @param other the Trash object to be copied
 */
[[maybe_unused]] Trash::Trash(const Trash &other) = default;

/**
 * @brief Updates the Trash object
 *
 * This function updates the Trash object by moving it to the right and changing its y coordinate to simulate a wave.
 */
[[maybe_unused]]
void Trash::Update() {
	rect.x += 1;
	rect.y = (int)((double)initial_y + sin(0.03 * (rect.x + offset)) * 5);
}

/**
 * @brief Returns the type of the Trash object
 * @return the type of the Trash object
 */
[[maybe_unused]] Trash::Type Trash::getType() const {
	return type;
}

void Trash::setType(Trash::Type newType) {
	type = newType;
	
	design.path = trash_types[(int)type];
}
