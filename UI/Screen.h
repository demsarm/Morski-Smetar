//
// Created by martind on 26.2.2024.
//

#ifndef MORSKI_SMETAR_SCREEN_H
#define MORSKI_SMETAR_SCREEN_H


#include <string>
#include <vector>
#include "Text.h"
#include "Button.h"

class Screen {
	std::string background;
	// TODO: Make everything on a screen an std::vector<GameObject *> so that we can have buttons and stuff
	std::vector<Text> texts;
	std::vector<Button> buttons;
public:
	void Update();
	
	Screen() = default;
	[[maybe_unused]]
	explicit Screen(const std::string& background);
	Screen(const Screen& other);
	
	[[nodiscard]] const std::string& getBackground() const;
	void setBackground(const std::string& background);
	
	[[nodiscard]] const std::vector<Text>& getTexts() const;
	
	void clearTexts();
	void addText(const Text& text);
	void addButton(const Button& button);
	void clearButtons();
	
	[[nodiscard]] const std::vector<Button>& getButtons() const;
};


#endif //MORSKI_SMETAR_SCREEN_H
