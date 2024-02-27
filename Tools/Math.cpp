#include <valarray> // Apparently contains pow
#include "Math.hpp"
#include <cassert>
/**
 * @brief Returns the minimum of two values
 */
template <typename T>
T min(T a, T b) {
	return a < b ? a : b;
}

/**
 * @brief Returns the maximum of two values
 */
template <typename T>
T max(T a, T b) {
	return a > b ? a : b;
}

/**
 * @brief Returns the Euclidean distance between two points
 */
double distance(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double distance(const GameObject& a, const GameObject& b) {
	return distance(a.getRect().x, a.getRect().y, b.getRect().x, b.getRect().y);
}

/**
 * @brief Should return the point of intersection between a line and a circle
 * @note I have no idea if it works
 * @param x1 The x coordinate of the first point of the line
 * @param y1 The y coordinate of the first point of the line
 * @param x2 The x coordinate of the second point of the line
 * @param y2 The y coordinate of the second point of the line
 * @param cx The x coordinate of the center of the circle
 * @param cy The y coordinate of the center of the circle
 * @param r The radius of the circle
 */
std::pair<int, int> line_circle_intersection(int x1, int y1, int x2, int y2, int cx, int cy, int r) {
	double a = pow(x2 - x1, 2) + pow(y2 - y1, 2);
	double b = 2 * (x2 - x1) * (x1 - cx) + 2 * (y2 - y1) * (y1 - cy);
	double c = pow(x1 - cx, 2) + pow(y1 - cy, 2) - pow(r, 2);
	
	double dis = pow(b, 2) - 4 * a * c;
	assert(dis >= 0);
	
	double t1 = (-b + sqrt(dis)) / (2 * a);
	double t2 = (-b - sqrt(dis)) / (2 * a);
	
	assert((t1 >= 0 && t1 <= 1) != (t2 >= 0 && t2 <= 1));
	double t = t1 >= 0 && t1 <= 1 ? t1 : t2;
	return {(int) (x1 + (x2 - x1) * t), (int) (y1 + (y2 - y1) * t)};
}

 // Code that doesn't work but at least I understand it better
/*
 std::pair<int, int> line_circle_intersection(int x1, int y1, int x2, int y2, int cx, int cy, int r) {
	double k = (double) (y2 - y1) / (x2 - x1);
	double n = y1 - k * x1;
	
	double t = y1 - cy - k * x1;
	
	double a = 1 + pow(k, 2);
	double b = 2 * (cx + k * t);
	double c = pow(cx, 2) + pow(t, 2) - pow(r, 2);
	
	double dis = pow(b, 2) - 4 * a * c;
	
	if (dis < 0) {
		return {-1, -1};
	} else {
		double x1_inter = (-b + sqrt(dis)) / (2 * a);
		if (x1_inter > min(x1, x2) || x1_inter < max(x1, x2)) {
			int y_inter = (int) (k * x1_inter + n);
			return {(int) x1_inter, y_inter};
		} else {
			double x2_inter = (-b - sqrt(dis)) / (2 * a);
			int y_inter = (int) (k * x2_inter + n);
			return {(int) x2_inter, y_inter};
		}
	}
}*/
