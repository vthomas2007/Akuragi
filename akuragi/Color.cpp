#include "Color.h"

Color::Color()
	:color(NULL)
{
}

Color::Color( int r, int g, int b )
{
	color = new SDL_Color;
	color->r = r;
	color->g = g;
	color->b = b;
}

Color::~Color()
{
	delete color;
}

SDL_Color* Color::get()
{
	return color;
}