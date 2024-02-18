#include "Wave.hpp"

#include "../Tools/Random.hpp"
#include "../Config/Config.hpp"
Wave::Wave() = default;
Wave::Wave(int x, int y) {
	rect = {x, y, 50, 160};
	design.path = "Assets/Wave.png";
}
[[maybe_unused]] Wave::Wave(const Wave &other) = default;

Wave::~Wave() = default;

void Wave::Update() {
	rect.x -= 1;
	if (rect.x < 200) {
		rect.x = Random::randint(500, WindowData::SCREEN_WIDTH);
	}
}

