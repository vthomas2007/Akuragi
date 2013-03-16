#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"
#include <list>

class Player
{
public:
	Player(SDL_Surface* whiteImage, SDL_Surface* blackImage);
	virtual ~Player() { };

	SDL_Rect getBoundingBox() const;
	std::list<GameObject> getPlayerObjects() const;
	int getXLeft() const;
	int getXRight() const;
	int getYTop() const;
	int getYBottom() const;
	int getLives() const;
	int getScore() const;
	int getMultiplier() const;
	int getRespawnTimer() const;
	polarity getPolarity() const;

	void handle_input( const SDL_Event& event );
	void move();
	void show( SDL_Surface* dest ) const;
	void absorbEnemy( GameObject enemy );
	void die();
	void decrementRespawnTimer();
	void reset();

private:
	void updateBoundingBox();

	SDL_Surface* whiteImage;
	SDL_Surface* blackImage;
	std::list<GameObject> playerObjects;
	SDL_Rect boundingBox;

	// Not wild about this, redundant since the GameObject already has velocities
	// TODO: Refactor, find a way to eliminate this
	float xVel, yVel;

	int lives;
	int score;
	int multiplier;
	int respawnTimer;  // Should this live in some kind of game manager class? (TODO)
};

#endif