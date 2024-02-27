//
// Created by martind on 26.2.2024.
//

#ifndef MORSKI_SMETAR_LINE_HPP
#define MORSKI_SMETAR_LINE_HPP

#include <utility>
#include <SDL2/SDL.h>
class Line {
	std::pair<int, int> p1, p2;
	SDL_Color color;
	int width;
public:
	Line(int x1, int y1, int x2, int y2, SDL_Color color, int width);
	
	[[maybe_unused]] Line(const std::pair<int, int>& p1, const std::pair<int, int>& p2, SDL_Color color, int width);
	
	[[maybe_unused]] Line(const Line& other);
	Line& operator=(const Line& other);
	
	[[nodiscard]] std::pair<int, int> getP1() const;
	[[nodiscard]] std::pair<int, int> getP2() const;
	[[nodiscard]] SDL_Color getColor() const;
	[[nodiscard]] int getWidth() const;
};


#endif //MORSKI_SMETAR_LINE_HPP
