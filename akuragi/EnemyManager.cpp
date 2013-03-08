#include "EnemyManager.h"
#include "constants.h"
#include <iterator>
#include <cstdlib>
#include <ctime>

using namespace Akuragi::Constants;

typedef std::vector<GameObject>::iterator goIter;

EnemyManager::EnemyManager(SDL_Surface* enemyImage, SDL_Surface* dest)
	:enemyImage(enemyImage), dest(dest), enemy_width(20), enemy_height(20), enemy_speed(5), framesBetweenEnemies(60)
{
	orientation = HORIZONTAL;
}

void EnemyManager::update(int frameNum)
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
		addEnemy();
	}
}

void EnemyManager::addEnemy()
{
	float xVel = (orientation == HORIZONTAL) ? enemy_speed : 0;
	float yVel = (orientation == VERTICAL) ? enemy_speed : 0;

	enemyContainer.push_back(GameObject(
		enemyImage,
		(float)(rand() % (SCREEN_WIDTH - enemy_width )),
		(float)(rand() % (SCREEN_HEIGHT - enemy_height )),
		xVel,
		yVel
	));

	toggleOrientation();
}

void EnemyManager::toggleOrientation()
{
	if ( orientation == VERTICAL )
		orientation = HORIZONTAL;
	else
		orientation = VERTICAL;
}

const std::vector<GameObject>& EnemyManager::getEnemyContainer()
{
	return enemyContainer;
}