#define SDL_MAIN_HANDLED

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

#include "Game.hpp"

using namespace std;


int main() {
	SDL_SetMainReady(); // If there are problems, try removing this line to see if anything's changed. It lets SDL functions know the initialization went well.
	
	// Because the SDL documentation is godawful and doesn't list the values of the SDL_Event types, I'm going to do it here.
	/*
	std::cout << "SDL_QUIT: " << SDL_QUIT << std::endl;
	std::cout << "SDL_WINDOWEVENT_CLOSE: " << SDL_WINDOWEVENT_CLOSE << std::endl;
	std::cout << "SDL_KEYDOWN: " << SDL_KEYDOWN << std::endl;
	std::cout << "SDL_KEYUP: " << SDL_KEYUP << std::endl;
	*/
	
	Game game;
	game.Setup();
	
	// Game loop
	while (game.isOpen()) {
		// If the player is alive, play the game
		if (Data::isAlive) {
			game.Update();
		}
		else { // If the player is dead (game over), show the game over screen
			
			const Uint8 * keystate = SDL_GetKeyboardState(nullptr);
			if (keystate[SDL_SCANCODE_R]){
				game.Restart();
			}
		}
		game.Render();
		game.CheckWindowEvents();
	}
	return 0;
}