#include "SDL.h"
#include "SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *dots = NULL;
SDL_Surface *screen = NULL;

SDL_Event my_event;

SDL_Rect clip[4];

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

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
	// Make a temporary rectangle to hold the offsets and assign their values
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	// Blit the surface
	SDL_BlitSurface( source, clip, destination, &offset );
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
	dots = load_image( "dots.png" );

	if ( dots == NULL )
	{
		return false;
	}

	return true;
}

void clean_up()
{
	SDL_FreeSurface( dots );
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

	//Clip range for the top left
    clip[ 0 ].x = 0;
    clip[ 0 ].y = 0;
    clip[ 0 ].w = 100;
    clip[ 0 ].h = 100;
    
    //Clip range for the top right
    clip[ 1 ].x = 100;
    clip[ 1 ].y = 0;
    clip[ 1 ].w = 100;
    clip[ 1 ].h = 100;
    
    //Clip range for the bottom left
    clip[ 2 ].x = 0;
    clip[ 2 ].y = 100;
    clip[ 2 ].w = 100;
    clip[ 2 ].h = 100;
    
    //Clip range for the bottom right
    clip[ 3 ].x = 100;
    clip[ 3 ].y = 100;
    clip[ 3 ].w = 100;
    clip[ 3 ].h = 100;

	// Fill the screen white
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

	// Add the images to the screen
	apply_surface( 0, 0, dots, screen, &clip[0] );
	apply_surface( 540, 0, dots, screen, &clip[1] );
	apply_surface( 0, 380, dots, screen, &clip[2] );
	apply_surface( 540, 380, dots, screen, &clip[3] );

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