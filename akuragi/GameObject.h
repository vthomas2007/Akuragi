#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "SDL.h"

enum polarity { WHITE, BLACK };

class GameObject
{
public:
	// Constructors and destructor
	GameObject();
	GameObject(SDL_Surface* image, float x, float y, float xVel, float yVel, polarity polarityValue = WHITE);
	virtual ~GameObject();

	// Getters and setters
	float getX() const;
	float getY() const;
	float getXOld() const;
	float getYOld() const;
	float getXVel() const;
	float getYVel() const;
	SDL_Surface* getImage() const;
	polarity getPolarity() const;

	void setX(float xVal);
	void setY(float yVal);
	void setXOld(float xOldVal);
	void setYOld(float yOldVal);
	void setXVel(float xVelVal);
	void setYVel(float yVelVal);
	void setImage(SDL_Surface* newImage);
	void setPolarity(polarity polarityValueVal);

	void show(SDL_Surface* dest) const;
	void move();

protected:
	SDL_Surface *image;
	float x, y;
	float xOld, yOld;
	float xVel, yVel;
	polarity polarityValue;
};

#endif