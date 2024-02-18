#include "SDL2/SDL.h"

#include <vector>

#include "UI/Window.hpp"

#include "Tools/Random.hpp"
#include "Tools/Collision.hpp"
#include "Tools/Overloads.hpp"

#include "Entities/GameObject.hpp"
#include "Entities/Player.hpp"
#include "Entities/Wave.hpp"
#include "Entities/Trash.hpp"
#include "Entities/Text.hpp"

#include "Config/Config.hpp"
#include "Config/Data.hpp"

using namespace std;

#define LETTER_WIDTH 35

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
	player.setRect({Random::randint((int)(WindowData::SCREEN_WIDTH * 11 / 30), (int)(WindowData::SCREEN_WIDTH * 0.9)), Random::randint(500, 800), 160, 115});
	
	std::vector<Wave> waves;
	for (int i = 0; i < 20; i++){
		waves.push_back(Wave(Random::randint(500, WindowData::SCREEN_WIDTH), (i % 10) * 160)); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
		waves[i].setPath("Assets/Wave.png");
	}
	
	std::vector<Trash> trash;
	for (int i = 20; i--;) {
		trash.push_back(Trash(Random::randint(WindowData::SCREEN_WIDTH/3, WindowData::SCREEN_WIDTH - 50), Random::randint( WindowData::SCREEN_HEIGHT - 50)));
	}
	
	Text score;
	score.setText(string("Score-O-Meter"));
	score.setFontPath(string("Assets/Fonts/VCR_OSD_MONO.ttf"));
	score.setFontSize(30);
	score.setColor((SDL_Color){255, 255, 255, 255});
	score.setRect({10, 10, 200, 50});
	
	GameObject land;
	land.setColor((SDL_Color){220, 220, 120, 255});
	land.setRect({0, 0, WindowData::SCREEN_WIDTH/3, WindowData::SCREEN_HEIGHT});
	
	while (window.isRunning()){
		window.Clear((SDL_Color){50, 210, 255, 255});
		
		for (auto &wave : waves){
			wave.Update();
			window.Draw(wave);
		}
		
		window.Draw(land);
		
		for (uint64_t i = trash.size(); i--;) {
			trash[i].Update();
			if (isColliding(player, trash[i])) {
				trash.erase(trash.begin() + (int64_t)i);
				++Data::score;
			}
			window.Draw(trash[i]);
		}
		
		player.Update();
		window.Draw(player, 0.0, player.isFacingLeft() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
		
		if (player.getBoatPosition() != (SDL_Rect){0, 0, 0, 0}){
			GameObject boat;
			boat.setPath("Assets/Boat.png");
			boat.setRect(player.getBoatPosition());
			window.Draw(boat);
		}
		
		score.setText(string("Score: ") + to_string(Data::score));
		window.Draw(score);
		if (isColliding(player.getRect(), (SDL_Rect){WindowData::SCREEN_WIDTH / 3, 0, WindowData::SCREEN_WIDTH / 6, WindowData::SCREEN_HEIGHT})){
			std::string str = "Press E to disembark";
			Text embark(str, string("Assets/Fonts/VCR_OSD_MONO.ttf"), 30, (SDL_Color){255, 255, 255, 255});
			embark.setRect({WindowData::SCREEN_WIDTH/2 - 400, WindowData::SCREEN_HEIGHT - 100, LETTER_WIDTH * (int)str.size(), 50});
			window.Draw(embark);
			
			GameObject disembark_loc;
			disembark_loc.setPath("Assets/Disembark-Indicator.png");
			disembark_loc.setRect({WindowData::SCREEN_WIDTH/3 - (player.getRect().x - WindowData::SCREEN_WIDTH/3 + 80), player.getRect().y, 80, 115});
			window.Draw(disembark_loc, 0.0, player.isFacingRight() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
		}
		if (isColliding(player.getRect() + (SDL_Rect){player.getRect().w, 0, 0, 0}, {player.getBoatPosition().x - WindowData::SCREEN_WIDTH/12, player.getBoatPosition().y - 50, player.getBoatPosition().w + WindowData::SCREEN_WIDTH/6, player.getBoatPosition().h + 100})){
			std::string str = "Press E to embark";
			Text embark(str, string("Assets/Fonts/VCR_OSD_MONO.ttf"), 30, (SDL_Color){255, 255, 255, 255});
			embark.setRect({WindowData::SCREEN_WIDTH/2 - 400, WindowData::SCREEN_HEIGHT - 100, LETTER_WIDTH * (int)str.size(), 50});
			window.Draw(embark);
		}
		
		window.Flip();
	}
}
