#include "GameObject.h"
#include "utility_functions.h"
#include "constants.h" // can move this once you create a player sublcass (TODO)
#include <iostream>
#include <fstream>

using namespace Akuragi::UtilFunctions;
using namespace Akuragi::Constants;

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

// TODO: Create a subclass for the player and move this to it
void GameObject::handle_input( const SDL_Event& event )
{
	// If a key was pressed
	if ( event.type == SDL_KEYDOWN )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= image->h / 4; break;
			case SDLK_DOWN: yVel += image->h / 4; break;
			case SDLK_LEFT: xVel -= image->w / 4; break;
			case SDLK_RIGHT: xVel += image->w / 4; break;
		}
	}
	// If a key was released
	else if ( event.type == SDL_KEYUP )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel += image->h / 4; break;
			case SDLK_DOWN: yVel -= image->h / 4; break;
			case SDLK_LEFT: xVel += image->w / 4; break;
			case SDLK_RIGHT: xVel -= image->w / 4; break;
		}
	}
}


// GETTERS --------------------------------------------------

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

SDL_Surface* GameObject::getImage() const
{
	return image;
}

// SETTERS --------------------------------------------------

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

void GameObject::setImage(SDL_Surface* newImage)
{
	image = newImage;
}