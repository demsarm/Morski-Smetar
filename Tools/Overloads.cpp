#include "Overloads.h"

// Can you even add docstrings to overloads?

SDL_Rect operator+(const SDL_Rect &lhs, const SDL_Rect &rhs) {
	return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.h};
}


bool operator!=(const SDL_Rect &lhs, const SDL_Rect &rhs) {
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.w != rhs.w || lhs.h != rhs.h;
}

std::string operator*(const std::string &str, int times) {
	std::string result;
	for (int i = times; i--;) {
		result += str;
	}
	return result;
}
