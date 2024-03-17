//
// Created by martind on 10.3.2024.
//

#include "Button.h"


Button::Button(const SDL_Rect &rect, const Text& text, const Design &defaultDesign, const Design &hoverDesign, const Design &clickDesign,
               const std::function<void()> &onClick) : GameObject() {
	this->rect = rect;
	this->defaultDesign = defaultDesign;
	this->hoverDesign = hoverDesign;
	this->clickDesign = clickDesign;
	this->text = text;
	this->onClick = onClick; // PLEASE tell me functions have a copy constructor and assignment operator I CANNOT think of anything to do if they're like threads
	
	design = defaultDesign;
}

[[maybe_unused]]
Button::Button(const SDL_Rect &rect, const Text& text, const std::string &defaultPath, const std::string &hoverPath,
               const std::string &clickPath, const std::function<void()> &onClick) {
	this->rect = rect;
	this->defaultDesign = {SDL_Color{0, 0, 0, 0}, defaultPath};
	this->hoverDesign = {SDL_Color{0, 0, 0, 0}, hoverPath};
	this->clickDesign = {SDL_Color{0, 0, 0, 0}, clickPath};
	this->text = text;
	this->onClick = onClick;
	
	design = defaultDesign;
}

void Button::Update() {
	if (isPointInRect(Input::getMousePosition(), rect)) {
		if (Input::getMouseButton("Left")) {
			design = clickDesign;
		} else {
			design = hoverDesign;
		}
		if (Input::getMouseButtonUp("Left")) {
			onClick();
			
		}
	} else {
		design = defaultDesign;
	}
}

/**
 * @brief Sets the onClick function of the button
 * @param func The function to be called when the button is clicked
 * @note The function should not take any arguments and any return value will be ignored - use a lambda if you need to pass arguments
 */
void Button::setOnClick(const std::function<void()> &func) {
	onClick = func;
}

Button::Button(const SDL_Rect &rect, const Text &text, const SDL_Color &defaultColor, const SDL_Color &hoverColor,
               const SDL_Color &clickColor, const std::function<void()> &onClick) {
	this->rect = rect;
	this->defaultDesign = {defaultColor, ""};
	this->hoverDesign = {hoverColor, ""};
	this->clickDesign = {clickColor, ""};
	this->text = text;
	this->onClick = onClick;
	
	design = defaultDesign;
}
