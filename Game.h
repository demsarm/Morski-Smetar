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
#include "Tools/BinaryFile.h"

#include "Recording/Recorder.h"
#include "Recording/Reader.h"

class Game {
	Window window = Window("Morski Smetar", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	std::string username;
	struct Compare {
		bool operator()(int a, int b) const { return a > b; }
	};
	std::multimap<int, std::string, Compare> highscores;
	
	Player player;
	std::vector<Wave> waves;
	std::vector<Trash> trash;
	std::vector<Enemy> enemies;
	std::vector<Friendly> friendlies;
	std::vector<GameObject> warnings;
	std::vector<Line> lines;
	std::vector<Text> texts;
	
	GameObject land;
	GameObject * disembark_indicator = nullptr;
	
	Screen gameOverScreen;
	Screen mainMenuScreen;
	Screen usernameScreen;
	Screen leaderboardScreen;
	Screen playbackEndScreen;
	Screen pauseScreen;
	
	Recorder recorder;
	Reader reader;
	std::vector<GameObject> playback_objs;
	
	int backspaceCooldown = 0;
	int escapeCooldown = 0;
	bool open = true;
	
	void generateMainMenu();
	void generateGameOverScreen();
	void generateUsernameScreen();
	void generateLeaderboardScreen();
	void generatePlaybackEndScreen();
	void generatePauseScreen();
public:
	Game() = default;
	[[nodiscard]] bool isOpen() const;
	void Setup();
	void Update();
	void PlayingUpdate();
	void PausedUpdate();
	void MainMenuUpdate();
	void GameOverUpdate();
	void UsernameUpdate();
	void LeaderboardUpdate();
	void PlaybackUpdate();
	void PlaybackEndUpdate();
	void CheckWindowEvents();
	void Render(); // Having a separate fuction to render makes the code marginally cleaner, but may cause inefficiencies as some containers might be parsed multiple times (enemies, friendlies, waves & trash)
	void CompleteStage();
	void Restart();
	void writeHighscores();
	
	
	[[nodiscard]] [[maybe_unused]]
	const Player &getPlayer() const; // Probably over 2 months in and Game gets its first getter
	[[nodiscard]] [[maybe_unused]]
	std::vector<Trash> getTrash() const;
	[[nodiscard]] [[maybe_unused]]
	std::vector<Enemy> getEnemies() const;
	[[nodiscard]] [[maybe_unused]]
	std::vector<Friendly> getFriendlies() const;
};


#endif //MORSKI_SMETAR_GAME_H
