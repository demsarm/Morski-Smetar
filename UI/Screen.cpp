//
// Created by martind on 26.2.2024.
//

#include "Screen.h"


Screen::Screen(const std::string &background) {
	this->background = background;
}

Screen::Screen(const Screen &other) {
	this->background = other.background;
	this->texts = other.texts;
}

/**
 * Adds a text to the screen
 * @param text the text to be added
 */
void Screen::addText(const Text &text) {
	this->texts.push_back(text);
}

/**
 * Returns the background
 * @return the background
 */
const std::string &Screen::getBackground() const {
	return this->background;
}

/**
 * Sets the screen background
 * @param bg the background to be set
 */
void Screen::setBackground(const std::string &bg) {
	this->background = bg;
}

/**
 * Returns the (holy) texts
 * @return a vector of Text objects
 */
const std::vector<Text> &Screen::getTexts() const {
	return this->texts;
}

/**
 * Removes all texts from the screen
 */
void Screen::clearTexts() {
	texts.clear();
}

/**
 * Adds a button to the screen
 * @param button the button to be added
 */
void Screen::addButton(const Button &button) {
	buttons.push_back(Button(button)); // Copy because I'm not sure how it would work with the references
}

/**
 * Removes all buttons from the screen
 */
void Screen::clearButtons() {
	buttons.clear();
}

/**
 * Returns the buttons
 * @return a vector of Button objects
 */
const std::vector<Button>& Screen::getButtons() const {
	return buttons;
}

/**
 * Updates the screen
 */
void Screen::Update() {
	for (auto& button : buttons) {
		button.Update();
	}
}
