#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

#include "SDL.h"
#include "utility_functions.h"
#include "Player.h"
#include <list>


enum orientations { VERTICAL, HORIZONTAL };

class EnemyManager
{
public:
	// Constructors and destructors
	EnemyManager(SDL_Surface* enemyWhiteImage, SDL_Surface* enemyBlackImage, SDL_Surface* dest = NULL);
	void update(int frameNum, SDL_Rect* spawnBuffer = NULL);
	void addEnemy(SDL_Rect* spawnBuffer = NULL); // TODO: Make this private eventually and call it from update
	const std::list<GameObject>& getEnemyContainer();
	void reset();

	friend bool handle_enemy_player_collisions( EnemyManager& enemyManager, Player& player );

private:
	std::list<GameObject> enemyContainer;
	orientations orientation;
	SDL_Surface* enemyWhiteImage;
	SDL_Surface* enemyBlackImage;
	SDL_Surface* dest;

	int enemy_width;
	int enemy_height;
	float enemy_speed;

	int framesBetweenEnemies;

	void toggleOrientation();
};

#endif