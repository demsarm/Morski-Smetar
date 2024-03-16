// Credit where credit is due - I got the idea from gagaboom1 (aka. Gašper Horvatić)
// The code is mine though - the code is mainly just what was in main() before - I just moved it to a class to make it cleaner

#ifndef MORSKI_SMETAR_GAME_H
#define MORSKI_SMETAR_GAME_H

#include "SDL2/SDL.h"

#include <vector>

#include "UI/Window.h"

#include "Tools/Random.h"
#include "Tools/Collision.h"
#include "Tools/Overloads.h"
#include "Tools/Math.h"

#include "Entities/GameObject.h"
#include "Entities/Player.h"
#include "Entities/Wave.h"
#include "Entities/Trash.h"
#include "UI/Text.h"

#include "Config/Config.h"
#include "Config/Data.h"
#include "Entities/Enemy.h"
#include "Entities/Friendly.h"
#include "UI/Line.h"

class Game {
	Window window = Window("Morski Smetar", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	std::string username;
	std::multimap<std::string, int> highscores;
	
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
	Screen mainMenuScreen;
	Screen usernameScreen;
	int backspaceCooldown = 0;
	bool open = true;
	
	void generateMainMenu();
	void generateGameOverScreen();
	void generateUsernameScreen();
public:
	Game() = default;
	[[nodiscard]] bool isOpen() const;
	void Setup();
	void Update();
	void PlayingUpdate();
	void MainMenuUpdate();
	void GameOverUpdate();
	void UsernameUpdate();
	void CheckWindowEvents();
	void Render(); // Having a separate fuction to render makes the code marginally cleaner, but may cause inefficiencies as some containers might be parsed multiple times (enemies, friendlies, waves & trash)
	void CompleteStage();
	void Restart();
	void writeHighscores();
};


#endif //MORSKI_SMETAR_GAME_H
