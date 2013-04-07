#pragma once

#include "SDL.h"
#include <string>

class Color
{
public:
	Color();
	// TODO: Add ctor to read these from an XML file
	Color( int r, int g, int b );
	~Color();

	SDL_Color* get();

private:
	SDL_Color* color;
};