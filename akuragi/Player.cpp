#include "Player.h"

Player::Player(SDL_Surface* whiteImage, SDL_Surface* blackImage, float x, float y, float xVel, float yVel)
	:GameObject(whiteImage, x, y, xVel, yVel, WHITE), whiteImage(whiteImage), blackImage(blackImage)
{
}


// TODO: Create a subclass for the player and move these to it
void Player::handle_input( const SDL_Event& event )
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
			case SDLK_SPACE: togglePolarity(); break;
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

SDL_Rect Player::getSpawnBuffer() const
{
	SDL_Rect rect;
	rect.x = (int)(x + 0.5f);
	rect.y = (int)(y + 0.5f);
	rect.w = image->w;
	rect.h = image->h;

	return rect;
}

void Player::togglePolarity()
{
	if ( polarityValue == WHITE )
	{
		polarityValue = BLACK;
		image = blackImage;
	}
	else 
	{
		polarityValue = WHITE;
		image = whiteImage;
	}
}