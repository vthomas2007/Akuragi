#include "square.h"
#include "constants.h"
#include "utility_functions.h"

using namespace Akuragi::Constants;
using namespace Akuragi::UtilFunctions;

Square::Square()
{
	box.x = 0;
	box.y = 0;

	box.w = SQUARE_WIDTH;
	box.h = SQUARE_HEIGHT;

	xVel = 0;
	yVel = 0;
}

Square::Square(SDL_Surface* inputImage)
{
	box.x = 0;
	box.y = 0;

	box.w = SQUARE_WIDTH;
	box.h = SQUARE_HEIGHT;

	xVel = 0;
	yVel = 0;

	image = inputImage;
}

void Square::move()
{
	box.x += xVel;
	if ( (box.x < 0) || (box.x + SQUARE_WIDTH > SCREEN_WIDTH) )
	{
		box.x -= xVel;
	}

	box.y += yVel;
	if ( (box.y < 0) || (box.y + SQUARE_HEIGHT > SCREEN_HEIGHT) )
	{
		box.y -= yVel;
	}
}

void Square::handle_input(const SDL_Event& event)
{
	// If a key was pressed
	if ( event.type == SDL_KEYDOWN )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= SQUARE_HEIGHT / 2; break;
			case SDLK_DOWN: yVel += SQUARE_HEIGHT / 2; break;
			case SDLK_LEFT: xVel -= SQUARE_WIDTH / 2; break;
			case SDLK_RIGHT: xVel += SQUARE_WIDTH / 2; break;
		}
	}
	// If a key was released
	else if ( event.type == SDL_KEYUP )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel += SQUARE_HEIGHT / 2; break;
			case SDLK_DOWN: yVel -= SQUARE_HEIGHT / 2; break;
			case SDLK_LEFT: xVel += SQUARE_WIDTH / 2; break;
			case SDLK_RIGHT: xVel -= SQUARE_WIDTH / 2; break;
		}
	}
}

void Square::show(SDL_Surface* dest)
{
	apply_surface(box.x, box.y, image, dest);
}