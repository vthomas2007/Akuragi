#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

#include "SDL.h"
#include "GameObject.h"
#include <vector>

enum orientations { VERTICAL, HORIZONTAL };

class EnemyManager
{
public:
	// Constructors and destructors
	EnemyManager(SDL_Surface* enemyImage = NULL, SDL_Surface* dest = NULL);
	void update(int frameNum);
	void addEnemy(); // TODO: Make this private eventually and call it from update
	const std::vector<GameObject>& getEnemyContainer();

private:
	std::vector<GameObject> enemyContainer;
	orientations orientation;
	SDL_Surface* enemyImage;
	SDL_Surface* dest;

	int enemy_width;
	int enemy_height;
	float enemy_speed;

	int framesBetweenEnemies;

	void toggleOrientation();
};

#endif