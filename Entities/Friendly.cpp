//
// Created by martind on 20.2.2024.
//

#include "Friendly.hpp"
#include "../Config/Config.hpp"
#include "../Tools/Random.hpp"

Friendly::Friendly() : GameObject(){
	setRect({Random::randint((int)(WindowData::SCREEN_WIDTH * 0.1), (int)(WindowData::SCREEN_WIDTH * 0.9) - 70), Random::randint((int)(WindowData::SCREEN_HEIGHT * 0.1), (int)(WindowData::SCREEN_HEIGHT * 0.9) - 60), 70, 60});
	while (WindowData::SCREEN_WIDTH / 3 < rect.x && rect.x < WindowData::SCREEN_WIDTH / 3 + 70){
		rect.x = Random::randint((int)(WindowData::SCREEN_WIDTH * 0.1), (int)(WindowData::SCREEN_WIDTH * 0.9) - 70);
	}
	if (rect.x < WindowData::SCREEN_WIDTH / 3){
		type = Type::LAND;
	} else {
		type = Type::WATER;
	}
	axis = (Axis)(Random::randint(0, 2));
	direction = (Direction)(Random::randint(0, 2));
	if (type == WATER){
		setPath("Assets/Fishing-Boat.png");
	} else {
		setPath("Assets/Tractor.png");
	}
}

[[maybe_unused]] Friendly::Friendly(int x, int y) {
	rect = {x, y, 70, 60};
	if (rect.x < WindowData::SCREEN_WIDTH / 3){
		type = Type::LAND;
	} else {
		type = Type::WATER;
	}
	axis = (Axis)(Random::randint(0, 2));
	direction = (Direction)(Random::randint(0, 2));
}

void Friendly::Update() {
	if (axis == X){
		rect.x += direction == Direction::POSITIVE ? 1 : -1;
	} else {
		rect.y += direction == Direction::POSITIVE ? 1 : -1;
	}
	if ((type == LAND && rect.x <= 0) || (type == WATER && rect.x <= WindowData::SCREEN_WIDTH / 3)){
		direction = Direction::POSITIVE;
	} else if ((type == LAND && rect.x >= WindowData::SCREEN_WIDTH / 3 - 70) || (type == WATER && rect.x >= WindowData::SCREEN_WIDTH - 70)){
		direction = Direction::NEGATIVE;
	}
	if (rect.y <= 0){
		direction = Direction::POSITIVE;
	} else if (rect.y >= WindowData::SCREEN_HEIGHT - 60){
		direction = Direction::NEGATIVE;
	}
}

Friendly::Axis Friendly::getAxis() const {
	return axis;
}

void Friendly::setAxis(Friendly::Axis axis) {
	Friendly::axis = axis;
}

Friendly::Direction Friendly::getDirection() const {
	return direction;
}

void Friendly::setDirection(Friendly::Direction direction) {
	Friendly::direction = direction;
}
