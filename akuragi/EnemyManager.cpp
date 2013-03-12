#include "EnemyManager.h"
#include "constants.h"

#include <iterator>
#include <cstdlib>
#include <ctime>

// TODO: Delete, temporary for debugging
//#include <fstream>
//#include <iostream>
//std::ofstream outputFile( "debug_output.txt", std::ios::out );

using namespace Akuragi::Constants;
using namespace Akuragi::UtilFunctions;

typedef std::list<GameObject>::iterator golIter;

EnemyManager::EnemyManager(SDL_Surface* enemyWhiteImage, SDL_Surface* enemyBlackImage, SDL_Surface* dest)
	:enemyWhiteImage(enemyWhiteImage), enemyBlackImage(enemyBlackImage), dest(dest), enemy_width(20), enemy_height(20), enemy_speed(5), framesBetweenEnemies(60)
{
	orientation = HORIZONTAL;
}

void EnemyManager::update( int frameNum, SDL_Rect* spawnBuffer )
{
	for (golIter iter = enemyContainer.begin(), end = enemyContainer.end(); iter != end; iter++ )
	{
		// Update the motion each enemy
		iter->move();

		// If an enemy hits a wall, invert its orientation
		if ( iter->getX() < 0 || iter->getRightX() > SCREEN_WIDTH )
		{
			if ( iter->getX() < 0 )
				iter->setX(0.0f);
			if ( iter->getRightX() > SCREEN_WIDTH )
				iter->setX( (float)(SCREEN_WIDTH - iter->getWidth()) );
			iter->setXVel( iter->getXVel() * -1.0f );
		}
		if ( iter->getY() < 0 || iter->getBottomY() > SCREEN_HEIGHT )
		{
			if ( iter->getY() < 0 )
				iter->setY(0.0f);
			if ( iter->getBottomY() > SCREEN_HEIGHT )
				iter->setY( (float)(SCREEN_HEIGHT - iter->getHeight()) );
			iter->setYVel( iter->getYVel() * -1.0f );
		}

		// Show the enemy
		iter->show( dest );
	}

	if ( frameNum % framesBetweenEnemies == 0 )
	{
		addEnemy(spawnBuffer);
	}
}

// Note: Passing a pointer instead of a const reference to handle the situation where there is no spawnbuffer
void EnemyManager::addEnemy( SDL_Rect* spawnBuffer )
{
	// Randomly determine the new enemy's polarity and image
	polarity enemyPolarity = ( rand() % 2 == 0 ) ? WHITE : BLACK;
	SDL_Surface* enemyImage = ( enemyPolarity == WHITE ) ? enemyWhiteImage : enemyBlackImage;

	// Ensure that the enemy doesn't spawn too close to the player
	// TODO: Eventually ensure that if the player gets too large, it won't "box out" an entire area
	// TODO: Figure out all the integral types, wayyy too much back-and-forth typecasting going on
	// TODO: Move all of this into a function so that this function is easy to follow
	int bufferX = spawnBuffer->x - ENEMY_SPAWN_BUFFER; // TODO: consider converting these lines to a rect
	int bufferY = spawnBuffer->y - ENEMY_SPAWN_BUFFER;
	int bufferWidth = spawnBuffer->w + (2 * ENEMY_SPAWN_BUFFER);
	int bufferHeight = spawnBuffer->h + (2 * ENEMY_SPAWN_BUFFER);
	int bufferArea = bufferWidth * bufferHeight;

	int screenArea = SCREEN_WIDTH * SCREEN_HEIGHT;
	int spawnableAreaTotal = screenArea - bufferArea;

	// NOTE:
	// SpawnableAreaDiag represents the region that does not align with the player's spawn buffer
	// along both axes, x AND y
	// SpawnableAreaVert represents the region that aligns vertically with the player's spawn buffer,
	// but not horizontally
	// SpawnableAreaHoriz represents the region that aligns horizontally with the player's spawn buffer,
	// but not veritcally
	int spawnableAreaHoriz = ( SCREEN_WIDTH * bufferHeight ) - bufferArea;
	float horizRatio = ( (float)spawnableAreaHoriz / (float)spawnableAreaTotal ) * 100.0f;

	int spawnableAreaVert = ( SCREEN_HEIGHT * bufferWidth ) - bufferArea;
	float vertRatio = ( (float)spawnableAreaVert / (float)spawnableAreaTotal ) * 100.0f;

	// TODO: When done testing and debugging, delete these two lines, shouldn't need 'em
	int spawnableAreaDiag = screenArea - spawnableAreaHoriz - spawnableAreaVert - bufferArea;
	float diagRatio = ( (float)spawnableAreaDiag / (float)spawnableAreaTotal ) * 100.0f;

	// Use the value of areaDecider to decide which of the 3 areas to spawn an enemy
	float areaDecider = (float)(rand() % 101);

	int x = 0;
	int y = 0;
	if ( areaDecider < horizRatio )
	{
		// Spawn in the horizontal area
		x = rand() % ( SCREEN_WIDTH - bufferWidth );
		if ( x > bufferX )
		{
			x += bufferWidth;
		}

		y = ( rand() % bufferHeight ) + bufferY;

	}
	else if ( areaDecider < ( horizRatio + vertRatio ) )
	{
		// Spawn in the vertical area
		x = ( rand() % bufferWidth );

		y = rand() % ( SCREEN_HEIGHT - bufferHeight );
		if ( y > bufferY )
		{
			y += bufferHeight;
		}
	}
	else
	{
		// Spawn in the diagonal area
				// Spawn in the horizontal area
		x = rand() % ( SCREEN_WIDTH - bufferWidth );
		if ( x > bufferX )
		{
			x += bufferWidth;
		}

		y = rand() % ( SCREEN_HEIGHT - bufferHeight );
		if ( y > bufferY )
		{
			y += bufferHeight;
		}
	}

	/*
	float x = (float)(rand() % (SCREEN_WIDTH - ENEMY_SPAWN_BUFFER_WIDTH - enemyImage->w ));
	if ( spawnBuffer != NULL && x > spawnBuffer->x )
	{
		x += ENEMY_SPAWN_BUFFER_WIDTH;
	}
	float y = (float)(rand() % (SCREEN_HEIGHT - ENEMY_SPAWN_BUFFER_HEIGHT - enemyImage->h));
	if ( spawnBuffer != NULL && y > spawnBuffer->y )
	{
		y += ENEMY_SPAWN_BUFFER_HEIGHT;
	}
	*/

	// Determine the orientation and initial direction
	// TODO: Randomly choose WHICH direction to go in after the orientation has been selected
	float xVel = (orientation == HORIZONTAL) ? enemy_speed : 0;
	float yVel = (orientation == VERTICAL) ? enemy_speed : 0;

	enemyContainer.push_back(GameObject(enemyImage, (float)x, (float)y, xVel, yVel, enemyPolarity));

	toggleOrientation();
}

