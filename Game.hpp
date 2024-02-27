// Credit where credit is due - I got the idea from gagaboom1 (aka. Gašper Horvatić)
// The code is mine though - the code is mainly just what was in main() before - I just moved it to a class to make it cleaner

#ifndef MORSKI_SMETAR_GAME_HPP
#define MORSKI_SMETAR_GAME_HPP

#include "SDL2/SDL.h"

#include <vector>

#include "UI/Window.hpp"

#include "Tools/Random.hpp"
#include "Tools/Collision.hpp"
#include "Tools/Overloads.hpp"
#include "Tools/Math.hpp"

#include "Entities/GameObject.hpp"
#include "Entities/Player.hpp"
#include "Entities/Wave.hpp"
#include "Entities/Trash.hpp"
#include "UI/Text.hpp"

#include "Config/Config.hpp"
#include "Config/Data.hpp"
#include "Entities/Enemy.hpp"
#include "Entities/Friendly.hpp"
#include "UI/Line.hpp"

class Game {
	Window window = Window("Morski Smetar", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	Player player;
	std::vector<Wave> waves;
	std::vector<Trash> trash;
	std::vector<Enemy> enemies;
	std::vector<Friendly> friendlies;
	std::vector<GameObject> warnings;
	std::vector<Line> lines;
	std::vector<Text> texts;
//	Text score;
	GameObject land;
	GameObject * disembark_indicator = nullptr;
	
	Screen gameOverScreen;
	
	
	bool open = true;
public:
	Game() = default;
	[[nodiscard]] bool isOpen() const;
	void Setup();
	void Update();
	void CheckWindowEvents();
	void Render(); // Having a separate fuction to render makes the code marginally cleaner, but may cause inefficiencies as some containers might be parsed multiple times (enemies, friendlies, waves & trash)
	void CompleteStage();
	void Restart();
};


#endif //MORSKI_SMETAR_GAME_HPP
