#include "SDL.h"
#include "SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *foo = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

SDL_Event my_event;

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

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	// Make a temporary rectangle to hold the offsets and assign their values
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	// Blit the surface
	SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
	// Initialize all SDL subsystems
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;
	}

	// Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if ( screen == NULL )
	{
		return false;
	}

	// Set the window caption
	SDL_WM_SetCaption( "Event test", NULL );

	return true;
}

bool load_files()
{
	// Load the image
	background = load_image( "background.png" );
	foo = load_image( "foo.png" );

	if ( background == NULL || foo == NULL )
	{
		return false;
	}

	return true;
}

void clean_up()
{
	SDL_FreeSurface( background );
	SDL_FreeSurface( foo );
	SDL_Quit();
}

int main(int arg, char** argv)
{
	bool quit = false;

	if ( init() == false )
	{
		return 1;
	}

	if ( load_files() == false )
	{
		return 1;
	}

	apply_surface( 0, 0, background, screen );
	apply_surface( 240, 190, foo, screen );

	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	while ( quit == false )
	{
		while ( SDL_PollEvent( &my_event ) )
		{
			if ( my_event.type == SDL_QUIT )
			{
				quit = true;
			}
		}
	}

	clean_up();

	return 0;
}