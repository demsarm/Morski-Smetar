#include "Game.hpp"

using namespace std;

// When looking through this code, one may encounter horrors beyond mortal comprehension, in Update() in particular - I am not liable for any harm caused by reading this code
// Ye hast been warned


bool Game::isOpen() const {
	return open;
}

void Game::Setup() {
	// Window setup
	Window::init();
	Window::setWindowSize(800, 600);
	
	open = true;
	
//	Window test("Test", 100, 100, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT, 0);
//	window = Window("Morski Smetar", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT, 0);
	
	// Player setup
	player.setPath("Assets/Boat.png");
	player.setRect({Random::randint((int) (WindowData::SCREEN_WIDTH * 21 / 30), (int) (WindowData::SCREEN_WIDTH * 0.9)),
	                Random::randint(WindowData::SCREEN_HEIGHT / 10, WindowData::SCREEN_HEIGHT * 9 / 10), 160, 115});
	player.setSonarColor((SDL_Color) {0, 255, 0, 255});
	// Make the enemies know about the player
	Enemy::setPlayer(&player);
	
	// Wave setup
	for (int i = 0; i < 20; i++) {
		waves.push_back(Wave(Random::randint(500, WindowData::SCREEN_WIDTH),
		                     (i % 10) * 160)); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
		waves[i].setPath("Assets/Wave.png"); // Constructors be damned (joke)
	}
	
	// Trash setup
	Enemy::setTrashes(&trash);
	for (int i = 10; i--;) {
		trash.push_back(Trash(Random::randint(WindowData::SCREEN_WIDTH / 3, (WindowData::SCREEN_WIDTH * 2) / 3 - 50),
		                      Random::randint(WindowData::SCREEN_HEIGHT - 50)));
	}
	
	Enemy::setEnemies(&enemies);
	for (int i = Data::difficulty; i--;) {
		Enemy enemy;
		enemy.setPath("Assets/Enemy.png");
		enemy.setRect({Random::randint(WindowData::SCREEN_WIDTH / 12, WindowData::SCREEN_WIDTH / 4),
		               Random::randint(WindowData::SCREEN_HEIGHT / 12, WindowData::SCREEN_HEIGHT * 11 / 12), 60, 65});
		enemies.push_back(enemy);
	}
	
	// Create friendlies
	for (int i = Data::difficulty; i--;) {
		friendlies.push_back(Friendly());
	}
	// Consistent number bases are for the weak and the uninitiated (Disclaimer: I am weak, however I may or may not be initiated - I don't know what the exact definition of initiated is)
	Friendly::setSonarColor((SDL_Color) {0b0, 0x88, 00, 255});
	
	// Let there be land
	// TODO: Make the land look nicer, be it with textures or something like the waves on the water
	land.setColor((SDL_Color) {220, 220, 120, 255});
	land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
	
	
}

