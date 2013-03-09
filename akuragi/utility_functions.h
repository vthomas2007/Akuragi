#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#ifndef _UTILITY_FUNCTIONS_H_
#define _UTILITY_FUNCTIONS_H_

#include <string>
#include <list>
#include "GameObject.h"

namespace Akuragi
{
	namespace UtilFunctions
	{
		bool init();
		SDL_Surface* init_screen();
		void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
		SDL_Surface* load_image( std::string filename );
		//bool load_files();
		bool handle_enemy_player_collisions( std::list<GameObject>& enemies, const GameObject& player );
		bool check_circular_collision( const GameObject& A, const GameObject& B );
		bool check_collision( SDL_Rect A, SDL_Rect B );
		void clean_up();
	}
}

#endif