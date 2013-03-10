#include "EnemyManager.h"
#include "constants.h"

#include <iterator>
#include <cstdlib>
#include <ctime>

// TODO: Delete, temporary for debugging
#include <fstream>
#include <iostream>
std::ofstream outputFile( "debug_output.txt", std::ios::out );
bool tmpDebug = false;

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
		if ( iter->getX() < 0 || iter->getX() + enemy_width > SCREEN_WIDTH )
		{
			iter->setXVel( iter->getXVel() * -1.0f );
		}
		if ( iter->getY() < 0 || iter->getY() + enemy_height > SCREEN_HEIGHT )
		{
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

void EnemyManager::addEnemy( SDL_Rect* spawnBuffer )
{
	// Randomly determine the new enemy's polarity
	polarity enemyPolarity = ( rand() % 2 == 0 ) ? WHITE : BLACK;
	SDL_Surface* enemyImage = ( enemyPolarity == WHITE ) ? enemyWhiteImage : enemyBlackImage;

	// Ensure that the enemy doesn't spawn too close to the player
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

	float xVel = (orientation == HORIZONTAL) ? enemy_speed : 0;
	float yVel = (orientation == VERTICAL) ? enemy_speed : 0;

	enemyContainer.push_back(GameObject(enemyImage, x, y, xVel, yVel, enemyPolarity));

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