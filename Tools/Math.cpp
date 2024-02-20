#include <valarray> // Apparently contains pow
#include "Math.hpp"

/*
 * @brief Returns the minimum of two values
 */
template <typename T>
T min(T a, T b) {
	return a < b ? a : b;
}

/*
 * @brief Returns the maximum of two values
 */
template <typename T>
T max(T a, T b) {
	return a > b ? a : b;
}

/*
 * @brief Returns the euclidean distance between two points
 */
double distance(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

/*
 * @brief Should return the point of intersection between a line and a circle
 * @note I have no idea if it works
 */
std::pair<int, int> line_circle_intersection(int x1, int y1, int x2, int y2, int cx, int cy, int r){
	int h = cx;
	int k = cy;
	
	double line_slope = ((double)(y2 - y1) / (x2 - x1));
	
	double y_intercept = y1 - line_slope * x1;
	
	double a = 1 + pow(line_slope, 2);
	double b = 2 * (line_slope * (y_intercept - k) - h);
	double c = pow(h, 2) - pow(r, 2) + pow(h, 2) - 2 * b * k + pow(b, 2);
	
	double dis = pow(b, 2) - 4 * a * c;
	
	if (dis < 0) {
		return {-1, -1};
	}
	else {
		double x1_inter = (-b + sqrt(dis)) / (2 * a);
//		double x2_inter = (-b - sqrt(dis)) / (2 * a); // these two are ignored for now
		double y1_inter = line_slope * x1 + y_intercept;
//		double y2_inter = line_slope * x2 + y_intercept;
		return {(int)x1_inter, (int)y1_inter};
	}
}