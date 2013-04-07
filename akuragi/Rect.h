#pragma once

#include "SDL.h"
#include <string>

class Rect
{
public:
	Rect();
	Rect( int x, int y, int w, int h );
	~Rect();

	SDL_Rect* get();

private:
	SDL_Rect* rect;
};