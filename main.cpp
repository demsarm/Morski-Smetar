//#define SDL_MAIN_HANDLED // Apparently needed for mingw to work

#include "Config/Data.h"

#include "Game.h"
#include "UI/Input.h"

using namespace std;

#ifdef _WIN32
int WinMain() {
#else
int main() {
#endif
//	SDL_SetMainReady(); // If there are problems, try removing this line to see if anything's changed. It lets SDL functions know the initialization went well.
	
	Game game;
	game.Setup();
	
	// Game loop
	while (game.isOpen()) {
		Input::Update();
		// If the player is alive, play the game
		if (Data::isAlive) {
			game.Update();
		}
		else { // If the player is dead (game over), show the game over screen
			if (Input::getKey("R")) {
				game.Restart();
			}
		}
		game.Render();
		game.CheckWindowEvents();
	}
	return 0;
}