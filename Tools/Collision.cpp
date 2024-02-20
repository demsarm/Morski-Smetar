#include "Collision.hpp"

/*
 * @brief Checks if two rectangles are colliding
 */
bool isColliding(const SDL_Rect &a, const SDL_Rect &b) {
	return a.x + a.w >= b.x && b.x + b.w >= a.x && a.y + a.h >= b.y && b.y + b.h >= a.y;
}

/*
 * @brief Checks if two GameObjects are colliding
 */
bool isColliding(const GameObject &a, const GameObject &b) {
	return isColliding(a.getRect(), b.getRect());
}