void Game::Update() {
	lines.clear();
	warnings.clear();
	
	player.Update();
	
	for (auto &w: waves) {
		w.Update();
	}
	
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
	}
	
	for (uint64_t i = enemies.size(); i--;) {
		// Make the enemies take action
		enemies[i].Update();
		// Telegraphing
		for (auto &other: enemies) {
			// Warn the player if two enemies are colliding
			if (isColliding(enemies[i], other) && enemies[i] != other && distance(enemies[i], player) < (double) Config::PLAYER_SIGHT_RANGE) {
				GameObject warn;
				warn.setPath("Assets/Exclamation.png");
				warn.setRect({enemies[i].getRect().x + enemies[i].getRect().w / 2 - 20,
				              enemies[i].getRect().y - 60, 30, 55});
				warnings.push_back(warn); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
			}
			
			// Draw a line to enemies in range
			if (other != enemies[i] && distance(player, enemies[i]) < (double) Config::PLAYER_SIGHT_RANGE) {
				if (distance(enemies[i], other) < (double) Config::PLAYER_SIGHT_RANGE) {
					Line l(enemies[i].getRect().x + enemies[i].getRect().w / 2,
					       enemies[i].getRect().y + enemies[i].getRect().h / 2,
					       other.getRect().x + other.getRect().w / 2,
					       other.getRect().y + other.getRect().h / 2,
						   (SDL_Color) {255, 0, 0, 255},
						   3
					);
					lines.push_back(l); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
				}
				// TODO fix this - the new version of line_circle_intersection is better (hopefully)
				/*
				else {
					std::pair<int, int> inter = line_circle_intersection(enemies[i].getRect().x + enemies[i].getRect().w / 2,
					                                                    enemies[i].getRect().y + enemies[i].getRect().h / 2,
					                                                    other.getRect().x + other.getRect().w / 2,
					                                                    other.getRect().y + other.getRect().h / 2,
					                                                    player.getRect().x + player.getRect().w / 2,
					                                                    player.getRect().y + player.getRect().h / 2,
					                                                    Config::PLAYER_SIGHT_RANGE);
					if (inter.first != -1 && inter.second != -1) {
						Line l(
								{enemies[i].getRect().x + enemies[i].getRect().w / 2,
								 enemies[i].getRect().y + enemies[i].getRect().h / 2},
								inter,
								(SDL_Color) {255, 0, 0, 255},
								3
						);
						lines.push_back(l); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
					}
				}*/
			}
		}
		
		// Kill either the player or the enemy if they collide (rules in the README)
		if (isColliding(player, enemies[i]) && player.getState() == Player::PlayerState::DISEMBARKED){
			bool playerLoss = false;
			for (auto &other: enemies) {
				if (other != enemies[i] && sqrt(
						pow(other.getRect().x - enemies[i].getRect().x, 2) +
						pow(other.getRect().y - enemies[i].getRect().y, 2)) <
				                           (double) Config::ENEMY_LINK_RANGE) {
					playerLoss = true;
					break;
				}
			}
			if (!playerLoss) {
				enemies.erase(enemies.begin() +
				              (int64_t) i); // Yes that is how I cast to long I may or may not be chronically deranged
				Data::score += 2;
			} else {
				Data::isAlive = false;
				// Create the game over screen - it's not done in Setup() because WindowData::SCREEN_WIDTH and WindowData::SCREEN_HEIGHT are changed during gameplay
				gameOverScreen.setBackground("Assets/Game.png");
				Text gameOverText("Game Over", "Assets/Fonts/VCR_OSD_MONO.ttf", 100, (SDL_Color) {255, 0, 0, 255});
				Text restartText("Press R to restart", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0x88, 0x88, 0x88, 255});
				gameOverText.setRect({WindowData::SCREEN_WIDTH / 2 - 300, WindowData::SCREEN_HEIGHT / 2 - 50, 600, 100});
				restartText.setRect({WindowData::SCREEN_WIDTH / 2 - 180, WindowData::SCREEN_HEIGHT / 2 + 50, 360, 30});
				gameOverScreen.clearTexts();
				gameOverScreen.addText(gameOverText);
				gameOverScreen.addText(restartText);
				// Some math to make the text look not squished
				// Game Over - 9 letters
				// Press R to restart - 18 letters
				// @ font size 100, Game over is 600 px w (and a w:h ratio of letter of  67 : 100)
				// @ font size 100, Press R to restart would be 1200 px w (with same aspect ratio)
				// lower the font size to 30 - both w and h would fall to 30% - 360 * 30, I believe
			
				// Note to self in the future: If you plan to change the text in the future, make sure to keep the 1:3 ratio
			}
		}
		
	}
	
	for (uint64_t i = friendlies.size(); i--;) { // Yes that is how I make longs (I may or may not be chronically deranged)
		// Move the friendlies
		friendlies[i].Update();
		
		// Kill the friendly if the player collides with it
		if (isColliding(player, friendlies[i])) {
			friendlies.erase(friendlies.begin() +
			                 (int64_t) i); // Yes that is how I cast to long I may or may not be chronically deranged
			Data::score -= 2;
		}
	}
	
	texts.clear();
	
	// Disembark from the boat (text)
	if (isColliding(player.getRect(), (SDL_Rect) {
			WindowData::SCREEN_WIDTH / 3, 0,
			WindowData::SCREEN_WIDTH / 6,
			WindowData::SCREEN_HEIGHT})) {
		Text embark(string("Press E to disembark"),
					string("Assets/Fonts/VCR_OSD_MONO.ttf"),
					30, (SDL_Color) {255, 255, 255, 255});
		embark.setRect({WindowData::SCREEN_WIDTH / 2 - 400,
		                WindowData::SCREEN_HEIGHT - 100,
		               700, 50});
		texts.push_back(embark); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
		
		delete disembark_indicator; // delete automatically checks if the pointer is nullptr - no worries there (I think - ChatGPT says it's been like that since C++98)
		GameObject disembark_loc;
		disembark_loc.setPath("Assets/Disembark-Indicator.png");
		disembark_loc.setRect(
				{WindowData::SCREEN_WIDTH / 3 - (player.getRect().x - WindowData::SCREEN_WIDTH / 3 + 80),
				 player.getRect().y, 80, 115});
		disembark_indicator = new GameObject(disembark_loc);
//
	} else {
		if (disembark_indicator != nullptr) {
			delete disembark_indicator;
			disembark_indicator = nullptr;
		}
	}
	
	// Embark on the boat (text)
	if (isColliding(player.getRect() + (SDL_Rect) {player.getRect().w, 0, 0, 0}, {
			player.getBoatPosition().x - WindowData::SCREEN_WIDTH / 8,
			player.getBoatPosition().y - 50,
			player.getBoatPosition().w + WindowData::SCREEN_WIDTH / 4,
			player.getBoatPosition().h + 100})) {
		std::string str = "Press E to embark";
		Text embark(str, string("Assets/Fonts/VCR_OSD_MONO.ttf"), 30, (SDL_Color) {255, 255, 255, 255});
		embark.setRect({WindowData::SCREEN_WIDTH / 2 - 400, WindowData::SCREEN_HEIGHT - 100,
		                595, 50});
		texts.push_back(embark); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
	}

	// Write the score
	Text score(string("Score: ") + std::to_string(Data::score),
	           string("Assets/Fonts/VCR_OSD_MONO.ttf"),
	           30, (SDL_Color) {255, 255, 255, 255});
	score.setRect({10, 10, 200, 50});
	texts.push_back(score); // Clang-Tidy: Use emplace_back instead of push_back (iont feel like it)
	
	if (enemies.empty() && trash.empty()) { // && friendlies.empty() if you want to be evil and make the player commit several federal crimes and be arrested on 91 criminal charges in order to complete the stage
		CompleteStage();
	}
	
	
}

