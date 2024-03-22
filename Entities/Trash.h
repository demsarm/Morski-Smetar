#ifndef MORSKI_SMETAR_TRASH_H
#define MORSKI_SMETAR_TRASH_H

#include <string>

#include "GameObject.h"

class Trash : public GameObject {
public:
	enum class Type {
		BOX = 0,
		CAN = 1,
		BOTTLE = 2
	};
	
private:
	inline static const std::string trash_types[] = {
			"Assets/Trash-Box.png",
			"Assets/Trash-Can.png",
			"Assets/Trash-Bottle.png",
	}; // I made this before I knew about enums, but I don't feel like changing it
	
	Type type;
	
	
	int initial_y;
	int offset;
	
public:
	[[maybe_unused]] Trash();
	[[maybe_unused]] Trash(int x, int y);
	[[maybe_unused]] Trash(const Trash &other);
	[[maybe_unused]] [[nodiscard]] Type getType() const;
	
	[[maybe_unused]]
	void Update() override;
};


#endif //MORSKI_SMETAR_TRASH_H
