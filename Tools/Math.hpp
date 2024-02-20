#ifndef MORSKI_SMETAR_MATH_HPP
#define MORSKI_SMETAR_MATH_HPP

#include <utility>

template <typename T>
T min(T a, T b);
template <typename T>
T max(T a, T b);

double distance(int x1, int y1, int x2, int y2);
std::pair<int, int> line_circle_intersection(int x1, int y1, int x2, int y2, int cx, int cy, int r);

#endif //MORSKI_SMETAR_MATH_HPP
