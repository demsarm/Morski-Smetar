#include "SDL2/SDL.h"

#include <vector>

#include "UI/Window.hpp"

#include "Tools/Random.hpp"
#include "Tools/Collision.hpp"

#include "Entities/Player.hpp"
#include "Entities/Wave.hpp"
#include "Entities/Trash.hpp"

#include "Config/Config.hpp"

int main() {
	Window::init();
	Window::setWindowSize(getScreenSize());
	Window window(
			"SDL Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WindowData::SCREEN_WIDTH,
			WindowData::SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN)
	;
	
	Player player;
	player.setPath("Assets/Boat.png");
	player.setRect({Random::randint((int)(WindowData::SCREEN_WIDTH * 0.15), WindowData::SCREEN_WIDTH - (int)(WindowData::SCREEN_WIDTH * 0.15)), Random::randint(500, 800), 160, 115});
	
	std::vector<Wave> waves;
	for (int i = 0; i < 10; i++){
		waves.push_back(Wave(Random::randint(500, WindowData::SCREEN_WIDTH), i * 160)); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
		waves[i].setPath("Assets/Wave.png");
	}
	
	std::vector<Trash> trash;
	for (int i = 10; i--;) {
		trash.push_back(Trash(Random::randint(500, 800), Random::randint( WindowData::SCREEN_HEIGHT)));
	}
	
	while (window.isRunning()){
		window.Clear((SDL_Color){50, 210, 255, 255});
		
		for (auto &wave : waves){
			wave.Update();
			window.Draw(wave);
		}
		
		for (uint64_t i = trash.size(); i--;) {
			trash[i].Update();
			if (isColliding(player, trash[i])) {
				trash.erase(trash.begin() + (int64_t)i);
			}
			window.Draw(trash[i]);
			
		}
		
		player.Update();
		window.Draw(player, 0.0, player.isFacingLeft() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
		window.Flip();
	}
}
