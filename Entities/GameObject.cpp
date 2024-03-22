#include "GameObject.h"

/**
 * @brief Default constructor for GameObject
 */
GameObject::GameObject() {
	rect = {0, 0, 0, 0};
	design.color = {0, 0, 0, 0};
}

/**
 * @brief Copy constructor for GameObject
 * @param other The GameObject to copy
 */
[[maybe_unused]] GameObject::GameObject(const GameObject &other) {
	rect = other.rect;
	design = other.design;
}

/**
 * @brief Destructor for GameObject
 */
GameObject::~GameObject() = default;

/**
 * @brief Moves the GameObject by the specified amount
 * @param x The amount to move the GameObject on the x-axis
 * @param y The amount to move the GameObject on the y-axis
 */
void GameObject::move(int x, int y) {
	rect.x += x;
	rect.y += y;
}

/**
 * @brief Returns the design of the GameObject
 * @return The design of the GameObject
 */
Design GameObject::getDesign() const {
	return design;
}

/**
 * @brief Returns the rectangle of the GameObject
 * @return The rectangle of the GameObject
 */
// This causes segfault - 'this' is NULL at some point for whatever reason
SDL_Rect GameObject::getRect() const {
	return rect;
}

/**
 * @brief Sets the color of the GameObject
 * @param color The color to set the GameObject to
 */
void GameObject::setColor(const SDL_Color& color) {
	design.color = color;
}

/**
 * @brief Sets the path of the GameObject
 * @param path The path to set the GameObject to
 */
void GameObject::setPath(const std::string& path) {
	design.path = path;
}

/**
 * @brief Sets the rectangle of the GameObject
 * @param rect1 The rectangle to set the GameObject to
 */
void GameObject::setRect(const SDL_Rect& rect1) {
	this->rect = rect1;
}

/**
 * Does nothing by default - to be overridden by child classes
 */
void GameObject::Update() {
	if (rect.x < 0) { // Force the object's position to be non-negative - it's a bad solution, however, since I am on a time limit, I can't waste time finding out why the object's position is -3 on 7 frames
		rect.x = 0;
	}
	if (rect.y < 0) {
		rect.y = 0;
	}
}
