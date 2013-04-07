#pragma once

#include <list>
#include <utility>
#include "SDL.h"
#include "GameObject.h"

class Scene
{
public:
	void show( SDL_Surface* dest );
	void addRect( SDL_Rect* rect, SDL_Color* color );
	//void addRect( int x, int y, int w, int h, int r, int g, int b );
	void addGameObject( GameObject gameObject );
	void addGameObject( SDL_Surface* image, int x, int y );

private:
	// TODO: This could be one list over an abstract class
	std::list< std::pair< SDL_Rect*, SDL_Color* > > rects;
	std::list< GameObject > gameObjects;
};