#include "Text.hpp"

Text::Text() : font_size(0) {}

// Clang-Tidy: Pass by value and use std::move (iont feel like it)
Text::Text(const std::string &text, const std::string &font_path, int font_size, const SDL_Color &color) : text(text), font_path(font_path), font_size(font_size), color(color) {}

void Text::setFontPath(const std::string &new_fontPath) {
	this->font_path = new_fontPath;
}

void Text::setFontSize(int new_fontSize) {
	this->font_size = new_fontSize;
}

void Text::setText(const std::string &new_text) {
	this->text = new_text;
}

void Text::setColor(const SDL_Color &new_color) {
	this->color = new_color;
}

const std::string &Text::getFontPath() const {
	return font_path;
}

int Text::getFontSize() const {
	return font_size;
}

const std::string &Text::getText() const {
	return text;
}

const SDL_Color &Text::getColor() const {
	return color;
}