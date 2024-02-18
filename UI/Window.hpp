#ifndef SDL_GAME_WINDOW_HPP
#define SDL_GAME_WINDOW_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "iostream"
#include "string"
#include "thread"

#include "../Entities/GameObject.hpp"
#include "../Tools/Path.hpp"
#include "../Config/Config.hpp"
#include "../Entities/Text.hpp"
std::pair<int, int> getScreenSize();

class Window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	bool quit_handler_running;
	std::thread quit_handler;
	
public:
	static void init();
	static void setWindowSize(int percent);
	static void setWindowSize(int w, int h);
	static void setWindowSize(std::pair<int, int> size);
	
	Window(const char *title, int x, int y, int w, int h, Uint32 flags);
	~Window();
	
	void Flip();
	void Clear();
	void Clear(SDL_Color col);
	void Draw(SDL_Rect rect, SDL_Color);
	void Draw(const SDL_Rect& rect, const std::string& texture_path);
	void Draw(const GameObject& object, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void Draw(const Text& text);
	void DrawBackground(const std::string& texture_path);
	void Raise();
	
	[[nodiscard]] bool isRunning() const;
	void join_quit_handler();
	
	[[nodiscard]] SDL_Renderer *getRenderer() const;
	[[nodiscard]] SDL_Window *getWindow() const;
};

#endif //SDL_GAME_WINDOW_HPP