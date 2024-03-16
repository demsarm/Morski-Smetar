
#ifndef SDL_GAME_RANDOM_HPP
#define SDL_GAME_RANDOM_HPP

#include <random>
#include <vector>
#include <queue>
#include <cmath>
//#include "../Tools/Math.hpp"
namespace Random{
	double value();
	int randint(int max);
	int randint(int min, int max);
	
	// These two are leftovers from another project, no reason not for them to stay - compiler's going to cut 'em for optimization anyway
	[[maybe_unused]]
	int Gaussian(int mean, int deviation);
	
	[[maybe_unused]]
	std::vector<std::vector<uint8_t>> worleyNoise(int width, int height, int points);
}

#endif //SDL_GAME_RANDOM_HPP
