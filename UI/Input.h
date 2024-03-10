#ifndef MORSKI_SMETAR_INPUT_H
#define MORSKI_SMETAR_INPUT_H

#include <map>
#include <string>
#include <vector>
#include "SDL2/SDL.h"

class Input {
private:
	static int mouseX, mouseY;
	static const Uint8 *prevKeyState;
	static const Uint8 *keyState;
	static Uint32 prevMouseState;
	static Uint32 mouseState;
	
	static std::map<std::string, SDL_Scancode> keyMap;
	static std::map<std::string, int> mouseMap;
	static std::vector<SDL_Event> events;
	// TODO add mouse input and other event handling
	// Don't wanna do it today I'm tired
	
	
public:
	void static Update();
	SDL_Scancode static mapKey(const std::string &key); // I'm tempted to make this private, but I'm not sure if I'll need it outside the class, so I'll just leave it public for now
	[[maybe_unused]] int static mapMouseButton(const std::string &button);
	
	Input() = delete; // This class is not meant to be instantiated I mean why would you even want to so say bye to the constructor it's the last time you'll ever see it
	~Input() = delete; // Can this even be called a destructor if the class is never instantiated? I mean it's there, but it's not doing anything, so I guess it's like a ghost or something
	
	bool static getKey(const std::string &key);
	bool static getKeyDown(const std::string &key);
	bool static getKeyUp(const std::string &key);
	
	bool static getMouseButton(const std::string& button);
	bool static getMouseButtonDown(const std::string& button);
	bool static getMouseButtonUp(const std::string& button);
	
	static std::pair<int, int> getMousePosition();
};

#endif //MORSKI_SMETAR_INPUT_H
