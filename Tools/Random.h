
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
	
	int Gaussian(int mean, int deviation);
	
	std::vector<std::vector<uint8_t>> worleyNoise(int width, int height, int points, int seed);
}

#endif //SDL_GAME_RANDOM_HPP
