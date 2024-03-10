#ifndef MORSKI_SMETAR_COLLISION_H
#define MORSKI_SMETAR_COLLISION_H

#include "SDL2/SDL.h"
#include "../Entities/GameObject.h"

bool isColliding(const SDL_Rect &a, const SDL_Rect &b);
bool isColliding(const GameObject &a, const GameObject &b);

bool isPointInRect(const std::pair<int, int> &point, const SDL_Rect &rect);
bool isPointInRect(const std::pair<int, int> &point, const GameObject &object);
#endif //MORSKI_SMETAR_COLLISION_H
