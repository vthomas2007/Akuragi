#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "SDL.h"

class GameObject
{
public:
	// Constructors and destructor
	GameObject();
	GameObject(SDL_Surface* image, float x, float y, float xVel, float yVel);
	virtual ~GameObject();

	// Getters and setters
	float getX() const;
	float getY() const;
	float getXOld() const;
	float getYOld() const;
	float getXVel() const;
	float getYVel() const;

	void setX(float xVal);
	void setY(float yVal);
	void setXOld(float xOldVal);
	void setYOld(float yOldVal);
	void setXVel(float xVelVal);
	void setYVel(float yVelVal);

	void show(SDL_Surface* dest) const;
	void move();

protected:
	SDL_Surface *image;
	float x, y;
	float xOld, yOld;
	float xVel, yVel;
};

#endif