#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>

namespace Akuragi
{
	namespace UtilFunctions
	{
		bool init();
		SDL_Surface* init_screen();
		void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
		SDL_Surface* load_image( std::string filename );
		//bool load_files();
		bool check_collision( SDL_Rect A, SDL_Rect B );
		void clean_up();
	}
}