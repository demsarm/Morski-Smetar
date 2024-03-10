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
	buttons.push_back(new Button(button));
}

void Screen::clearButtons() {
	for (auto button : buttons) {
		delete button;
	}
	buttons.clear();
}

const std::vector<GameObject *>& Screen::getButtons() const {
	return buttons;
}

void Screen::Update() {
	for (auto button : buttons) {
		button->Update();
	}
}

