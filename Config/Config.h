#ifndef MORSKI_SMETAR_CONFIG_H
#define MORSKI_SMETAR_CONFIG_H

// Data about how the game should run

#include <SDL2/SDL_pixels.h> // Don't need all of SDL2, just the color struct

namespace WindowData {
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;
	extern int SCREEN_FPS;
}

namespace Config{
	extern int ENEMY_ATTACK_CD;
	extern int PLAYER_SIGHT_RANGE;
	extern int ALLY_SIGHT_RANGE;
	
	extern int ENEMY_SIGHT_RANGE;
	extern int ENEMY_LINK_RANGE;
	
	extern SDL_Color BUTTON_COLOR;
	extern SDL_Color TEXT_COLOR;
}

#endif //MORSKI_SMETAR_CONFIG_H
