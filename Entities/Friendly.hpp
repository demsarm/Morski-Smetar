#ifndef MORSKI_SMETAR_FRIENDLY_HPP
#define MORSKI_SMETAR_FRIENDLY_HPP


#include "GameObject.hpp"

class Friendly : public GameObject {
public:
	enum Axis{
		X,
		Y
	};
	
	enum Direction{
		POSITIVE,
		NEGATIVE
	};
	
	enum Type{
		WATER,
		LAND
	};
	
private:
	Type type;
	Axis axis;
	Direction direction;
public:
	[[maybe_unused]] Friendly();
	[[maybe_unused]] [[maybe_unused]] Friendly(int x, int y);
	
	void Update() override;

	[[nodiscard]] [[maybe_unused]] Axis getAxis() const;
	[[maybe_unused]] void setAxis(Axis axis);
	
	[[nodiscard]] Direction getDirection() const;
	[[maybe_unused]] void setDirection(Direction direction);
};


#endif //MORSKI_SMETAR_FRIENDLY_HPP
