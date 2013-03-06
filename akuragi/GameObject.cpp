#include "GameObject.h"
#include "utility_functions.h"
#include <iostream>
#include <fstream>

using namespace Akuragi::UtilFunctions;

GameObject::GameObject()
{
}

GameObject::GameObject( SDL_Surface* image, float x, float y, float xVel, float yVel )
	: image(image), x(x), y(y), xOld(x), yOld(y), xVel(xVel), yVel(yVel)
{
}

GameObject::~GameObject()
{
}

void GameObject::show(SDL_Surface* dest) const
{
	// Make a temporary rectangle to hold the offests
	SDL_Rect offset;

	// Convert floats to ints and assign them to the offet rect
	offset.x = (int)(x + 0.5f);
	offset.y = (int)(y + 0.5f);

	// Blit the surface
	SDL_BlitSurface( image, NULL, dest, &offset );
}

void GameObject::move()
{
	xOld = x;
	yOld = y;

	x += xVel;
	y += yVel;
}

// GETTERS AND SETTERS -----------------------------------

float GameObject::getX() const
{
	return x;
}

float GameObject::getY() const
{
	return y;
}

float GameObject::getXOld() const
{
	return xOld;
}

float GameObject::getYOld() const
{
	return yOld;
}

float GameObject::getXVel() const
{
	return xVel;
}

float GameObject::getYVel() const
{
	return yVel;
}

void GameObject::setX(float xVal)
{
	x = xVal;
}

void GameObject::setY(float yVal)
{
	y = yVal;
}

void GameObject::setXOld(float xOldVal)
{
	xOld = xOldVal;
}

void GameObject::setYOld(float yOldVal)
{
	yOld = yOldVal;
}

void GameObject::setXVel(float xVelVal)
{
	xVel = xVelVal;
}

void GameObject::setYVel(float yVelVal)
{
	yVel = yVelVal;
}