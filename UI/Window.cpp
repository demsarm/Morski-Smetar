#include "Window.h"

#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // Because x86_64-w64-mingw32-g++ doesn't know what M_PI is apparently

/**
 * @brief gets the screen size
 * @return The screen size
 * @warning Doesn't work
 */
std::pair<int, int> getScreenSize() {
	SDL_Window *window = SDL_CreateWindow(
			"You're not supposed to see this why can you see it something went horribly wrong I just want to know your resolution so I can base the actual window's size off of that please help me I'm stuck in a computer and I can't get out",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			100,
			100,
			SDL_WINDOW_HIDDEN
	);
	int displayIndex = SDL_GetWindowDisplayIndex(window);
	
	SDL_DisplayMode displayMode;
	if (!SDL_GetCurrentDisplayMode(displayIndex, &displayMode)){
		return {displayMode.w, displayMode.h};
	}
	SDL_DestroyWindow(window);
	return {0, 0};
}

/**
 * @brief Creates a window
 * @param title The title of the window
 * @param x The x position of the window
 * @param y The y position of the window
 * @param w The width of the window
 * @param h The height of the window
 * @param flags The flags of the window
 */
Window::Window(const char *title, int x, int y, int w, int h, Uint32 flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	window = SDL_CreateWindow(
			title,
			x,
			y,
			w,
			h,
			flags
	);
	
	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
			// | SDL_RENDERER_PRESENTVSYNC // Vsync seems to break the renderer initialization, hence it is commented out
	);
	
	running = true;
}

/**
 * @brief Destructor for the Window class
 * Joins the quit handler thread
 */
Window::~Window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
//	SDL_Quit();
}

/**
 * @brief Draws a rectangle to the screen
 * @param rect The rectangle to draw
 * @param color The color to draw the rectangle
 */
void Window::Draw(SDL_Rect rect, SDL_Color color){
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}

/**
 * @brief Draws a texture to the screen
 * @param rect The rectangle to draw the texture to
 * @param texture_path The path to the texture
 */
void Window::Draw(const SDL_Rect& rect, const std::string& texture_path){
	SDL_Rect drawn = rect;
	SDL_Surface *surface = IMG_Load(texture_path.c_str());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, nullptr, &drawn);
	SDL_DestroyTexture(texture);
}

/**
 * @brief Draws a GameObject to the screen
 * @param object The GameObject to draw
 */
void Window::Draw(const GameObject& object, double angle, SDL_RendererFlip flip){
	SDL_Rect drawn = object.getRect();
	if (object.getDesign().path.empty()){
		Draw(drawn, object.getDesign().color);
		return;
	} else {
		SDL_Surface *surface = IMG_Load(absolutePath(object.getDesign().path).c_str());
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		SDL_RenderCopyEx(renderer, texture, nullptr, &drawn, angle, nullptr, flip);
		SDL_DestroyTexture(texture);
	}
}

/**
 * @brief Updates the window
 */
void Window::Flip(){
	SDL_RenderPresent(renderer);
}

/**
 * @brief Clears the window
 */
void Window::Clear(){
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 255);
	SDL_RenderClear(renderer);
}

/**
 * @brief Clears the window
 * @param col The color to clear the window with
 */
void Window::Clear(SDL_Color col){
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
	SDL_RenderClear(renderer);
}

/**
 * @brief Returns whether the window is running
 * @return Whether the window is running
 */
bool Window::isRunning() const{
	return running;
}

/**
 * @brief Returns the renderer
 * @warning This should only be used if you know what you are doing and why you are using it
 * @return The renderer
 */
SDL_Renderer *Window::getRenderer() const{
	return renderer;
}

/**
 * @brief Returns the window
 * @warning This should only be used if you know what you are doing and why you are using it
 * @return The window
 */
SDL_Window *Window::getWindow() const{
	return window;
}

/**
 * @brief Raises the window
 */
