#ifndef MORSKI_SMETAR_DATA_H
#define MORSKI_SMETAR_DATA_H

// Data used during the game

namespace Data {
	enum GameState {
		MAIN_MENU,
		PLAYING,
		GAME_OVER,
		USERNAME // Username input screen
	};

	extern int score;
	extern int difficulty;
	extern int sonar;
	extern int friendly_sonar;
	extern GameState gameState;
}

#endif //MORSKI_SMETAR_DATA_H
