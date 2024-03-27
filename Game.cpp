#include <iostream>
#include "Game.h"
#include "UI/Button.h"
#include "Tools/File.h"

// I have no clue where this will be used (in the generate____Screen functions, but I'm not sure where else), so it goes here
#define LETTER_RATIO(h, len) (h * 2 / 3 * len)

using namespace std;



// When looking through this code, one may encounter horrors beyond mortal comprehension, in Update() in particular - I am not liable for any harm caused by reading this code
// Ye hast been warned

/**
 * Checks if the game is open
 * @return true if the game is open, false otherwise
 */
bool Game::isOpen() const {
	return open;
}

// This could be done in the constructor but whatever I don't care
/**
 * Initializes the game
 */
void Game::Setup() {
	// Window setup
	Window::init();
	Window::setWindowSize(800, 600);
	open = true;
	
	window.Raise();
	
	Input::Setup();
	
	recorder.open(absolutePath("recording.bin"));
	
	reader.open(absolutePath("recording.bin"));
	playback_objs.clear();
	
	saveManager.open(absolutePath("save.mssf"));
	
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
//	land.setColor((SDL_Color) {220, 220, 120, 255});
	land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
	land.setPath("Assets/Sand.png");
	
	// Need them names for score keeping, and I'm not going to force the player to input their name
	const std::vector<std::string> namesList = {"Anita Bath", "Al Beback", "Barry D'Alive", "Justin Thyme", "Sue Permann", "Dee Zaster", "Polly Ester", "Sal Monella", "Bill Board", "Barb D. Wyer", "Max Power", "Skel Etal", "Lou Scannon", "Pete Sariya", "Vlad"};
	username = namesList[Random::randint((int)(namesList.size()))]; // It should be fine to convert to string here, the list probably won't ever be longer than 2^31 - 1 (I mean how would you even get that many names?)
	
	// I am so tempted to give a 0.01% chance of the player name being "My beautiful baby, Independence day".
	// Though admittedly, that would break the leaderboard (making it take about forever to generate)
	// or about as long as it takes your computer to join 2^31-15 spaces together into a string (taking up around 4 gigs of RAM just for the string), colloquially referred to as "a long time"
	
	// Load the highscores
	File scores(absolutePath("highscores.txt"));
	std::string name = scores.readLine(); // I'd use do-while but our lord and savior Clang-Tidy says do-while is not good for whatever reason
	while (!name.empty()) {
		int space = (int)(name.find_first_of(' '));
		int score = std::stoi(name.substr(0, space));
		highscores.insert({score, name.substr(space + 1)}); // By Jove, I hope stoi works like it should I have so many negative experiences with it
		name = scores.readLine();
	}
	
	// These have to be made post highscore loading because the highscores are used in the leaderboard screen (and I don't want them scattered willy-nilly)
	generateMainMenu();
	generatePauseScreen();
	generateUsernameScreen();
	generateLeaderboardScreen();
}

/**
 * @brief Increases difficulty and goes to next level
 */
