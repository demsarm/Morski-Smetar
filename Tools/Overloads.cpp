#include "Overloads.h"

// Can you even add docstrings to overloads?

SDL_Rect operator+(const SDL_Rect &lhs, const SDL_Rect &rhs) {
	return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.h};
}


bool operator!=(const SDL_Rect &lhs, const SDL_Rect &rhs) {
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.w != rhs.w || lhs.h != rhs.h;
}
