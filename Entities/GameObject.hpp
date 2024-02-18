#ifndef SDL_GAME_GAMEOBJECT_HPP
#define SDL_GAME_GAMEOBJECT_HPP

#include <SDL2/SDL.h>
#include <string>

struct Design {
	SDL_Color color;
	std::string path;
};

class GameObject {
protected:
	SDL_Rect rect{0, 0,0,0};
	Design design;
public:
	GameObject();
	
	[[maybe_unused]] GameObject(const GameObject &other);
	virtual ~GameObject();
	
	[[nodiscard]] SDL_Rect getRect() const;
	[[nodiscard]] Design getDesign() const;
	
	void setPath(const std::string& path);
	[[maybe_unused]] void setColor(const SDL_Color& color);
	void setRect(const SDL_Rect& rect);
	
	virtual void Update();
	
	void move(int x, int y);
};

#endif //SDL_GAME_GAMEOBJECT_HPP