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

void Screen::addText(const Text &text) {
	this->texts.push_back(text);
}

const std::string &Screen::getBackground() const {
	return this->background;
}

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

void Screen::clearTexts() {
	texts.clear();
}

void Screen::addButton(const Button &button) {
	buttons.push_back(Button(button)); // Copy because I'm not sure how it would work with the references
}

void Screen::clearButtons() {
	buttons.clear();
}

const std::vector<Button>& Screen::getButtons() const {
	return buttons;
}

void Screen::Update() {
	for (auto& button : buttons) {
		button.Update();
	}
}
