#include "Text.h"

// It's just a bunch of getter and setters - I'm not wasting my time adding docstrings to this

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

std::string& Text::getText() const {
	return (std::string&)text; // Binding reference to type 'basic_string<...>' to value of type 'const basic_string<...>' drops 'const' qualifier - hence the cast
}

const SDL_Color &Text::getColor() const {
	return color;
}

Text &Text::operator+=(const std::string &str) {
	text += str;
	return *this;
}
