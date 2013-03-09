#include "Player.h"
#include "constants.h"
#include <cfloat>
#include "utility_functions.h"

using namespace Akuragi::Constants;
using namespace Akuragi::UtilFunctions;

typedef std::list<GameObject>::iterator golIter;
typedef std::list<GameObject>::const_iterator constGolIter;

Player::Player(SDL_Surface* whiteImage, SDL_Surface* blackImage)
	:whiteImage(whiteImage), blackImage(blackImage), xVel(0), yVel(0)
{
	playerObjects.push_back(GameObject(whiteImage, 0, 0, 0, 0, WHITE));
	updateBoundingBox();
}

SDL_Rect Player::getBoundingBox() const
{
	return boundingBox;
}

std::list<GameObject> Player::getPlayerObjects() const
{
	return playerObjects;
}

int Player::getXLeft() const
{
	return boundingBox.x;
}

int Player::getXRight() const
{
	return boundingBox.x + boundingBox.w;
}

int Player::getYTop() const
{
	return boundingBox.y;
}

int Player::getYBottom() const
{
	return boundingBox.y + boundingBox.h;
}

void Player::handle_input( const SDL_Event& event )
{
	float tempYVel = 0;
	float tempXVel = 0;
	bool changePolarity = false;
	polarity newPolarity = WHITE;
	SDL_Surface* newPolarityImage = NULL;

	// If a key was pressed
	if ( event.type == SDL_KEYDOWN )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= PLAYER_SPEED; break;
			case SDLK_DOWN: yVel += PLAYER_SPEED; break;
			case SDLK_LEFT: xVel -= PLAYER_SPEED; break;
			case SDLK_RIGHT: xVel += PLAYER_SPEED; break;
			case SDLK_SPACE: changePolarity = true; break;
		}
	}
	// If a key was released
	else if ( event.type == SDL_KEYUP )
	{
		// Adjust the velocity
		switch ( event.key.keysym.sym )
		{
			case SDLK_UP: yVel += PLAYER_SPEED; break;
			case SDLK_DOWN: yVel -= PLAYER_SPEED; break;
			case SDLK_LEFT: xVel += PLAYER_SPEED; break;
			case SDLK_RIGHT: xVel -= PLAYER_SPEED; break;
		}
	}

	// If necessary, set the new polarity value and image
	// NOTE: This will only work if you assume the player can only have all objects of one polarity type
	// and may need to be refactored in the future
	if ( changePolarity )
	{
		newPolarity = ( playerObjects.begin()->getPolarity() == WHITE ) ? BLACK : WHITE;
		newPolarityImage = ( newPolarity == WHITE ) ? whiteImage : blackImage;
	}

	for ( golIter iter = playerObjects.begin(), end = playerObjects.end(); iter != end; iter++ )
	{
		iter->setXVel( xVel );
		iter->setYVel( yVel );
		if ( changePolarity )
		{
			iter->setPolarity( newPolarity );
			iter->setImage( newPolarityImage );
		}
	}
}

void Player::move()
{
	updateBoundingBox();
	bool xShouldMove = false;
	bool yShouldMove = false;

	// If moving the player does not shove it out of bounds, move it
	if ( getXLeft() + xVel >= 0 && getXRight() + xVel <= SCREEN_WIDTH )
		xShouldMove = true;
	if ( getYTop() + yVel >= 0 && getYBottom() + yVel <= SCREEN_HEIGHT )
		yShouldMove = true;

	if ( xShouldMove || yShouldMove )
	{
		for ( golIter iter = playerObjects.begin(), end = playerObjects.end(); iter != end; iter++ )
		{
			if ( xShouldMove )
				iter->moveX();
			if ( yShouldMove )
				iter->moveY();
		}
	}
}

void Player::show( SDL_Surface* dest ) const
{
	for ( constGolIter iter = playerObjects.begin(), end = playerObjects.end(); iter != end; iter++ )
	{
		iter->show( dest );
	}
}

void Player::absorbEnemy( GameObject enemy )
{
	SDL_Surface* tmpImage = ( enemy.getPolarity() == WHITE ) ? whiteImage : blackImage;
	playerObjects.push_back( GameObject(tmpImage, enemy.getX(), enemy.getY(), 0, 0, enemy.getPolarity() ) );
}

void Player::updateBoundingBox()
{
	float xLeft = FLT_MAX;
	float xRight = FLT_MIN;
	float yTop = FLT_MAX;
	float yBottom = FLT_MIN;
	for ( golIter iter = playerObjects.begin(), end = playerObjects.end(); iter != end; iter++ )
	{
		if ( iter->getX() < xLeft )
			xLeft = iter->getX();
		if ( iter->getX() + iter->getImage()->w > xRight )
			xRight = iter->getX();
		if ( iter->getY() < yTop )
			yTop = iter->getY();
		if ( iter->getY() > yBottom )
			yBottom = iter->getY();
	}

	boundingBox.x = ftoi(xLeft);
	boundingBox.y = ftoi(yTop);
	boundingBox.w = ftoi(xRight - xLeft);
	boundingBox.h = ftoi(yBottom - yTop);
}