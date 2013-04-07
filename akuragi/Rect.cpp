#include "Rect.h"

Rect::Rect()
	:rect(NULL)
{
}

Rect::Rect( int x, int y, int w, int h )
{
	rect = new SDL_Rect;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}

Rect::~Rect()
{
	delete rect;
}

SDL_Rect* Rect::get()
{
	return rect;
}