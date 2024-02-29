#ifndef MORSKI_SMETAR_OVERLOADS_H
#define MORSKI_SMETAR_OVERLOADS_H

#include <SDL2/SDL.h>

SDL_Rect operator+(const SDL_Rect &lhs, const SDL_Rect &rhs);
bool operator!=(const SDL_Rect &lhs, const SDL_Rect &rhs);

#endif //MORSKI_SMETAR_OVERLOADS_H