void Window::Raise(){
	SDL_RaiseWindow(window);
}

/**
 * @brief Initializes SDL
 */
void Window::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	
	if (TTF_Init() != 0) {
		std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
		exit(1);
	}
}

/**
 * @brief Sets the window size
 * @param percent The percent of the screen size to set the window size to
*/
 void Window::setWindowSize(int percent) {
	SDL_DisplayMode displayMode;
	if (!SDL_GetCurrentDisplayMode(0, &displayMode)){
		WindowData::SCREEN_WIDTH = displayMode.w * percent / 100;
		WindowData::SCREEN_HEIGHT = displayMode.h * percent / 100;
	}
}

/**
 * @brief Sets the window size
 * @param w The width of the window
 * @param h The height of the window
 */
void Window::setWindowSize(int w, int h) {
	WindowData::SCREEN_WIDTH = w;
	WindowData::SCREEN_HEIGHT = h;
}

/**
 * @brief Sets the window size
 * @param size the size of the window
 */
void Window::setWindowSize(std::pair<int, int> size) {
	WindowData::SCREEN_WIDTH = size.first;
	WindowData::SCREEN_HEIGHT = size.second;
}

/**
 * @brief Draws a texture to the background
 * @param texture_path The path to the texture
 */
void Window::DrawBackground(const std::string &texture_path) {
	SDL_Texture * texture = IMG_LoadTexture(renderer, absolutePath(texture_path).c_str());
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_DestroyTexture(texture);
	// Make it less goofy when aspect ratios don't match if you ever feel like it
}

/**
 * @brief Draws a Text to the screen
 * @param text The Text to draw
 * @note Already uses absolutePath to get the absolute path of the font
 */
void Window::Draw(const Text &text) {
	SDL_Rect drawn = text.getRect();
	TTF_Font *font = TTF_OpenFont(absolutePath(text.getFontPath()).c_str(), text.getFontSize());
	SDL_Surface *surface = TTF_RenderText_Solid(font, text.getText().c_str(), text.getColor());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, nullptr, &drawn);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

/**
 * @brief Draws a line to the screen
 * @param x1 The x position of the first point
 * @param y1 The y position of the first point
 * @param x2 The x position of the second point
 * @param y2 The y position of the second point
 * @param width The width of the line
 * @param color The color of the line
 */
void Window::DrawLine(int x1, int y1, int x2, int y2, int width, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int i = width/2; i > -width/2; --i){
		SDL_RenderDrawLine(renderer, x1 + i, y1, x2 + i, y2);
	}
	
}

/**
 * @brief Draws a circle to the screen
 * @param x The x position of the circle
 * @param y The y position of the circle
 * @param r The radius of the circle
 * @param w The width of the circle
 * @param color The color of the circle
 * @param accuracy The accuracy of the circle
 * @note This does NOT actually draw a circle, but rather a polygon - drawing a circle tends to be very slow and when accuracy is high enough, it really doesn't matter
 */
void Window::DrawCircle(int x, int y, int r, int w, SDL_Color color, int accuracy) {
	int n = 32;
	for (int i = 0; i < n; i++){
		DrawLine((int)(r * sin(2 * M_PI * i / accuracy) + x),
				(int)(r * cos(2 * M_PI * i / accuracy) + y),
				(int)(r * sin(2 * M_PI * (i + 1) / accuracy) + x),
				(int)(r * cos(2 * M_PI * (i + 1) / accuracy) + y),
		                w, color);
		
	}
}

/**
 * @brief Changes the window size
 * @param w The width of the window
 * @param h The height of the window
 */
void Window::changeWindowSize(int w, int h) {
	SDL_SetWindowSize(window, w, h);
}

/**
 * @brief Returns the position of the window
 * @return The position of the window
 */
std::pair<int, int> Window::getWindowsPosition() {
	int x, y;
	SDL_GetWindowPosition(window, &x, &y);
	return {x, y};
}

