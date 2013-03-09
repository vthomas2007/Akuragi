#include "EnemyManager.h"
#include "constants.h"
#include "utility_functions.h"
#include <iterator>
#include <cstdlib>
#include <ctime>

using namespace Akuragi::Constants;
using namespace Akuragi::UtilFunctions;

typedef std::list<GameObject>::iterator goIter;

EnemyManager::EnemyManager(SDL_Surface* enemyWhiteImage, SDL_Surface* enemyBlackImage, SDL_Surface* dest)
	:enemyWhiteImage(enemyWhiteImage), enemyBlackImage(enemyBlackImage), dest(dest), enemy_width(20), enemy_height(20), enemy_speed(5), framesBetweenEnemies(60)
{
	orientation = HORIZONTAL;
}

void EnemyManager::update(int frameNum, SDL_Rect* spawnBuffer)
{
	for (goIter iter = enemyContainer.begin(), end = enemyContainer.end(); iter != end; iter++ )
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

void EnemyManager::addEnemy(SDL_Rect* spawnBuffer )
{
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

bool handle_enemy_player_collisions( EnemyManager& enemyManager, GameObject& player )
{
	// Iterate over the list of enemies
	goIter iter = enemyManager.enemyContainer.begin(), end = enemyManager.enemyContainer.end(); 
	while ( iter != end )
	{
		// Check if there is a collision with the enemy and the player
		if ( check_circular_collision( *iter, player ) )
		{
			// Handle collisions accordingly
			if ( iter->getPolarity() == player.getPolarity() )
			{
				// The player has absorbed an enemy of the same polarity
				iter = enemyManager.enemyContainer.erase(iter);
			}
			else
			{
				// The player has hit an enemy of opposite polarity
				return true;
			}
		}
		else
		{
			iter++;
		}
	}

	// The player has not hit any enemies of opposite polarity
	return false;
}