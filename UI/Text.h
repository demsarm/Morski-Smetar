//
// Created by martind on 18.2.2024.
//

#ifndef MORSKI_SMETAR_TEXT_H
#define MORSKI_SMETAR_TEXT_H

#include <string>

#include "../Entities/GameObject.h"
#include "SDL2/SDL_ttf.h"

class Text : public GameObject{
	std::string text;
	std::string font_path;
	int font_size;
	SDL_Color color{};
	
public:
	Text();
	Text(const std::string &text, const std::string &font_path, int font_size, const SDL_Color &color);
	
	[[nodiscard]] const std::string &getFontPath() const;
	[[maybe_unused]] void setFontPath(const std::string &fontPath);
	
	[[nodiscard]] int getFontSize() const;
	[[maybe_unused]] void setFontSize(int fontSize);

	[[nodiscard]] const std::string &getText() const;
	[[maybe_unused]] void setText(const std::string &text);
	
	[[nodiscard]] const SDL_Color &getColor() const;
	[[maybe_unused]] void setColor(const SDL_Color &color);
	
};


#endif //MORSKI_SMETAR_TEXT_H
