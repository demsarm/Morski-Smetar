#ifndef MORSKI_SMETAR_COLLISION_HPP
#define MORSKI_SMETAR_COLLISION_HPP

#include "SDL2/SDL.h"
#include "../Entities/GameObject.hpp"

bool isColliding(const SDL_Rect &a, const SDL_Rect &b);
bool isColliding(const GameObject &a, const GameObject &b);

#endif //MORSKI_SMETAR_COLLISION_HPP
