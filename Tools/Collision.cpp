#include "Collision.hpp"

bool isColliding(const SDL_Rect &a, const SDL_Rect &b) {
	return a.x + a.w >= b.x && b.x + b.w >= a.x && a.y + a.h >= b.y && b.y + b.h >= a.y;
}

bool isColliding(const GameObject &a, const GameObject &b) {
	return isColliding(a.getRect(), b.getRect());
}