//
// Created by martind on 26.2.2024.
//

#ifndef MORSKI_SMETAR_SCREEN_HPP
#define MORSKI_SMETAR_SCREEN_HPP


#include <string>
#include <vector>
#include "Text.hpp"

class Screen {
	std::string background;
	std::vector<Text> texts;
public:
	Screen() = default;
	Screen(const std::string& background);
	Screen(const Screen& other);
	
	[[nodiscard]] const std::string& getBackground() const;
	void setBackground(const std::string& background);
	
	[[nodiscard]] const std::vector<Text>& getTexts() const;
	
	void clearTexts();
	void addText(const Text& text);
};


#endif //MORSKI_SMETAR_SCREEN_HPP
