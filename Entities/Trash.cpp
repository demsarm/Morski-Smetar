#include "Trash.hpp"

#include "../Tools/Random.hpp"
#include "../Config/Config.hpp"

Trash::Trash() : GameObject() {
	type = Random::randint(0, 3);
	design.path =trash_types[type];
	
	initial_y = rect.y;
	offset = Random::randint(0, 50);
}

Trash::Trash(int x, int y) : GameObject() {
	rect = {x, y, 50, 50};
	type = Random::randint(0, 3);
	design.path = trash_types[type];
	
	initial_y = rect.y;
	offset = Random::randint(0, 50);
}

[[maybe_unused]] Trash::Trash(const Trash &other) = default;

void Trash::Update() {
	rect.x += 1;
	rect.y = (int)((double)initial_y + sin(0.03 * (rect.x + offset)) * 5);
}


[[maybe_unused]] uint8_t Trash::getType() const {
	return type;
}