void Game::CompleteStage() {
	Data::score += 5 * Data::difficulty; // To give incentive to complete the stage instead of just picking up trash
	
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

/** Draws everything to the window
 */
void Game::Render(){
	switch (Data::gameState) {
		case Data::GameState::PLAYING:
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
					                  Data::friendly_sonar, 5, Friendly::getSonarColor(), 32);
					window.DrawCircle(f.getRect().x + f.getRect().w / 2,
					                  f.getRect().y + f.getRect().h / 2,
					                  Config::ALLY_SIGHT_RANGE, 5, Friendly::getSonarColor(), 32);
				}
			}
			
			++Data::friendly_sonar;
			if (Data::friendly_sonar > Config::ALLY_SIGHT_RANGE) {
				Data::friendly_sonar = 0;
			}
			
			// Draw the player and his 'sonar'
			window.DrawCircle(player.getRect().x + player.getRect().w / 2, player.getRect().y + player.getRect().h / 2,
			                  Config::PLAYER_SIGHT_RANGE, 5, player.getSonarColor(), 32);
			window.DrawCircle(player.getRect().x + player.getRect().w / 2, player.getRect().y + player.getRect().h / 2,
			                  Data::sonar, 5, player.getSonarColor(), 32);
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
			break;
		case Data::GameState::PAUSED:
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
					                  Data::friendly_sonar, 5, Friendly::getSonarColor(), 32);
					window.DrawCircle(f.getRect().x + f.getRect().w / 2,
					                  f.getRect().y + f.getRect().h / 2,
					                  Config::ALLY_SIGHT_RANGE, 5, Friendly::getSonarColor(), 32);
				}
			}
			
			++Data::friendly_sonar;
			if (Data::friendly_sonar > Config::ALLY_SIGHT_RANGE) {
				Data::friendly_sonar = 0;
			}
			
			// Draw the player and his 'sonar'
			window.DrawCircle(player.getRect().x + player.getRect().w / 2, player.getRect().y + player.getRect().h / 2,
			                  Config::PLAYER_SIGHT_RANGE, 5, player.getSonarColor(), 32);
			window.DrawCircle(player.getRect().x + player.getRect().w / 2, player.getRect().y + player.getRect().h / 2,
			                  Data::sonar, 5, player.getSonarColor(), 32);
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
			
			window.Draw(pauseScreen);
			break;
		case Data::GameState::PLAYBACK: // TODO Test this
			window.Clear((SDL_Color) {50, 210, 255, 255});
			
			for (const auto& wave : waves){
				window.Draw(wave);
			}
			
			window.Draw(land);

			for (auto& go : playback_objs){
				window.Draw(go);
			}
			
			break;
		case Data::GameState::GAME_OVER:
			window.Draw(gameOverScreen);
			break;
		case Data::GameState::MAIN_MENU:
			window.Draw(mainMenuScreen);
			break;
		case Data::GameState::USERNAME:
			window.Draw(usernameScreen);
			break;
		case Data::GameState::LEADERBOARD:
			window.Draw(leaderboardScreen);
			break;
		case Data::GameState::PLAYBACK_END:
			window.Draw(playbackEndScreen);
			break;
	}
	window.Flip();
}

/**
 * @brief Restarts the game
 */
