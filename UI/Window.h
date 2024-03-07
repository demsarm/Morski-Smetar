#ifndef SDL_GAME_WINDOW_HPP
#define SDL_GAME_WINDOW_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "iostream"
#include "string"
#include "thread"

#include "../Entities/GameObject.h"
#include "../Tools/filesystem.h"
#include "../Config/Config.h"
#include "Text.h"
#include "Line.h"
#include "Screen.h"

[[maybe_unused]] std::pair<int, int> getScreenSize();

class Window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	bool running;
public:
	static void init();
	
	[[maybe_unused]] static void setWindowSize(int percent);
	static void setWindowSize(int w, int h);
	[[maybe_unused]] static void setWindowSize(std::pair<int, int> size);
	void changeWindowSize(int w, int h);
	
	std::pair<int, int> getWindowsPosition();
	void setWindowsPosition(int x, int y);
	
	Window();
	Window(const Window& other);
	Window(const char *title, int x, int y, int w, int h, Uint32 flags);
	~Window();
	
	void Flip();
	[[maybe_unused]] void Clear();
	void Clear(SDL_Color col);
	void Draw(SDL_Rect rect, SDL_Color);
	[[maybe_unused]] void Draw(const SDL_Rect& rect, const std::string& texture_path);
	void Draw(const GameObject& object, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void Draw(const Text& text);
	void Draw(const Screen& screen);
	void DrawLine(int x1, int y1, int x2, int y2, int width, SDL_Color color);
	void DrawLine(const Line& line);
	
	void DrawCircle(int x, int y, int r, int w, SDL_Color color, int accuracy = 32);
	[[maybe_unused]] void DrawBackground(const std::string& texture_path);
	[[maybe_unused]] void Raise();
	
	std::pair<int, int> getScreenSize();
	
	[[nodiscard]] bool isRunning() const;
	
	Window& operator=(const Window& other);
	
	[[nodiscard]] [[maybe_unused]] SDL_Renderer *getRenderer() const;
	[[nodiscard]] [[maybe_unused]] SDL_Window *getWindow() const;
	
	void centerWindow();
	void checkQuit();
};

#endif //SDL_GAME_WINDOW_HPP