void EnemyManager::toggleOrientation()
{
	if ( orientation == VERTICAL )
		orientation = HORIZONTAL;
	else
		orientation = VERTICAL;
}

const std::list<GameObject>& EnemyManager::getEnemyContainer()
{
	return enemyContainer;
}

void EnemyManager::reset()
{
	enemyContainer.clear();
}

bool handle_enemy_player_collisions( EnemyManager& enemyManager, Player& player )
{
	bool playerDied = false;

	GameObject g;
	bool absorbed = false;
	
	std::list<GameObject> tempPlayerObjects = player.getPlayerObjects();

	// Iterate over the list of enemies
	golIter enemyIter = enemyManager.enemyContainer.begin(), enemyEnd = enemyManager.enemyContainer.end(); 
	while ( enemyIter != enemyEnd )
	{
		absorbed = false;
		// Iterate over each player object
		for ( golIter playerIter = tempPlayerObjects.begin(), playerEnd = tempPlayerObjects.end();
			  playerIter != playerEnd;
			  playerIter++ )
		{
			// Check if there is a collision with the enemy and the player
			if ( check_circular_collision( *enemyIter, *playerIter ) )
			{
				// Handle collisions accordingly
				if ( enemyIter->getPolarity() == playerIter->getPolarity() )
				{
					// The player has absorbed an enemy of the same polarity

					// TEMP, HACKY, TODO: FIX
					//g.setImage(SDL_ConvertSurface(enemyIter->getImage(), enemyIter->getImage()->format, enemyIter->getImage()->flags));
					g.setX(enemyIter->getX() + playerIter->getXVel());
					g.setY(enemyIter->getY() + playerIter->getYVel());
					g.setXVel(playerIter->getXVel());
					g.setYVel(playerIter->getYVel());
					g.setPolarity(enemyIter->getPolarity());
					absorbed = true;
					//player.absorbEnemy( g );
					// enemyIter = enemyManager.enemyContainer.erase(enemyIter);
				}
				else
				{
					// The player has hit an enemy of opposite polarity
					playerDied = true;
				}
			}
		}


		if ( absorbed ) {
			player.absorbEnemy(g);
			enemyIter = enemyManager.enemyContainer.erase(enemyIter);
		}
		else {
			enemyIter++;
		}
	}

	// Return an indicator of whether or not the player died
	return playerDied;
}