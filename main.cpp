//#define SDL_MAIN_HANDLED // Apparently needed for mingw to work


#include "Game.h"
#include "UI/Input.h"

#ifdef DEBUG
#include "Tools/filesystem.h"
#endif

// TODO Note to self: enable checking for fallthrough in Clang-Tidy when you figure out how

using namespace std;

#ifdef _WIN32
int WinMain() {
#else
int main() {
#endif
//	SDL_SetMainReady(); // If there are problems, try removing this line to see if anything's changed. It lets SDL functions know the initialization went well.

// So I know how to actually use the hash function
//	md5 hasher;
//	string data = "The quick brown fox jumps over the lazy dog", hash;
//	hasher.update(data.begin(), data.end());
//	hasher.hex_digest(hash);
//
//	cout << "Hash of " << data << " is " << hash << endl;
#ifdef DEBUG
	freopen(absolutePath("debug.txt").c_str(), "w", stdout);
	freopen(absolutePath("debug.txt").c_str(), "w", stderr);
#elif __linux__
	freopen("/dev/tty", "w", stdout);
	freopen("/dev/tty", "w", stderr);
#else
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
#endif

	Game game;
	game.Setup();
	// Game loop
	while (game.isOpen()) {
		Input::Update();

		game.Update(); // Player status is checked in here, no need for it to be in the main loop directly
		
		game.Render();
		game.CheckWindowEvents();
	}
	
	game.writeHighscores();
	return 0;
}