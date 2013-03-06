#include "SDL.h"

class GameObject
{
private:
	SDL_Surface *image;
	int x;
	int y;
	int xVel;
	int yVel;
public:
	GameObject();
	virtual ~GameObject();
};