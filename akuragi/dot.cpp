#include "dot.h"
#include "constants.h"
#include "utility_functions.h"

using namespace Akuragi::Constants;
using namespace Akuragi::UtilFunctions;

const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

Dot::Dot()
{
	x = 0;
	y = 0;
	xVel = 0;
	yVel = 0;
}

void Dot::handle_input(const SDL_Event& event)
{
	// If a key was pressed
	if ( event.type == SDL_KEYDOWN )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel += DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel -= DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel += DOT_WIDTH / 2; break;
		}
	}
	// If a key was released
	else if ( event.type == SDL_KEYUP )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel += DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel += DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel -= DOT_WIDTH / 2; break;
		}
	}
}

void Dot::move()
{
	x += xVel;
	if ( ( x < 0 ) || ( x + DOT_WIDTH > SCREEN_WIDTH ) )
	{
		x -= xVel;
	}

	y += yVel;
	if ( ( y < 0 ) || ( y + DOT_HEIGHT > SCREEN_HEIGHT ) )
	{
		y -= yVel;
	}
}

void Dot::show(SDL_Surface* dest)
{
	apply_surface( x, y, image, dest );
}