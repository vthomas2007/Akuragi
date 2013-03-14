#include "Player.h"
#include "constants.h"
#include <cfloat>
#include "utility_functions.h"

using namespace Akuragi::Constants;
using namespace Akuragi::UtilFunctions;

typedef std::list<GameObject>::iterator golIter;
typedef std::list<GameObject>::const_iterator constGolIter;

Player::Player(SDL_Surface* whiteImage, SDL_Surface* blackImage)
	:whiteImage(whiteImage), blackImage(blackImage), xVel(0), yVel(0), respawnTimer(0), lives(PLAYER_LIVES),
	 score(0), multiplier(1)
{
	playerObjects.push_back(GameObject(whiteImage, (float)PLAYABLE_X_OFFSET, (float)PLAYABLE_Y_OFFSET, 0, 0, WHITE));
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

int Player::getLives() const
{
	return lives;
}

int Player::getScore() const
{
	return score;
}

int Player::getMultiplier() const
{
	return multiplier;
}

int Player::getRespawnTimer() const
{
	return respawnTimer;
}

void Player::handle_input( const SDL_Event& event )
{
	float tempYVel = 0;
	float tempXVel = 0;
	bool changePolarity = false;
	polarity newPolarity = WHITE;
	SDL_Surface* newPolarityImage = NULL;

	// If a key was pressed
	/*
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
			// TODO: There is a bug that occurs when
			// the player has a key held down during a state change, look into polling keystates
			// as possibly better alternative
			case SDLK_UP: yVel += PLAYER_SPEED; break;
			case SDLK_DOWN: yVel -= PLAYER_SPEED; break;
			case SDLK_LEFT: xVel += PLAYER_SPEED; break;
			case SDLK_RIGHT: xVel -= PLAYER_SPEED; break;
		}
	}
	*/
	// Try keystates to fix a bug that occurs when a player has a keydown while switching into ACTIVE
	// state, causing keyup to fire when you don't want it to
	if ( event.type == SDL_KEYDOWN )
	{
		if ( event.key.keysym.sym == SDLK_SPACE )
		{
			changePolarity = true;
		}
	}

	Uint8 *keyStates = SDL_GetKeyState( NULL );

	float tmpXVel = 0.0f;
	float tmpYVel = 0.0f;
	if ( keyStates[ SDLK_UP ] )
		tmpYVel -= PLAYER_SPEED;
	if ( keyStates[ SDLK_DOWN ] )
		tmpYVel += PLAYER_SPEED;
	if ( keyStates[ SDLK_LEFT ] )
		tmpXVel -= PLAYER_SPEED;
	if ( keyStates[ SDLK_RIGHT ] )
		tmpXVel += PLAYER_SPEED;

	xVel = tmpXVel;
	yVel = tmpYVel;

	// If necessary, set the new polarity value and image
	// NOTE: This will only work if you assume the player can only have all objects of one polarity type
	// and may need to be refactored in the future
	if ( changePolarity )
	{
		newPolarity = ( playerObjects.begin()->getPolarity() == WHITE ) ? BLACK : WHITE;
		newPolarityImage = ( newPolarity == WHITE ) ? whiteImage : blackImage;
		multiplier = 1;
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
	if ( getXLeft() + xVel >= LEFT_BORDER && getXRight() + xVel <= RIGHT_BORDER )
		xShouldMove = true;
	if ( getYTop() + yVel >= TOP_BORDER && getYBottom() + yVel <= BOTTOM_BORDER )
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
	playerObjects.push_back( GameObject(tmpImage, enemy.getX(), enemy.getY(), enemy.getXVel(), enemy.getYVel(), enemy.getPolarity() ) );
	score += ABSORB_POINTS * multiplier;
	multiplier++;
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
		if ( iter->getRightX() > xRight )
			xRight = iter->getRightX();
		if ( iter->getY() < yTop )
			yTop = iter->getY();
		if ( iter->getBottomY() > yBottom )
			yBottom = iter->getBottomY();
	}

	boundingBox.x = ftoi(xLeft);
	boundingBox.y = ftoi(yTop);
	boundingBox.w = ftoi(xRight - xLeft);
	boundingBox.h = ftoi(yBottom - yTop);
}

void Player::die()
{
	lives--;
	respawnTimer = PLAYER_RESPAWN_TIME;
}

void Player::decrementRespawnTimer()
{
	respawnTimer--;
}

void Player::reset()
{
	// TODO: Consider calling all of this functionality from the ctor to avoid duplicate code
	playerObjects.clear();
	playerObjects.push_back(GameObject(whiteImage, (float)PLAYABLE_X_OFFSET, (float)PLAYABLE_Y_OFFSET, 0, 0, WHITE));
	updateBoundingBox();
	lives = PLAYER_LIVES;
	respawnTimer = 0;
	score = 0;
	multiplier = 1;
}