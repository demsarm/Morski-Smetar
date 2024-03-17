#ifndef MORSKI_SMETAR_OVERLOADS_H
#define MORSKI_SMETAR_OVERLOADS_H

#include <SDL2/SDL.h>
#include <string>
SDL_Rect operator+(const SDL_Rect &lhs, const SDL_Rect &rhs);
bool operator!=(const SDL_Rect &lhs, const SDL_Rect &rhs);
std::string operator*(const std::string &str, int times);

#endif //MORSKI_SMETAR_OVERLOADS_H