void Game::Restart() {
	recorder.clearFile();
	Window::setWindowSize(800, 600);
	window.changeWindowSize(WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
	
	generatePauseScreen();
	
	Data::score = 0;
	Data::difficulty = 1;
	Data::sonar = 0;
	Data::gameState = Data::GameState::PLAYING;
	
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

/**
 * @brief Makes sure the window finds out if game should close
 */
void Game::CheckWindowEvents() {
	window.checkQuit();
	if (!window.isRunning()){
		open = false;
	}
}

/**
 * @brief Updates the game
 */
void Game::Update() {
	switch (Data::gameState) {
		case Data::GameState::PLAYING:
			PlayingUpdate();
			break;
		case Data::GameState::GAME_OVER:
			GameOverUpdate();
			break;
		case Data::GameState::MAIN_MENU:
			MainMenuUpdate();
			break;
		case Data::GameState::USERNAME:
			UsernameUpdate();
			break;
		case Data::GameState::LEADERBOARD:
			LeaderboardUpdate();
			break;
		case Data::GameState::PLAYBACK:
			PlaybackUpdate();
			break;
		case Data::GameState::PLAYBACK_END:
			PlaybackEndUpdate();
			break;
		case Data::GameState::PAUSED:
			PausedUpdate();
			break;
	}
}

/**
 * @brief Updates the main menu screen
 */
void Game::MainMenuUpdate() {
	mainMenuScreen.Update();
}

/**
 * @brief Updates the game during play
 */
void Game::PlayingUpdate() {
	lines.clear();
	warnings.clear();
	
	player.Update();
	player.GameObject::Update();
	
	for (auto &w: waves) {
		w.Update();
	}
	
	for (uint64_t i = trash.size(); i--;) {
		// Move the trash
		trash[i].Update();
		trash[i].GameObject::Update();
		
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
		enemies[i].GameObject::Update();
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
			
			if (other != enemies[i] && distance(player, enemies[i]) < Config::PLAYER_SIGHT_RANGE) {
				if (distance(player, other) < Config::PLAYER_SIGHT_RANGE) {
					lines.push_back(Line({enemies[i].getRect().x + enemies[i].getRect().w / 2,
					                      enemies[i].getRect().y + enemies[i].getRect().h / 2},
					                     {other.getRect().x + other.getRect().w / 2,
					                      other.getRect().y + other.getRect().h / 2},
					                     (SDL_Color) {255, 0, 0, 255}, 3));
				} else {
					/*
					std::pair <int, int> inter = line_circle_intersection(enemies[i].getRect().x + enemies[i].getRect().w / 2,
					                                                      enemies[i].getRect().y + enemies[i].getRect().h / 2,
					                                                      other.getRect().x + other.getRect().w / 2,
					                                                      other.getRect().y + other.getRect().h / 2,
					                                                      player.getRect().x + player.getRect().w / 2,
					                                                      player.getRect().y + player.getRect().h / 2,
					                                                       Config::PLAYER_SIGHT_RANGE);
					if (inter.first != -1 && inter.second != -1) {
						lines.push_back(Line({enemies[i].getRect().x + enemies[i].getRect().w / 2,
						                      enemies[i].getRect().y + enemies[i].getRect().h / 2},
						                     {inter.first, inter.second},
						                     (SDL_Color) {255, 0, 0, 255}, 3));
					}*/
				}
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
				saveManager.deleteSave();
				Data::gameState = Data::GameState::GAME_OVER;
				// Create the game over screen - it's not done in Setup() because WindowData::SCREEN_WIDTH and WindowData::SCREEN_HEIGHT are changed during gameplay
				generateGameOverScreen();
				
				highscores.insert({Data::score, username}); // Since C++ file I/O is super slow (O(n)), I'll write to the file only when the game is closed (or maybe when the player exits to menu as well)
				// Some math to make the text look not squished
				// Game Over - 9 letters
				// Press R to restart - 18 letters
				// @ font size 100, Game over is 600 px w (and a w:h ratio of letter of  67 : 100)
				// @ font size 100, Press R to restart would be 1200 px w (with same aspect ratio)
				// lower the font size to 30 - both w and h would fall to 30% - 360 * 30, I believe
				
				// Note to self in the future: If you plan to change the text in the future, make sure to keep the 2:3 ratio
				
				// Another note at a later date: Apparently the standard ratio for monospace fonts is 1:2, not 2:3
			}
		}
		
	}
	
	for (uint64_t i = friendlies.size(); i--;) { // Yes that is how I make longs (I may or may not be chronically deranged)
		// Move the friendlies
		friendlies[i].Update();
		friendlies[i].GameObject::Update();
		
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

	recorder.Update(player, enemies, friendlies, trash);

	if (Input::getKey("Escape") && !escapeCooldown) {
		Data::gameState = Data::GameState::PAUSED;
		escapeCooldown = 100;
	} else {
		escapeCooldown = escapeCooldown > 0 ? escapeCooldown - 1 : 0;
	}
	
	if (enemies.empty() && trash.empty()) { // && friendlies.empty() if you want to be evil and make the player commit several federal crimes and be arrested on 91 criminal charges in order to complete the stage
		CompleteStage();
		recorder.Write(Recorder::Type::COMPLETE_STAGE, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
	}
}

/**
 * @brief Updates the pause screen
 */
void Game::PausedUpdate() {
	pauseScreen.Update();
	if (Input::getKey("Escape") && !escapeCooldown) {
		Data::gameState = Data::GameState::PLAYING;
		escapeCooldown = 100;
	} else {
		escapeCooldown = escapeCooldown > 0 ? escapeCooldown - 1 : 0;
	}
}

/**
 * @brief Updates the game over screen
 */
void Game::GameOverUpdate() {
	gameOverScreen.Update();
}

/**
 * @brief Updates the username screen
 */
void Game::UsernameUpdate() {
	// I was going to have a class InputField, but it'd only be used once and oops it mysteriously disappeared when it didn't feel like working why would that be
	if (Input::getKey("Return")){ // I'd use getKeyUp, but I can't be bothered to copy the entirety of the keyboard state to another variable, so I'll just use getKey
		Data::gameState = Data::GameState::MAIN_MENU;
	}
	
	// Because I know the pain of having to delete text character by character
	if ((Input::getKey("Right Ctrl") || Input::getKey("Left Ctrl")) && Input::getKey("Backspace")){ // Don't need to check if the string is empty because the function does that for me
		username.clear();
	}
	
	backspaceCooldown = backspaceCooldown > 0 ? backspaceCooldown - 1 : 0;
	if (Input::getKey("Backspace") && !username.empty() && !backspaceCooldown){
		if (!username.empty()){
			username.pop_back();
		}
		backspaceCooldown = 20; // So it doesn't evaporate the entire string in 7.34 frames, but twenty should still be low enough to not be annoying
	}
	for (auto &event : Input::getEvents()){
		switch (event.type){
			case SDL_TEXTINPUT:
				username.size() < 20 ? username += event.text.text : username; // Can't be bothered to write an if statement
				break;
		}
	}
	Text& usernameText = (Text&)usernameScreen.getTexts()[0]; // I FOUND A USE FOR REFERENCES I NEVER THOUGHT I'D SEE THE DAY
	usernameText.setText(username);
	usernameText.setRect({WindowData::SCREEN_WIDTH / 2 - 200, WindowData::SCREEN_HEIGHT / 2 - 50, LETTER_RATIO(30, (int)username.size()), 30});
	usernameScreen.Update();
}

/**
 * @brief Updates the leaderboard screen
 */
void Game::LeaderboardUpdate() {
	leaderboardScreen.Update();
}

/**
 * @brief Updates the game during playback
 */
void Game::PlaybackUpdate() {
	reader.Scan();
	playback_objs.clear();
	bool exit = true;
	
	for (auto& wave : waves) {
		wave.Update();
	}
	
	while (exit) {
		Reader::Record record = reader.Next();
		GameObject buff;
		switch (record.type) {
			case Reader::Type::RESET:
				exit = false;
				break;
			case Reader::Type::PLAYER:
				if (record.x < WindowData::SCREEN_WIDTH / 3) {
					buff.setPath("Assets/Player.png");
					buff.setRect({(int)record.x, (int)record.y, 80, 115});
				} else {
					buff.setPath("Assets/Boat.png");
					buff.setRect({(int)record.x, (int)record.y, 160, 115});
				}
				break;
			case Reader::Type::ENEMY:
				buff.setPath("Assets/Enemy.png");
				buff.setRect({(int)record.x, (int)record.y, 60, 65});
				break;
			case Reader::Type::FRIENDLY_LAND:
				buff.setPath("Assets/Tractor.png");
				buff.setRect({(int)record.x, (int)record.y, 70, 60});
				break;
			case Reader::Type::FRIENDLY_WATER:
				buff.setPath("Assets/Fishing-Boat.png");
				buff.setRect({(int)record.x, (int)record.y, 70, 60});
				break;
			case Reader::Type::TRASH_BOTTLE:
				buff.setPath("Assets/Bottle.png");
				buff.setRect({(int)record.x, (int)record.y, 50, 50});
				break;
			case Reader::Type::TRASH_CAN:
				buff.setPath("Assets/Trash-Can.png");
				buff.setRect({(int)record.x, (int)record.y, 50, 50});
				break;
			case Reader::Type::TRASH_BOX:
				buff.setPath("Assets/Trash-Box.png");
				buff.setRect({(int)record.x, (int)record.y, 50, 50});
				break;
			case Reader::Type::COMPLETE_STAGE:
				Window::setWindowSize((int)record.x, (int)record.y); // I should arguably make the function take an uint to prevent negative window sizes, but eh who cares
				window.changeWindowSize(WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
				window.centerWindow();
				
				land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
				
				break;
			case Reader::Type::PLAYER_BOAT:
				buff.setPath("Assets/Boat.png");
				buff.setRect({(int)record.x, (int)record.y, 160, 115});
				break;
			default:
				cerr << "Oops, something went wrong (BAD_TYPE: " << (unsigned)record.type << ")\n" << endl;
				exit = false;
				break;
		}
		playback_objs.push_back(buff);
		
	}
	
	if (reader.eof()) {
		generatePlaybackEndScreen();
		Data::gameState = Data::GameState::PLAYBACK_END;
	}
	
	if (Input::getKey("Escape")){
		playback_objs.clear();
		Data::gameState = Data::GameState::MAIN_MENU;
	}
}

/**
 * @brief Updates the playback end screen
 */
void Game::PlaybackEndUpdate() {
	playbackEndScreen.Update();
}

/**
 * @brief Writes the highscores to the file
 * @note The file is cleared before writing - use with caution
 */
void Game::writeHighscores() {
	File scores(absolutePath("highscores.txt"));
	scores.clearFile();
	auto it = highscores.begin();
	for (int i = 5; i-- && it != highscores.end(); it++) {
		scores.write(std::to_string(it->first) + " " + it->second);
	}
}

/**
 * @brief This function generates the main menu
 * @warning The function does not clear the main menu screen, so it should be called only once
 */
void Game::generateMainMenu() {
	const int twelfth_x = WindowData::SCREEN_WIDTH / 12;
	const int twelfth_y = WindowData::SCREEN_HEIGHT / 12;
	
	mainMenuScreen.setBackground("Assets/MainMenu.png");
	
	SDL_Rect startRect = {twelfth_x, twelfth_y * 3, LETTER_RATIO(twelfth_y, 5), twelfth_y};
	SDL_Rect usernameRect = {twelfth_x, (int)(twelfth_y * 4.5), LETTER_RATIO(twelfth_y, 12), twelfth_y};
	SDL_Rect leaderboardRect = {twelfth_x, (int)(twelfth_y * 6), LETTER_RATIO(twelfth_y, 11), twelfth_y};
	SDL_Rect playbackRect = {twelfth_x, (int)(twelfth_y * 7.5), LETTER_RATIO(twelfth_y, 8), twelfth_y};
	SDL_Rect loadRect = {twelfth_x, (int)(twelfth_y * 9), LETTER_RATIO(twelfth_y, 9), twelfth_y};
	SDL_Rect quitRect = {twelfth_x, (int)(twelfth_y * 10.5), LETTER_RATIO(twelfth_y, 4), twelfth_y};
	
	Text gameText("Morski Smetar", "Assets/Fonts/VCR_OSD_MONO.ttf", 100, Config::TEXT_COLOR);
	Text startText("Start", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, Config::BUTTON_COLOR);
	Text usernameText("Set username", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, Config::BUTTON_COLOR);
	Text leaderboardText("Leaderboard", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, Config::BUTTON_COLOR);
	Text playbackText("Playback", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, Config::BUTTON_COLOR);
	Text loadText("Load save", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, Config::BUTTON_COLOR);
	Text quitText("Quit", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, Config::BUTTON_COLOR);
	
	gameText.setRect({twelfth_x, twelfth_y, LETTER_RATIO((int)(twelfth_y * 1.5), 13), (int)(twelfth_y * 1.5)});
	startText.setRect(startRect + (SDL_Rect){20, 5, -40, -10}); // Slightly smaller than the button size
	usernameText.setRect(usernameRect + (SDL_Rect){20, 5, -40, -10});
	leaderboardText.setRect(leaderboardRect + (SDL_Rect){20, 5, -40, -10});
	playbackText.setRect(playbackRect + (SDL_Rect){20, 5, -40, -10});
	loadText.setRect(loadRect + (SDL_Rect){20, 5, -40, -10});
	quitText.setRect(quitRect + (SDL_Rect){20, 5, -40, -10});

	Button startButton(
			startRect,
			startText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[this](){
				this->Restart();
				Data::gameState = Data::GameState::PLAYING;
			}
	);
	
	Button usernameButton(
			usernameRect,
			usernameText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[](){Data::gameState = Data::GameState::USERNAME;}
	);
	
	Button leaderboardButton(
			leaderboardRect,
			leaderboardText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[](){Data::gameState = Data::GameState::LEADERBOARD;}
	);
	
	Button playbackButton(
			playbackRect,
			playbackText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[](){Data::gameState = Data::GameState::PLAYBACK;}
	);
	
	Button loadButton(
			loadRect,
			loadText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[this](){
				if (this->saveManager.SaveExists()) {
					loadSave();
					Data::gameState = Data::GameState::PLAYING;
				}
			}
	);
	
	Button quitButton(
			quitRect,
			quitText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[this](){ open = false; }
	);
	
	mainMenuScreen.addText(gameText);
	mainMenuScreen.addButton(startButton);
	mainMenuScreen.addButton(usernameButton);
	mainMenuScreen.addButton(leaderboardButton);
	mainMenuScreen.addButton(playbackButton);
	mainMenuScreen.addButton(loadButton);
	mainMenuScreen.addButton(quitButton);
}

/**
 * @brief Generates the pause screen
 */
void Game::generatePauseScreen() {
	pauseScreen.setBackground("Assets/Gray50.png");
	
	pauseScreen.clearTexts();
	pauseScreen.clearButtons();
	
	SDL_Rect pauseRect = { 100, 100, LETTER_RATIO(100, 6), 100};
	SDL_Rect continueRect = {100, 250, LETTER_RATIO(30, 8), 30};
	SDL_Rect restartRect = {100, 300, LETTER_RATIO(30, 7), 30};
	SDL_Rect menuRect = {100, 350, LETTER_RATIO(30, 10), 30};
	
	Text pauseText("Paused", "Assets/Fonts/VCR_OSD_MONO.ttf", 100, Config::TEXT_COLOR);
	Text continueText("Continue", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, Config::BUTTON_COLOR);
	Text restartText("Restart", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, Config::BUTTON_COLOR);
	Text menuText("Main Menu", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, Config::BUTTON_COLOR);
	
	pauseText.setRect(pauseRect);
	continueText.setRect(continueRect);
	restartText.setRect(restartRect);
	menuText.setRect(menuRect);
	
	Button continueButton(
			continueRect,
			continueText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[](){ Data::gameState = Data::GameState::PLAYING; }
	);
	
	Button restartButton(
			restartRect,
			restartText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[this](){ this->Restart(); }
	);
	
	Button menuButton(
			menuRect,
			menuText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[this](){
				// int difficulty, int screenW, int screenH, int score, const Player &player, const std::vector<Trash> &trash, const std::vector<Enemy> &enemies, const std::vector<Friendly> &friendlies
				this->saveManager.WriteSave(Data::difficulty, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT, Data::score, player, trash, enemies, friendlies);
				saveManager.open(absolutePath("save.mssf"));
				Data::gameState = Data::GameState::MAIN_MENU;
				this->generateLeaderboardScreen();
			}
	);
	
	pauseScreen.addText(pauseText);
	pauseScreen.addButton(continueButton);
	pauseScreen.addButton(restartButton);
	pauseScreen.addButton(menuButton);
}

/**
 * @brief This function generates the game over screen
 * @warning The function does not clear the game over screen, so it should be called only once
 */
void Game::generateGameOverScreen() {
	gameOverScreen.setBackground("Assets/Game.png");
	Text gameOverText("Game Over", "Assets/Fonts/VCR_OSD_MONO.ttf", 100, (SDL_Color) {255, 0, 0, 255});
	Text scoreText("Score: " + std::to_string(Data::score), "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0xff, 0x40, 0x40, 255});
	int scoreLen = 18 * (int)scoreText.getText().length();
	gameOverText.setRect({WindowData::SCREEN_WIDTH / 2 - 300, WindowData::SCREEN_HEIGHT / 2 - 50, 600, 100});
	scoreText.setRect({WindowData::SCREEN_WIDTH / 2 - scoreLen/2, WindowData::SCREEN_HEIGHT / 2 + 50, scoreLen, 30}); // scoreLen has to be a variable because blah blah blah initializer list blah blah blah non-constant value blah blah blah
	gameOverScreen.clearTexts();
	gameOverScreen.clearButtons();
	gameOverScreen.addText(gameOverText);
	gameOverScreen.addText(scoreText);
	
	Text restartText("Restart", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0x88, 0x88, 0x88, 255});
	restartText.setRect({WindowData::SCREEN_WIDTH / 2 - 70, WindowData::SCREEN_HEIGHT / 2 + 100, 140, 30});
	Button restartButton((SDL_Rect){WindowData::SCREEN_WIDTH / 2 - 180, WindowData::SCREEN_HEIGHT / 2 + 100, 360, 30},
	                     restartText,
	                     "Assets/Empty.png",
	                     "Assets/Empty.png",
	                     "Assets/Empty.png",
	                     [this](){this->Restart();}
	);
	
	Text menuText("Main Menu", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0x88, 0x88, 0x88, 255});
	menuText.setRect({WindowData::SCREEN_WIDTH / 2 - 70, WindowData::SCREEN_HEIGHT / 2 + 150, 180, 30});
	Button menuButton((SDL_Rect){WindowData::SCREEN_WIDTH / 2 - 90, WindowData::SCREEN_HEIGHT / 2 + 150, 180, 30},
	                  menuText,
	                  "Assets/Empty.png",
	                  "Assets/Empty.png",
	                  "Assets/Empty.png",
	                  [this](){
						this->generateLeaderboardScreen(); // You could also do this on every death, but I find it's a little wasteful when you restart a lot
						Data::gameState = Data::GameState::MAIN_MENU;
						}
	);
	
	
	gameOverScreen.addButton(restartButton);
	gameOverScreen.addButton(menuButton);
}

/**
 * @brief This function generates the username screen
 * @warning The function does not clear the username screen, so it should be called only once
 */
void Game::generateUsernameScreen() {
	usernameScreen.setBackground("Assets/MainMenu.png");
	
	SDL_Rect usernameRect = {WindowData::SCREEN_WIDTH / 2 - 200, WindowData::SCREEN_HEIGHT / 2 - 50, LETTER_RATIO(30, (int)username.size()), 30};
	Text usernameText(username, "Assets/Fonts/VCR_OSD_MONO.ttf", 30, Config::TEXT_COLOR);
	usernameText.setRect(usernameRect);
	usernameScreen.addText(usernameText); // So it should be at index [0], this is important for the update function
	
	SDL_Rect submitRect = {WindowData::SCREEN_WIDTH / 2 - 70, WindowData::SCREEN_HEIGHT / 2 + 100, 140, 30};
	Text submitText("Submit", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, Config::BUTTON_COLOR);
	submitText.setRect(submitRect);
	Button submitButton(submitRect + (SDL_Rect){-20, -20, 40, 40},
	                    submitText,
						"Assets/Empty.png",
	                    "Assets/Empty.png",
	                    "Assets/Empty.png",
	                    [](){
	                      Data::gameState = Data::GameState::MAIN_MENU;
	                    }
	);
	
	usernameScreen.addButton(submitButton);
	
}

/**
 * @brief This function generates the leaderboard screen
 * @note Unlike the other generate functions, this function does clear the leaderboard screen
 */
void Game::generateLeaderboardScreen() {
	const int twelfth_x = WindowData::SCREEN_WIDTH / 12;
	const int twelfth_y = WindowData::SCREEN_HEIGHT / 12;
	
	leaderboardScreen.clearButtons();
	leaderboardScreen.clearTexts();
	
	leaderboardScreen.setBackground("Assets/MainMenu.png");
	std::string tmp = "Highest Scores";
	Text leaderboardText(tmp, "Assets/Fonts/VCR_OSD_MONO.ttf", 50, Config::TEXT_COLOR);
	SDL_Rect leaderboardRect = {twelfth_x, twelfth_y, LETTER_RATIO((int)(twelfth_y * 1.5), 13), (int)(twelfth_y * 1.5)};
	leaderboardText.setRect(leaderboardRect);
	leaderboardScreen.addText(leaderboardText);
	
	vector<pair<int, string>> top5;
	for (auto it = highscores.begin(); it != highscores.end() && top5.size() < 5; ++it) {
		top5.push_back(*it);
	}
	std::string temp;
	if (!top5.empty()) {
		temp = top5[0].second + (std::string(" ") * (20 - (int) top5[0].second.size())) +
		                   std::to_string(top5[0].first); // this is just to get the length of the string
	}
	SDL_Rect scoreRect = {twelfth_x, (int)(twelfth_y * 3), LETTER_RATIO(35, (int)temp.length()), 35};
	for (auto& score : top5) {
		temp = score.second + (std::string(" ") * (25 - (int)score.second.size())) + std::to_string(score.first);
		Text scoreText(temp, "Assets/Fonts/VCR_OSD_MONO.ttf", 30, Config::TEXT_COLOR);
		scoreText.setRect(scoreRect);
		leaderboardScreen.addText(scoreText);
		scoreRect.y += 75;
	}
	
	tmp = "Back";
	Text backText(tmp, "Assets/Fonts/VCR_OSD_MONO.ttf", 30, Config::BUTTON_COLOR);
	SDL_Rect backRect = {twelfth_x, WindowData::SCREEN_HEIGHT - 2 * twelfth_y, LETTER_RATIO(50, (int)tmp.size()), 50};
	backText.setRect(backRect);
	
	Button backButton(backRect + (SDL_Rect){-20, -20, 40, 40},
	                  backText,
	                  "Assets/Empty.png",
	                  "Assets/Empty.png",
	                  "Assets/Empty.png",
	                  [](){
	                      Data::gameState = Data::GameState::MAIN_MENU;
	                  }
	);
	leaderboardScreen.addButton(backButton);
}

/**
 * @brief Generates the screen seen at the end of playback
 * @note Unlike the other generate functions, this function does clear the playback end screen
 */
void Game::generatePlaybackEndScreen() {
	playbackEndScreen.clearButtons();
	playbackEndScreen.clearTexts();
	
	playbackEndScreen.setBackground("Assets/Game.png");
	
	SDL_Rect pbOverRect = {(WindowData::SCREEN_WIDTH - LETTER_RATIO(WindowData::SCREEN_WIDTH / 20, 16)) / 2, WindowData::SCREEN_HEIGHT / 2, LETTER_RATIO(WindowData::SCREEN_WIDTH / 16, 16), WindowData::SCREEN_WIDTH / 16};
	Text pbOverText("Playback is over", "Assets/Fonts/VCR_OSD_MONO.ttf", 100, (SDL_Color) {255, 0, 0, 255});
	pbOverText.setRect(pbOverRect);
	playbackEndScreen.addText(pbOverText);
	
	SDL_Rect restartRect = {WindowData::SCREEN_WIDTH / 2 - 160, WindowData::SCREEN_HEIGHT / 2 + 100, LETTER_RATIO(30, 16), 30};
	Text restartText("Restart playback" , "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0x88, 0x88, 0x88, 255});
	restartText.setRect(restartRect);
	Button restartButton(restartRect + (SDL_Rect){-20, -20, 40, 40},
	                     restartText,
	                     "Assets/Empty.png",
	                     "Assets/Empty.png",
	                     "Assets/Empty.png",
	                     [this](){
							reader.open(absolutePath("recording.bin"));
	                     	Data::gameState = Data::GameState::PLAYBACK;
	                     }
	);
	
	SDL_Rect mainMenuRect = {WindowData::SCREEN_WIDTH / 2 - 90, WindowData::SCREEN_HEIGHT / 2 + 150, LETTER_RATIO(30, 9), 30};
	Text mainMenuText("Main Menu", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0x88, 0x88, 0x88, 255});
	mainMenuText.setRect(mainMenuRect);
	Button mainMenuButton(mainMenuRect + (SDL_Rect){-20, -20, 40, 40},
	                     mainMenuText,
	                     "Assets/Empty.png",
	                     "Assets/Empty.png",
	                     "Assets/Empty.png",
	                     [this](){
							reader.open(absolutePath("recording.bin"));
	                     	Data::gameState = Data::GameState::MAIN_MENU;
	                     }
	);
	
	playbackEndScreen.addButton(restartButton);
	playbackEndScreen.addButton(mainMenuButton);
}

[[maybe_unused]]
const Player &Game::getPlayer() const {
	return player;
}

[[maybe_unused]]
std::vector<Trash> Game::getTrash() const {
	return trash;
}

[[maybe_unused]]
std::vector<Enemy> Game::getEnemies() const {
	return enemies;
}

[[maybe_unused]]
std::vector<Friendly> Game::getFriendlies() const {
	return friendlies;
}

void Game::loadSave() {
	enemies.clear();
	friendlies.clear();
	trash.clear();
	
	
	std::tuple<std::tuple<int, int, int>, int, Player, std::vector<Trash>, std::vector<Enemy>, std::vector<Friendly>> save = saveManager.ReadSave();
	
	std::tuple<int, int, int> gameState = std::get<0>(save);
	int score = std::get<1>(save);
	player = std::get<2>(save);
	trash = std::get<3>(save);
	enemies = std::get<4>(save);
	friendlies = std::get<5>(save);
	
	Data::score = score;
	int screenW = std::get<0>(gameState);
	int screenH = std::get<1>(gameState);
	int difficulty = std::get<2>(gameState);
	
	Window::setWindowSize(screenW, screenH);
	window.changeWindowSize(WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
	window.centerWindow();
	
	land.setRect({0, 0, WindowData::SCREEN_WIDTH / 3, WindowData::SCREEN_HEIGHT});
	land.setPath(absolutePath("Assets/Sand.png"));
	
	Data::difficulty = difficulty;
}



// This comment is just a rant, it's not important nor is it relevant to the code
// In the Game::PlayingUpdate() method, I mentioned that we need more games that play around with actual windows
// I wanted to mention a game that does this really well
// It's called windowkill
// The concept is that the game is on multiple windows you can move around etc.
// I've yet to play it, but it looks really cool