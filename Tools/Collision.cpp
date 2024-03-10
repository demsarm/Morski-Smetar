#include "Collision.h"

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

bool isPointInRect(const std::pair<int, int> &point, const SDL_Rect &rect) {
	return point.first >= rect.x && point.first <= rect.x + rect.w && point.second >= rect.y && point.second <= rect.y + rect.h;
}

bool isPointInRect(const std::pair<int, int> &point, const GameObject &object) {
	return isPointInRect(point, object.getRect());
}