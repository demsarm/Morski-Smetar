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
	
	generateMainMenu();
	generateUsernameScreen();
	
	window.Raise();
	
	Input::Setup();
	
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
	
	// Need them names for score keeping, and I'm not going to force the player to input their name
	const std::vector<std::string> namesList = {"Anita Bath", "Al Beback", "Barry D'Alive", "Justin Thyme", "Sue Permann", "Dee Zaster", "Polly Ester", "Sal Monella", "Bill Board", "Barb D. Wyer", "Max Power", "Skel Etal", "Lou Scannon", "Pete Sariya", "Vlad"};
	username = namesList[Random::randint((int)(namesList.size()))]; // It should be fine to convert to string here, the list probably won't ever be longer than 2^31 - 1 (I mean how would you even get that many names?)
	
	// TODO the highscores file gets wiped for whatever reason GOD I HATE C++
	// Load the highscores
	File scores(absolutePath("highscores.txt"));
	std::string name = scores.readLine(); // I'd use do-while but our lord and savior Clang-Tidy says do-while is not good for whatever reason
	while (!name.empty()) {
		int space = (int)(name.find_last_of(' ')); // TODO: Remember to put character limit on username
		highscores.insert({name.substr(0, space), std::stoi(name.substr(space + 1))}); // By Jove, I hope stoi works like it should I have so many negative experiences with it
		name = scores.readLine();
	}
}

/**
 * @brief Increases difficulty and goes to next level
 */
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
		case Data::GameState::GAME_OVER:
			window.Draw(gameOverScreen);
			break;
		case Data::GameState::MAIN_MENU:
			window.Draw(mainMenuScreen);
			break;
		case Data::GameState::USERNAME:
			window.Draw(usernameScreen);
			break;
	}
	window.Flip();
}

/**
 * @brief Restarts the game
 */
void Game::Restart() {
	Window::setWindowSize(800, 600);
	window.changeWindowSize(WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT);
	
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
				Data::gameState = Data::GameState::GAME_OVER;
				// Create the game over screen - it's not done in Setup() because WindowData::SCREEN_WIDTH and WindowData::SCREEN_HEIGHT are changed during gameplay
				generateGameOverScreen();
				
				highscores.insert({username, Data::score}); // Since C++ file I/O is super slow (O(n)), I'll write to the file only when the game is closed (or maybe when the player exits to menu as well)
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
		backspaceCooldown = 20; // So it doesn't evaporate the entire string in 7.34 frames, but ten should still be low enough to not be annoying
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
 * @brief Writes the highscores to the file
 * @note The file is cleared before writing - use with caution
 */
void Game::writeHighscores() {
	File scores(absolutePath("highscores.txt"));
	scores.clearFile();
	auto it = highscores.begin();
	for (int i = 5; i-- && it != highscores.end(); it++) {
		scores.write(it->first + " " + std::to_string(it->second));
	}
}

/**
 * @brief This function generates the main menu
 * @warning The function does not clear the main menu screen, so it should be called only once
 */
void Game::generateMainMenu() {
	const int twelfth_x = WindowData::SCREEN_WIDTH / 12;
	const int twelfth_y = WindowData::SCREEN_HEIGHT / 12;
	
	
	SDL_Rect startRect = {twelfth_x, twelfth_y * 3, LETTER_RATIO(twelfth_y, 5), twelfth_y};
	SDL_Rect usernameRect = {twelfth_x, (int)(twelfth_y * 4.5), LETTER_RATIO(twelfth_y, 5), twelfth_y};
	SDL_Rect quitRect = {twelfth_x, (int)(twelfth_y * 6), LETTER_RATIO(twelfth_y, 4), twelfth_y};
	
	mainMenuScreen.setBackground("Assets/MainMenu.png");
	Text gameText("Morski Smetar", "Assets/Fonts/VCR_OSD_MONO.ttf", 100, (SDL_Color) {0xc0, 0xc0, 0xc0, 255}); // I came up with 0xc0 on the fly, can't be bothered to convert to decimal
	Text usernameText("Set username", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, (SDL_Color) {0xc0, 0xc0, 0xc0, 255});
	Text startText("Start", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, (SDL_Color) {0xc0, 0xc0, 0xc0, 255});
	Text quitText("Quit", "Assets/Fonts/VCR_OSD_MONO.ttf", 50, (SDL_Color) {0xc0, 0xc0, 0xc0, 255});
	
	gameText.setRect({twelfth_x, twelfth_y, LETTER_RATIO((int)(twelfth_y * 1.5), 13), (int)(twelfth_y * 1.5)});
	startText.setRect(startRect + (SDL_Rect){20, 5, -40, -10}); // Slightly smaller than the button size
	usernameText.setRect(usernameRect + (SDL_Rect){20, 5, -40, -10});
	quitText.setRect(quitRect + (SDL_Rect){20, 5, -40, -10});

	Button startButton(
			startRect,
			startText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[](){Data::gameState = Data::GameState::PLAYING;}
	);
	
	Button usernameButton(
			usernameRect,
			usernameText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[](){Data::gameState = Data::GameState::USERNAME;}
	);
	
	Button quitButton(
			quitRect,
			quitText,
			"Assets/Empty.png",
			"Assets/Empty.png",
			"Assets/Empty.png",
			[this](){open = false;}
	);
	
	mainMenuScreen.addText(gameText);
	mainMenuScreen.addButton(startButton);
	mainMenuScreen.addButton(usernameButton);
	mainMenuScreen.addButton(quitButton);
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
	                  [](){Data::gameState = Data::GameState::MAIN_MENU;}
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
	Text usernameText(username, "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0xc0, 0xc0, 0xc0, 255});
	usernameText.setRect(usernameRect);
	usernameScreen.addText(usernameText); // So it should be at index [0], this is important for the update function
	
	SDL_Rect submitRect = {WindowData::SCREEN_WIDTH / 2 - 70, WindowData::SCREEN_HEIGHT / 2 + 100, 140, 30};
	Text submitText("Submit", "Assets/Fonts/VCR_OSD_MONO.ttf", 30, (SDL_Color) {0xc0, 0xc0, 0xc0, 255});
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

// This comment is just a rant, it's not important nor is it relevant to the code
// In the Game::PlayingUpdate() method, I mentioned that we need more games that play around with actual windows
// I wanted to mention a game that does this really well
// It's called windowkill
// The concept is that the game is on multiple windows you can move around etc.
// I've yet to play it, but it looks really cool