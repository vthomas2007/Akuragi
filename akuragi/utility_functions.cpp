#include "utility_functions.h"
#include "constants.h"

using namespace Akuragi::Constants;

namespace Akuragi
{
	namespace UtilFunctions
	{
		bool init()
		{
			// Initialize all SDL subsystems
			if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
			{
				return false;
			}

			// Initialize SDL_ttf
			if ( TTF_Init() == -1 )
			{
				return false;
			}

			// Set the window caption
			SDL_WM_SetCaption( GAME_TITLE.c_str(), NULL );

			return true;
		}

		SDL_Surface* init_screen()
		{
			return SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
		}

		void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
		{
			// Make a temporary rectangle to hold the offsets and assign their values
			SDL_Rect offset;
			offset.x = x;
			offset.y = y;

			// Blit the surface
			SDL_BlitSurface( source, clip, destination, &offset );
		}

		SDL_Surface *load_image( std::string filename )
		{

			// Temporary storage for the image that's loaded
			SDL_Surface* loadedImage = NULL;

			// The optimized image that will be used
			SDL_Surface* optimizedImage = NULL;

			// Load the image
			loadedImage = IMG_Load( filename.c_str() );

			if (loadedImage != NULL)
			{
				// Create an optimized image
				optimizedImage = SDL_DisplayFormat( loadedImage );

				// Free the old image
				SDL_FreeSurface( loadedImage );

				// If the image as successfully optimized
				if ( optimizedImage != NULL )
				{
					// Map the color key
					Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );

					// Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
					SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
				}

				return optimizedImage;
			}
		}

		/*
		bool load_files()
		{
			// Load the image
			square = load_image( "square.bmp" );

			font = TTF_OpenFont( "lazy.ttf", 36 );

			if ( square == NULL || font == NULL )
			{
				return false;
			}

			return true;
		}
		*/

		bool check_collision( SDL_Rect A, SDL_Rect B )
		{
			// The sides of the rectangles
			int leftA, leftB;
			int rightA, rightB;
			int topA, topB;
			int bottomA, bottomB;

			// Calculate the sides of rect A
			leftA = A.x;
			rightA = A.x + A.w;
			topA = A.y;
			bottomA = A.y + A.h;

			// Calculate the sides of rect B
			leftB = B.x;
			rightB = B.x + B.w;
			topB = B.y;
			bottomB = B.y + B.h;

			if ( bottomA <= topB || topA >= bottomB )
			{
				return false;
			}

			if ( rightA <= leftB || leftA >= rightB )
			{
				return false;
			}

			return true;
		}
		/*
		void clean_up()
		{
			SDL_FreeSurface( square );

			TTF_CloseFont( font );
			TTF_Quit();

			SDL_Quit();
		}
		*/
	}
}