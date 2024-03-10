//
// Created by martind on 10.3.2024.
//

#ifndef MORSKI_SMETAR_BUTTON_H
#define MORSKI_SMETAR_BUTTON_H


#include <functional>
#include "../Entities/GameObject.h"
#include "Input.h"
#include "../Tools/Collision.h"
#include "Text.h"

class Button : public GameObject {
	std::function<void()> onClick;
	
	Design defaultDesign;
	Design hoverDesign;
	Design clickDesign;
	
	Text text;
public:
	Button() = default;
	Button(const Button &other) = default;
	~Button() override = default;
	
	Button(const SDL_Rect& rect, const Text& text, const Design& defaultDesign, const Design& hoverDesign, const Design& clickDesign, const std::function<void()> &onClick = [](){});
	Button(const SDL_Rect& rect, const Text& text, const std::string& defaultPath, const std::string& hoverPath, const std::string& clickPath, const std::function<void()> &onClick = [](){});
	Button(const SDL_Rect& rect, const Text& text, const SDL_Color& defaultColor, const SDL_Color& hoverColor, const SDL_Color& clickColor, const std::function<void()> &onClick = [](){});
	
	[[nodiscard]] const Text &getText() const {
		return text;
	}
	
	void Update() override;
	
	/**
	 * @brief Sets the function to be called when the button is clicked
	 * @param onClick the function to be called when the button is clicked. Use a lambda function to call a function with parameters I can't be bothered to implement passing parameters it's way too complicated (I mean that's everything in C++ but still this one is especially painful)
	 */
	void setOnClick(const std::function<void()> &func);
};


#endif //MORSKI_SMETAR_BUTTON_H
