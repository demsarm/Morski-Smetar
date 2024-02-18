#include "Trash.hpp"

#include "../Tools/Random.hpp"

Trash::Trash() : GameObject() {
	direction = Random::randint(0, 2);
	type = Random::randint(0, 3);
	design.path =trash_types[type];
	offset = Random::randint(0, 50);
	initialY = rect.y;
}

Trash::Trash(int x, int y) : GameObject() {
	rect = {x, y, 50, 50};
	direction = Random::randint(0, 2);
	initialY = y;
	type = Random::randint(0, 3);
	design.path = trash_types[type];
	offset = Random::randint(0, 50);
}

Trash::Trash(const Trash &other) = default;

void Trash::Update() {
	if (Random::value() < 0.01) rect.x -= direction ? 1 : -1;
	rect.y = (int)((double)initialY + sin(offset) * 10);
	offset += 0.01;
}

uint8_t Trash::getType() const {
	return type;
}