/**
 * @brief Sets the position of the window
 * @param x The x position of the window
 * @param y The y position of the window
 */
void Window::setWindowsPosition(int x, int y) {
	SDL_SetWindowPosition(window, x, y);
}

/**
 * @brief Centers the window
 */
void Window::centerWindow() {
	SDL_DisplayMode displayMode;
	int windowDisplayIndex = SDL_GetWindowDisplayIndex(window);
	if (!SDL_GetCurrentDisplayMode(windowDisplayIndex, &displayMode)){
		SDL_SetWindowPosition(window, displayMode.w / 2 - WindowData::SCREEN_WIDTH / 2, displayMode.h / 2 - WindowData::SCREEN_HEIGHT / 2);
	}
}

/**
 * @brief Returns the screen size
 * @return The screen size
 */
std::pair<int, int> Window::getScreenSize() {
	SDL_DisplayMode displayMode;
	int windowDisplayIndex = SDL_GetWindowDisplayIndex(window);
	if (!SDL_GetCurrentDisplayMode(windowDisplayIndex, &displayMode)){
		return {displayMode.w, displayMode.h};
	} else return { 800, 600 };
}

Window::Window(const Window &other) {
	int x, y;
	SDL_GetWindowPosition(other.window, &x, &y);
	int w, h;
	SDL_GetWindowSize(other.window, &w, &h);
	window = SDL_CreateWindow(
			SDL_GetWindowTitle(other.window),
			x,
			y,
			w,
			h,
			SDL_GetWindowFlags(other.window)
	);
	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
			// | SDL_RENDERER_PRESENTVSYNC // Vsync seems to break the renderer initialization, hence it is commented out
	);
	running = true;
}

Window &Window::operator=(const Window &other) {
	if (this == &other){
		return *this;
	}
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	
	
	int x, y;
	SDL_GetWindowPosition(other.window, &x, &y);
	
	int w, h;
	SDL_GetWindowSize(other.window, &w, &h);
	
	window = SDL_CreateWindow(
			SDL_GetWindowTitle(other.window),
			x,
			y,
			w,
			h,
			SDL_GetWindowFlags(other.window)
	);
	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
			// | SDL_RENDERER_PRESENTVSYNC // Vsync seems to break the renderer initialization, hence it is commented out
	);
	
	return *this;
}

Window::Window() {
	window = nullptr;
	renderer = nullptr;
	running = false;
}

/**
 * @brief Draws a line
 * @param line The line to be drawn
 */
void Window::DrawLine(const Line &line) {
	DrawLine(line.getP1().first, line.getP1().second, line.getP2().first, line.getP2().second, line.getWidth(), line.getColor());
}

/**
 * @brief Draws a screen
 * @param screen The screen to be drawn
 */
void Window::Draw(Screen& screen) {
	Clear((SDL_Color){0, 0, 0, 255});
	// This draw method doesn't use absolutePath because I am very consistent (I'm not)
	Draw((SDL_Rect){0, 0, WindowData::SCREEN_WIDTH, WindowData::SCREEN_HEIGHT}, absolutePath(screen.getBackground()));
	for (const auto& t : screen.getTexts()){
		Draw(t);
	}
	
	for (const auto& b : screen.getButtons()) {
		Draw(b);
	}
}

/**
 * @brief Checks if the window should quit
 */
void Window::checkQuit() {
	for (auto event : Input::getEvents()){
		if (event.type == SDL_QUIT){
			running = false;
		}
	}
	if (Input::getKey("escape")){
		running = false;
	}
}

/**
 * @brief Draws a button
 * @param button The button to be drawn
 */
void Window::Draw(const Button &button) {
	Draw(*((GameObject *) &button), 0, SDL_FLIP_NONE); // The pointer cast is kind of dirty, but it's the quickest way I found to convince it to draw it as a GameObject
	Draw(*((Text *)&button.getText()));
}
