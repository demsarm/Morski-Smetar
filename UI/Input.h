#ifndef MORSKI_SMETAR_INPUT_H
#define MORSKI_SMETAR_INPUT_H

#include <map>
#include <string>
#include <vector>
#include "SDL2/SDL.h"

// TODO prevKeyState is always the same as keyState because pointers - copy to array or something if you feel like it

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
	
	
public:
	void static Update();
	void static Setup();
	SDL_Scancode static mapKey(const std::string &key); // I'm tempted to make this private, but I'm not sure if I'll need it outside the class, so I'll just leave it public for now
	[[maybe_unused]] int static mapMouseButton(const std::string &button);
	
	Input() = delete; // This class is not meant to be instantiated I mean why would you even want to so say bye to the constructor it's the last time you'll ever see it
	~Input() = delete; // Can this even be called a destructor if the class is never instantiated? I mean it's there, but it's not doing anything, so I guess it's like a ghost or something
	
	/**
	 * @brief Returns true if the key is pressed
	 * @param key The key to check, as a string; capitalize first letter
	 * @return True if the key is pressed
	 */
	[[maybe_unused]]
	bool static getKey(const std::string &key);
	/**
	 * @brief Returns true if the key was pressed in the last frame
	 * @param key The key to check, as a string; capitalize first letter
	 * @return True if the key was pressed
	 * @warning This function does not work as intended because pointers probably
	 */
	[[maybe_unused]]
	bool static getKeyDown(const std::string &key);
	/**
	 * @brief Returns true if the key was released in the last frame
	 * @param key The key to check, as a string; capitalize first letter
	 * @return True if the key was not pressed
	 * @warning This function does not work as intended because pointers probably
	 */
	[[maybe_unused]]
	bool static getKeyUp(const std::string &key);
	
	bool static getMouseButton(const std::string& button);
	[[maybe_unused]]
	bool static getMouseButtonDown(const std::string& button);
	bool static getMouseButtonUp(const std::string& button);
	
	static std::pair<int, int> getMousePosition();
	
	static std::vector<SDL_Event>& getEvents();
};

#endif //MORSKI_SMETAR_INPUT_H
