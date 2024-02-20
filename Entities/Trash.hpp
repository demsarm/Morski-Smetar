//
// Created by martind on 18.2.2024.
//

#ifndef MORSKI_SMETAR_TRASH_HPP
#define MORSKI_SMETAR_TRASH_HPP

#include <string>

#include "GameObject.hpp"

class Trash : public GameObject {
	inline static const std::string trash_types[] = {
			"Assets/Trash-Box.png",
			"Assets/Trash-Can.png",
			"Assets/Trash-Bottle.png",
	}; // I made this before I knew about enums, but I don't feel like changing it
	
	uint8_t type;
	
	int initial_y;
	int offset;
	
public:
	[[maybe_unused]] Trash();
	[[maybe_unused]] Trash(int x, int y);
	[[maybe_unused]] Trash(const Trash &other);
	[[maybe_unused]] [[nodiscard]] uint8_t getType() const;
	
	[[maybe_unused]]
	void Update() override;
};


#endif //MORSKI_SMETAR_TRASH_HPP
