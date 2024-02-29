#include "Wave.h"

#include "../Tools/Random.h"
#include "../Config/Config.h"
Wave::Wave() = default;
Wave::Wave(int x, int y) {
	rect = {x, y, 50, 160};
	design.path = "Assets/Wave.png";
}
[[maybe_unused]] Wave::Wave(const Wave &other) = default;

Wave::~Wave() = default;

/**
 * @brief Literally just moves the wave to the left and resets it when it goes off screen
 */
void Wave::Update() {
	rect.x -= 1;
	if (rect.x < 200) {
		rect.x = Random::randint(500, WindowData::SCREEN_WIDTH);
	}
}

