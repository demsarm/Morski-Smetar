#include "Random.hpp"

using namespace Random;

/**
 * @brief Returns a random number between 0 and 1
 * @return A random number between 0 and 1
 */
double Random::value() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);
	return dis(gen);
}

/**
 * @brief Returns a random number between 0 and max
 * @param max The maximum number
 * @return A random number between 0 and max
 */
int Random::randint(int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, max - 1);
	return dis(gen);
}

/**
 * @brief Returns a random number between min and max
 * @param min The minimum number
 * @param max The maximum number
 * @return A random number between min and max
 */
int Random::randint(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max - 1);
	return dis(gen);
}

/**
 * @brief Returns a 2D vector of worley noise
 * @param width The width of the noise
 * @param height The height of the noise
 * @param points The number of points to use
 * @param seed The seed to use
 * @return A 2D vector of worley noise
 */
std::vector<std::vector<uint8_t>> Random::worleyNoise(int width, int height, int points, int seed) {
	std::vector<std::vector<uint8_t>> noise;
	
	std::vector<std::vector<int>> points_list;
	
	for (int i = 0; i < points; i++) {
		points_list.push_back({randint(width), randint(height)});
	}
	
	for (int y = 0; y < height; y++) {
		std::vector<uint8_t> row;
		for (int x = 0; x < width; x++) {
			int min_dist = 100000;
			for (int i = 0; i < points; i++) {
//				int dist = (int)sqrt(pow(points_list[i][0] - x, 2) + pow(points_list[i][1] - y, 2)); // Uses euclidean distance
				 int dist = abs(points_list[i][0] - x) + abs(points_list[i][1] - y); // Uses manhattan distance
				if (dist < min_dist) {
					min_dist = dist;
				}
			}
			row.push_back(min_dist);
		}
		noise.push_back(row);
	}
	
	return noise;
}

/**
 * @brief Returns a random number from a gaussian distribution
 * @param mean The mean of the distribution
 * @param deviation The deviation of the distribution
 * @return A random number from a gaussian distribution
 */
int Random::Gaussian(int mean, int deviation) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> dis(mean, deviation);
	return (int)dis(gen);
}
