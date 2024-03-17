#include "Config.h"

// Data about how the game should run

namespace WindowData {
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;
	int SCREEN_FPS = 360;
}

namespace Config{
	int ENEMY_ATTACK_CD = 60;
	int PLAYER_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 2;
	int ALLY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 4;
	int ENEMY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 3;
	int ENEMY_LINK_RANGE = WindowData::SCREEN_WIDTH / 4;
	
	SDL_Color BUTTON_COLOR = {0xc0, 0xc0, 0xc0, 0xff}; // I came up with 0xc0 on the fly, can't be bothered to convert to decimal
	SDL_Color TEXT_COLOR = {0xff, 0xff, 0xff, 0xff};
}