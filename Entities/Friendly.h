#ifndef MORSKI_SMETAR_FRIENDLY_H
#define MORSKI_SMETAR_FRIENDLY_H


#include "GameObject.h"

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
	static SDL_Color sonar_color;
	
	Type type;
	Axis axis;
	Direction direction;
public:
	[[maybe_unused]] Friendly();
	[[maybe_unused]] [[maybe_unused]] Friendly(int x, int y);
	
	void Update() override;

	[[nodiscard]] [[maybe_unused]] Axis getAxis() const;
	[[maybe_unused]] void setAxis(Axis newaxis);
	
	[[nodiscard]] Direction getDirection() const;
	[[maybe_unused]] void setDirection(Direction direction);
	
	[[maybe_unused]] [[nodiscard]] Type getType() const;
	
	static void setSonarColor(SDL_Color newcolor);
	static SDL_Color getSonarColor();
};


#endif //MORSKI_SMETAR_FRIENDLY_H
