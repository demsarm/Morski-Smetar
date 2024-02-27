//
// Created by martind on 26.2.2024.
//

#include "Line.hpp"

Line::Line(int x1, int y1, int x2, int y2, SDL_Color color, int width) {
	p1 = {x1, y1};
	p2 = {x2, y2};
	this->color = color;
	this->width = width;
}

[[maybe_unused]] Line::Line(const std::pair<int, int> &p1, const std::pair<int, int> &p2, SDL_Color color, int width) {
	this->p1 = p1;
	this->p2 = p2;
	this->color = color;
	this->width = width;
}

[[maybe_unused]] Line::Line(const Line &other) {
	p1 = other.p1;
	p2 = other.p2;
	color = other.color;
	width = other.width;
}

Line &Line::operator=(const Line &other) = default;

[[nodiscard]] std::pair<int, int> Line::getP1() const {
	return p1;
}

[[nodiscard]] std::pair<int, int> Line::getP2() const {
	return p2;
}

[[nodiscard]] SDL_Color Line::getColor() const {
	return color;
}

[[nodiscard]] int Line::getWidth() const {
	return width;
}