void Game::CompleteStage() {
	++Data::difficulty;
	
	enemies.clear(); // This may be kind of redundant because why would you call this if enemies isn't empty
	for (int i = Data::difficulty; i--;) {
		Enemy enemy;
		enemy.setPath("Assets/Enemy.png");
		enemy.setRect({Random::randint(WindowData::SCREEN_WIDTH / 12, WindowData::SCREEN_WIDTH / 4),
		               Random::randint(WindowData::SCREEN_HEIGHT / 12, WindowData::SCREEN_HEIGHT * 11 / 12),
		               60,
		               65});
		// This is done to prevent enemies from spawning on top of each other - it's not perfect, but it's better than nothing
		// I didn't do this in setup and I should have, but there's only one enemy anyway, so this can't happen
		for (auto &other: enemies) {
			if (sqrt(
					pow(other.getRect().x - enemy.getRect().x, 2) +
					pow(other.getRect().y - enemy.getRect().y, 2)) < (double) Config::ENEMY_LINK_RANGE) {
				enemy.setAxis(other.getAxis() == Enemy::Axis::X ? Enemy::Axis::Y : Enemy::Axis::X);
			}
		}
		enemies.push_back(enemy);
	}
	
	trash.clear(); // Also redundant - see enemy respawning above
	for (int i = 10; i--;) {
		trash.push_back(
				Trash(Random::randint(WindowData::SCREEN_WIDTH / 3,
				                      (WindowData::SCREEN_WIDTH * 2) / 3 - 50),
				      Random::randint(WindowData::SCREEN_HEIGHT - 50)));
	}
	// This isn't redundant - the stage can be completed without all friendlies being destroyed brutally by the player
	// Though it would be funny if there were just more and more friendlies until you lose a bajillion points because you can't help but run over the lot of 'em
	friendlies.clear();
	for (int i = Data::difficulty; i--;) {
		friendlies.push_back(Friendly());
	}
	
	// Change the window size if able to
	// Since the player speed is fixed, changing the window size makes the game harder
	// Yes it's goofy and yes, I love it. We need more games that play around with actual windows
	if (800 + 100 * Data::difficulty < window.getScreenSize().first) {
		std::pair<int, int> pos = window.getWindowsPosition();
		pos.first -= 50;
//		window.setWindowsPosition(pos.first, pos.second); Unnecessary due to the window.centerWindow() call a little later
	}
	if (600 + 100 * Data::difficulty < window.getScreenSize().second) {
		std::pair<int, int> pos = window.getWindowsPosition();
		pos.second -= 50;
//		window.setWindowsPosition(pos.first, pos.second); Unnecessary due to the window.centerWindow() call a little later
	}
	
	Window::setWindowSize(
			800 + 100 * Data::difficulty < window.getScreenSize().first ? 800 + 100 * Data::difficulty : window.getScreenSize().first,
			600 + 100 * Data::difficulty < window.getScreenSize().second ? 600 + 100 * Data::difficulty : window.getScreenSize().second);
	// This just changes WindowData::SCREEN_WIDTH and WindowData::SCREEN_HEIGHT
	
	// Resize and move the window
	window.changeWindowSize(WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
	window.centerWindow();
	
	// Change a bunch of variables that are dependent on the window size
	Config::PLAYER_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 2;
	Config::ALLY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 4;
	Config::ENEMY_SIGHT_RANGE = WindowData::SCREEN_WIDTH / 3;
	Config::ENEMY_LINK_RANGE = WindowData::SCREEN_WIDTH / 4;
	land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
	
	// Move the player to the sea
	player.setBoatPosition({0, 0, 0, 0});
	player.setState(Player::EMBARKED);
	player.setPath("Assets/Boat.png");
	player.setRect({Random::randint((int) (WindowData::SCREEN_WIDTH * 21 / 30), (int) (WindowData::SCREEN_WIDTH * 0.9)),
	                Random::randint(WindowData::SCREEN_HEIGHT / 10, WindowData::SCREEN_HEIGHT * 9 / 10), 160, 115});
	
}

void Game::Render(){
	if(Data::isAlive) {
		// Clear the window
		window.Clear((SDL_Color) {50, 210, 255, 255});
		
		// Draw the waves
		for (auto &wave: waves) {
			window.Draw(wave);
		}
		
		// Let there be land (again)
		window.Draw(land);
		
		// Draw the trash
		for (auto &t: trash) {
			window.Draw(t);
		}
		
		// Draw the enemies
		for (auto &e: enemies) {
			bool draw = distance(player, e) < Config::PLAYER_SIGHT_RANGE;
			for (auto &f: friendlies) {
				if (distance(f, e) < Config::ALLY_SIGHT_RANGE &&
				    f.getType() == Friendly::Type::LAND) { // Make "sight sharing" exclusive to land friendlies
					draw = true;
				}
				if (draw) {
					// Would be put in the if statement above, but that would
					//  1. make the code less readable and
					//  2. result in the code going through the whole loop (in the worst case) even if the enemy is in sight of the player
					break;
				}
			}
			if (draw) {
				window.Draw(e);
			}
		}
		
		// Draw the friendlies
		for (auto &f: friendlies) {
			window.Draw(f, 0.0,
			            f.getDirection() == Friendly::Direction::NEGATIVE ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
			if (f.getType() == Friendly::Type::LAND) {
				window.DrawCircle(f.getRect().x + f.getRect().w / 2,
				                  f.getRect().y + f.getRect().h / 2,
				                  Data::friendly_sonar, 10, Friendly::getSonarColor(), 32);
				window.DrawCircle(f.getRect().x + f.getRect().w / 2,
				                  f.getRect().y + f.getRect().h / 2,
				                  Config::ALLY_SIGHT_RANGE, 10, Friendly::getSonarColor(), 32);
			}
		}
		
		++Data::friendly_sonar;
		if (Data::friendly_sonar > Config::ALLY_SIGHT_RANGE) {
			Data::friendly_sonar = 0;
		}
		
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
		for (auto &t: texts) {
			window.Draw(t);
		}
		
		for (auto &l: lines) {
			window.DrawLine(l);
		}
		
		for (auto &w: warnings) {
			window.Draw(w);
		}
		if (disembark_indicator != nullptr) {
			window.Draw(*disembark_indicator, 0.0, player.isFacingRight() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
		}
	} else {
		window.Draw(gameOverScreen);
	}
	window.Flip();
}

void Game::Restart() {
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
	
	land.setColor((SDL_Color) {220, 220, 120, 255});
	land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
}

void Game::CheckWindowEvents() {
	if (!window.isRunning()){
		open = false;
	}
}


// This comment is just a rant, it's not important nor is it relevant to the code
// In the Game::Update() method, I mentioned that we need more games that play around with actual windows
// I wanted to mention a game that does this really well
// It's called windowkill
// The concept is that the game is on multiple windows you can move around etc.
// I've yet to play it, but it looks really cool