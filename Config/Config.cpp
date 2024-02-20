#include "Config.hpp"

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
}