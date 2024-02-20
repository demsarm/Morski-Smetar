//
// Created by martind on 17.2.2024.
//

#include "Config.hpp"

namespace WindowData {
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;
	int SCREEN_FPS = 60;
}

namespace Config{
	int ENEMY_ATTACK_CD = 60;
	int PLAYER_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 2;
	int ALLY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 4;
	int ENEMY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 3;
	int ENEMY_LINK_RANGE = WindowData::SCREEN_WIDTH / 3;
}