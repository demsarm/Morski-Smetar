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
#include "Entities/Text.hpp"

#include "Config/Config.hpp"
#include "Config/Data.hpp"
#include "Entities/Enemy.hpp"
#include "Entities/Friendly.hpp"

using namespace std;

#define LETTER_WIDTH 35

int main() {
	// Create the window
	Window::init();
	Window::setWindowSize(800, 600);
	Window window(
			"SDL Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WindowData::SCREEN_WIDTH,
			WindowData::SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
	
	// Create the player
	Player player;
	player.setPath("Assets/Boat.png");
	player.setRect({Random::randint((int) (WindowData::SCREEN_WIDTH * 21 / 30), (int) (WindowData::SCREEN_WIDTH * 0.9)),
	                Random::randint(WindowData::SCREEN_HEIGHT / 10, WindowData::SCREEN_HEIGHT * 9 / 10), 160, 115});
	player.setSonarColor((SDL_Color) {0, 255, 0, 255});
	// Make the enemies know about the player
	Enemy::setPlayer(&player);
	
	// Create the waves - they aren't necessary for the game, but are still a nice touch
	std::vector<Wave> waves;
	for (int i = 0; i < 20; i++) {
		waves.push_back(Wave(Random::randint(500, WindowData::SCREEN_WIDTH),
		                     (i % 10) * 160)); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
		waves[i].setPath("Assets/Wave.png");
	}
	
	// Create trash and drop some in the water
	std::vector<Trash> trash;
	Enemy::setTrashes(&trash);
	for (int i = 10; i--;) {
		trash.push_back(Trash(Random::randint(WindowData::SCREEN_WIDTH / 3, (WindowData::SCREEN_WIDTH * 2) / 3 - 50),
		                      Random::randint(WindowData::SCREEN_HEIGHT - 50)));
	}
	
	// Create the enemies
	std::vector<Enemy> enemies;
	Enemy::setEnemies(&enemies);
	for (int i = Data::difficulty; i--;) {
		Enemy enemy;
		enemy.setPath("Assets/Enemy.png");
		enemy.setRect({Random::randint(WindowData::SCREEN_WIDTH / 12, WindowData::SCREEN_WIDTH / 4),
		               Random::randint(WindowData::SCREEN_HEIGHT / 12, WindowData::SCREEN_HEIGHT * 11 / 12), 60, 65});
		enemies.push_back(enemy);
	}
	
	// Create the friendlies
	std::vector<Friendly> friendlies;
	for (int i = Data::difficulty; i--;) {
		friendlies.push_back(Friendly());
	}
	Friendly::setSonarColor((SDL_Color) {0, 0x88, 0, 255}); // Consistent number bases are for the weak and the uninitiated (Disclaimer: I am weak, however I may or may not be initiated - I'm not sure)
	
	// Create the score display
	Text score;
	score.setText(string("Score-O-Meter"));
	score.setFontPath(string("Assets/Fonts/VCR_OSD_MONO.ttf"));
	score.setFontSize(30);
	score.setColor((SDL_Color) {255, 255, 255, 255});
	score.setRect({10, 10, 200, 50});
	
	// Let there be land
	// TODO: Make the land look nicer, be it with textures or something like the waves on the water
	GameObject land;
	land.setColor((SDL_Color) {220, 220, 120, 255});
	land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
	
	// Game loop
	while (window.isRunning()) {
		// If the player is alive, play the game
		if (Data::isAlive) {
			// Clear the window
			window.Clear((SDL_Color) {50, 210, 255, 255});
			
			// Draw the waves
			for (auto &wave: waves) {
				wave.Update();
				window.Draw(wave);
			}
			
			// Let there be land V2
			window.Draw(land);
			
			for (uint64_t i = trash.size(); i--;) {
				// Move the trash
				trash[i].Update();
				
				//Trash pickup
				if (isColliding(player, trash[i])) {
					trash.erase(trash.begin() +
					            (int64_t) i); // Yes that is how I cast to long I may or may not be chronically deranged
					++Data::score;
				}
				for (auto & friendly : friendlies) {
					if (isColliding(friendly, trash[i])) {
						trash.erase(trash.begin() +
						            (int64_t) i); // Yes that is how I cast to long I may or may not be chronically deranged
						++Data::score;
					}
				}
				// Lose points if trash goes of screen
				if (trash[i].getRect().x > WindowData::SCREEN_WIDTH + 50) {
					trash.erase(trash.begin() +
					            (int64_t) i); // Yes that is how I cast to long I may or may not be chronically deranged
					--Data::score;
				}
				// Draw the trash
				window.Draw(trash[i]);
			}
			
			for (uint64_t i = enemies.size(); i--;) {
				// Make the enemies take action
				enemies[i].Update();
				// Telegraphing
				for (auto &other: enemies) {
					// Warn the player if two enemies are colliding
					if (isColliding(enemies[i], other) && enemies[i] != other && distance(enemies[i].getRect().x, enemies[i].getRect().y, player.getRect().x, player.getRect().y) < (double) Config::PLAYER_SIGHT_RANGE) {
						GameObject warn;
						warn.setPath("Assets/Exclamation.png");
						warn.setRect({enemies[i].getRect().x + enemies[i].getRect().w / 2 - 20,
						              enemies[i].getRect().y - 60, 30, 55});
						window.Draw(warn);
					}
					// Draw a line between enemies if they are close enough and the player can see them - figure it out if you want to know more iont feel like explaining
					if (other != enemies[i] &&
						distance(enemies[i].getRect().x, enemies[i].getRect().y, other.getRect().x, other.getRect().y) < (double) Config::ENEMY_LINK_RANGE &&
						distance(player.getRect().x, player.getRect().y, enemies[i].getRect().x, enemies[i].getRect().y) < (double) Config::PLAYER_SIGHT_RANGE
						) {
							if (distance(player.getRect().x, player.getRect().y, other.getRect().x, other.getRect().y) < (double)Config::PLAYER_SIGHT_RANGE) {
								window.DrawLine(
										enemies[i].getRect().x + enemies[i].getRect().w / 2,
										enemies[i].getRect().y + enemies[i].getRect().h / 2,
										other.getRect().x + other.getRect().w / 2,
										other.getRect().y + other.getRect().h / 2,
										3, (SDL_Color) {255, 0, 0,
										                255}); // This will reveal the location of SOME enemies, but it does make the game fairer
							} else {
								std::pair<int, int> inter = line_circle_intersection(
										enemies[i].getRect().x + enemies[i].getRect().w / 2, enemies[i].getRect().y + enemies[i].getRect().h / 2,
										other.getRect().x + other.getRect().w / 2, other.getRect().y + other.getRect().h / 2,
										player.getRect().x + player.getRect().w / 2, player.getRect().y + player.getRect().h / 2,
										Config::PLAYER_SIGHT_RANGE);
								if (inter.first != -1) {
									window.DrawLine(
											enemies[i].getRect().x + enemies[i].getRect().w / 2,
											enemies[i].getRect().y + enemies[i].getRect().h / 2,
											inter.first, inter.second,
											3, (SDL_Color) {255, 0, 0, 255});
								}
							}
					}
				}
				// Either kill the player or the enemy if they collide
				if (isColliding(player, enemies[i])) {
					bool game = false;
					for (auto &other: enemies) {
						if (other != enemies[i] && sqrt(
								pow(other.getRect().x - enemies[i].getRect().x, 2) +
								pow(other.getRect().y - enemies[i].getRect().y, 2)) <
						                           (double) Config::ENEMY_LINK_RANGE) {
							game = true;
							break;
						}
					}
					if (!game) {
						enemies.erase(enemies.begin() +
						              (int64_t) i); // Yes that is how I cast to long I may or may not be chronically deranged
						Data::score += 2;
					} else {
						Data::isAlive = false;
					}
				}
				// Draw the enemies if they can be seen
				bool show = false;
				for (auto & friendly : friendlies){
					if (friendly.getType() == Friendly::Type::LAND && distance(friendly.getRect().x, friendly.getRect().y, enemies[i].getRect().x, enemies[i].getRect().y) <= (double) Config::ALLY_SIGHT_RANGE){
						show = true;
						break;
					}
				}
				if (show || (int) (sqrt(
								pow(player.getRect().x - enemies[i].getRect().x, 2) +
								   pow(player.getRect().y - enemies[i].getRect().y, 2))) < Config::PLAYER_SIGHT_RANGE) {
					window.Draw(enemies[i]);
				}
			}
			
			for (uint64_t i = friendlies.size(); i--;) {
				// Move the friendlies
				friendlies[i].Update();
				// Kill the friendly if the player collides with it
				if (isColliding(player, friendlies[i])) {
					friendlies.erase(friendlies.begin() +
					                 (int64_t) i); // Yes that is how I cast to long I may or may not be chronically deranged
					--Data::score;
				} else {
					// Draw the friendly's 'sonar'
					if (friendlies[i].getType() == Friendly::Type::LAND) {
						window.DrawCircle(friendlies[i].getRect().x + friendlies[i].getRect().w / 2,
						                  friendlies[i].getRect().y + friendlies[i].getRect().h / 2,
						                  Data::friendly_sonar, 10, Friendly::getSonarColor(), 32);
						window.DrawCircle(friendlies[i].getRect().x + friendlies[i].getRect().w / 2,
						                  friendlies[i].getRect().y + friendlies[i].getRect().h / 2,
						                  Config::ALLY_SIGHT_RANGE, 10, Friendly::getSonarColor(), 32);
					}
					// Draw the friendly
					window.Draw(friendlies[i], 0.0, friendlies[i].getDirection() == Friendly::Direction::NEGATIVE ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
				}
			}
			// Move the friendly's "inner sonar"
			++Data::friendly_sonar;
			if (Data::friendly_sonar > Config::ALLY_SIGHT_RANGE) {
				Data::friendly_sonar = 0;
			}
			
			// Move the player
			player.Update();
			// Draw the player and his 'sonar'
			window.DrawCircle(player.getRect().x + player.getRect().w / 2, player.getRect().y + player.getRect().h / 2,
			                  Config::PLAYER_SIGHT_RANGE, 10, player.getSonarColor(), 32);
			window.DrawCircle(player.getRect().x + player.getRect().w / 2, player.getRect().y + player.getRect().h / 2,
			                  Data::sonar, 10, player.getSonarColor(), 32);
			window.Draw(player, 0.0, player.isFacingLeft() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
			// Move the player's "inner sonar"
			++Data::sonar;
			if (Data::sonar > Config::PLAYER_SIGHT_RANGE) {
				Data::sonar = 0;
			}
			
			// Draw the boat if the player isn't on it
			if (player.getBoatPosition() != (SDL_Rect) {0, 0, 0, 0}) {
				GameObject boat;
				boat.setPath("Assets/Boat.png");
				boat.setRect(player.getBoatPosition());
				window.Draw(boat);
			}
			
			// Draw the score
			score.setText(string("Score: ") + to_string(Data::score));
			window.Draw(score);
			
			// Draw the disembark text and indicator
			if (isColliding(player.getRect(), (SDL_Rect) {
														  WindowData::SCREEN_WIDTH / 3, 0,
														  WindowData::SCREEN_WIDTH / 6,
			                                              WindowData::SCREEN_HEIGHT})) {
				std::string str = "Press E to disembark";
				Text embark(str, string("Assets/Fonts/VCR_OSD_MONO.ttf"), 30, (SDL_Color) {255, 255, 255, 255});
				embark.setRect({WindowData::SCREEN_WIDTH / 2 - 400,
								WindowData::SCREEN_HEIGHT - 100,
				                LETTER_WIDTH * (int) str.size(), 50});
				window.Draw(embark);
				
				GameObject disembark_loc;
				disembark_loc.setPath("Assets/Disembark-Indicator.png");
				disembark_loc.setRect(
						{WindowData::SCREEN_WIDTH / 3 - (player.getRect().x - WindowData::SCREEN_WIDTH / 3 + 80),
						 player.getRect().y, 80, 115});
				window.Draw(disembark_loc, 0.0, player.isFacingRight() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
			}
			
			// Draw the embark text
			if (isColliding(player.getRect() + (SDL_Rect) {player.getRect().w, 0, 0, 0}, {
					player.getBoatPosition().x - WindowData::SCREEN_WIDTH / 8,
					player.getBoatPosition().y - 50,
					player.getBoatPosition().w + WindowData::SCREEN_WIDTH / 4,
					player.getBoatPosition().h + 100})) {
				std::string str = "Press E to embark";
				Text embark(str, string("Assets/Fonts/VCR_OSD_MONO.ttf"), 30, (SDL_Color) {255, 255, 255, 255});
				embark.setRect({WindowData::SCREEN_WIDTH / 2 - 400, WindowData::SCREEN_HEIGHT - 100,
				                LETTER_WIDTH * (int) str.size(), 50});
				window.Draw(embark);
			}
			
			// Draw everything on the window
			window.Flip();
			
			// If the stage is complete, move on to the next and increase the difficulty
			if (enemies.empty() && trash.empty()) {
				++Data::difficulty;
				// Spawn more enemies, friendlies and trash
				for (int i = Data::difficulty; i--;) {
					Enemy enemy;
					enemy.setPath("Assets/Enemy.png");
					enemy.setRect({Random::randint(WindowData::SCREEN_WIDTH / 12, WindowData::SCREEN_WIDTH / 4),
					               Random::randint(WindowData::SCREEN_HEIGHT / 12, WindowData::SCREEN_HEIGHT * 11 / 12),
					               60,
					               65});
					for (auto &other: enemies) {
						if (sqrt(
								pow(other.getRect().x - enemy.getRect().x, 2) +
								   pow(other.getRect().y - enemy.getRect().y, 2)) < (double) Config::ENEMY_LINK_RANGE) {
							enemy.setAxis(other.getAxis() == Enemy::Axis::X ? Enemy::Axis::Y : Enemy::Axis::X);
						}
					}
					enemies.push_back(enemy);
				}
				for (int i = 10; i--;) {
					trash.push_back(
							Trash(Random::randint(WindowData::SCREEN_WIDTH / 3,
							                      (WindowData::SCREEN_WIDTH * 2) / 3 - 50),
							      Random::randint(WindowData::SCREEN_HEIGHT - 50)));
				}
				friendlies.clear();
				for (int i = Data::difficulty; i--;) {
					friendlies.push_back(Friendly());
				}
				
				// Increase the window size
				if (800 + 100 * Data::difficulty < window.getScreenSize().first) {
					std::pair<int, int> pos = window.getWindowsPosition();
					pos.first -= 50;
					window.setWindowsPosition(pos.first, pos.second);
				}
				if (600 + 100 * Data::difficulty < window.getScreenSize().second) {
					std::pair<int, int> pos = window.getWindowsPosition();
					pos.second -= 50;
					window.setWindowsPosition(pos.first, pos.second);
				}
				Window::setWindowSize(
						800 + 100 * Data::difficulty < window.getScreenSize().first ? 800 + 100 * Data::difficulty : window.getScreenSize().first,
						600 + 100 * Data::difficulty < window.getScreenSize().second ? 600 + 100 * Data::difficulty : window.getScreenSize().second);
				window.changeWindowSize(WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
				window.centerWindow();
				// Update everything to work with the new window size
				land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
				player.setBoatPosition({0, 0, 0, 0});
				player.setState(Player::EMBARKED);
				player.setPath("Assets/Boat.png");
				player.setRect({Random::randint((int) (WindowData::SCREEN_WIDTH * 21 / 30), (int) (WindowData::SCREEN_WIDTH * 0.9)),
				                Random::randint(WindowData::SCREEN_HEIGHT / 10, WindowData::SCREEN_HEIGHT * 9 / 10), 160, 115});
				Config::PLAYER_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 2;
				Config::ALLY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 4;
				Config::ENEMY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 3;
				Config::ENEMY_LINK_RANGE = WindowData::SCREEN_WIDTH / 4;
			}
			// Slow down the game a bit
			SDL_Delay(1000 / WindowData::SCREEN_FPS);
		}
		else { // If the player is dead
			// Clear the window
			window.Clear((SDL_Color) {0, 0, 0, 255});
			// Imply some dark things about what the enemies do to you when they catch you
			if (Random::value() < 0.05) window.DrawBackground("Assets/Game.png"); // This can and WILL only render partially, but I do like it that way
			// Draw the game over text and the restart text
			std::string str = "Game Over";
			Text gameover(str, string("Assets/Fonts/VCR_OSD_MONO.ttf"), 100, (SDL_Color) {255, 0, 0, 255});
			str = "Press R to restart";
			Text restart(str, string("Assets/Fonts/VCR_OSD_MONO.ttf"), 30, (SDL_Color) {255, 255, 255, 255});
			gameover.setRect({WindowData::SCREEN_WIDTH / 2 - 300, WindowData::SCREEN_HEIGHT / 2 - 50, 600, 100});
			restart.setRect({WindowData::SCREEN_WIDTH / 2 - 100, WindowData::SCREEN_HEIGHT / 2 + 50, 200, 50});
			window.Draw(gameover);
			window.Draw(restart);
			// Draw everything on the window
			window.Flip();
			
			// Restart the game if R is pressed - it's basically the same as the startup code (because it is for the most part)
			const Uint8 * keystate = SDL_GetKeyboardState(nullptr);
			if (keystate[SDL_SCANCODE_R]){
				Window::setWindowSize(800, 600);
				window.changeWindowSize(WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
				
				Data::score = 0;
				Data::difficulty = 1;
				Data::sonar = 0;
				Data::isAlive = true;
				
				player.setPath("Assets/Boat.png");
				player.setRect({Random::randint((int) (WindowData::SCREEN_WIDTH * 21 / 30), (int) (WindowData::SCREEN_WIDTH * 0.9)),
				                Random::randint(WindowData::SCREEN_HEIGHT / 10, WindowData::SCREEN_HEIGHT * 9 / 10), 160, 115});
				player.setBoatPosition((SDL_Rect) {0, 0, 0, 0});
				player.setState(Player::EMBARKED);
				
				waves.clear();
				for (int i = 0; i < 20; i++) {
					waves.push_back(Wave(Random::randint(500, WindowData::SCREEN_WIDTH),
					                     (i % 10) * 160)); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
					waves[i].setPath("Assets/Wave.png");
				}
				
				trash.clear();
				for (int i = 10; i--;) {
					trash.push_back(Trash(Random::randint(WindowData::SCREEN_WIDTH / 3, (WindowData::SCREEN_WIDTH * 2) / 3 - 50),
					                      Random::randint(WindowData::SCREEN_HEIGHT - 50)));
				}
				
				enemies.clear();
				for (int i = Data::difficulty; i--;) {
					Enemy enemy;
					enemy.setPath("Assets/Enemy.png");
					enemy.setRect({Random::randint(WindowData::SCREEN_WIDTH / 12, WindowData::SCREEN_WIDTH / 4),
					               Random::randint(WindowData::SCREEN_HEIGHT / 12, WindowData::SCREEN_HEIGHT * 11 / 12), 60, 65});
					enemies.push_back(enemy);
				}
				
				score.setText(string("Score-O-Meter"));
				score.setFontPath(string("Assets/Fonts/VCR_OSD_MONO.ttf"));
				score.setFontSize(30);
				score.setColor((SDL_Color) {255, 255, 255, 255});
				score.setRect({10, 10, 200, 50});
				
				land.setColor((SDL_Color) {220, 220, 120, 255});
				land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
			} // Restart code
		}
	}
}