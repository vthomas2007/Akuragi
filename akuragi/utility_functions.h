#ifndef _UTILITY_FUNCTIONS_H_
#define _UTILITY_FUNCTIONS_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <list>
#include "GameObject.h"
#include "SceneDeck.h"

namespace Akuragi
{
	namespace UtilFunctions
	{
		bool init();
		SDL_Surface* init_screen();
		void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
		SDL_Surface* load_image( std::string filename );
		bool check_circular_collision( GameObject A, GameObject B );
		bool check_collision( SDL_Rect A, SDL_Rect B );
		SceneDeck initalizeInstructions( GameObject* prevButton, GameObject* menuButton, GameObject* nextButton );
		//bool load_files();
		//void clean_up();
		int ftoi( float f );
		std::string itos ( int i );
	}
}

#endif