#include "SDL.h"
#include "SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

SDL_Surface *load_image( std::string filename )
{
	// Temporary storage for the image that's loaded
	SDL_Surface* loadedImage = NULL;

	// The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	// Load the image
	loadedImage = SDL_LoadBMP( filename.c_str() );

	if (loadedImage != NULL)
	{
		// Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		// Free the old image
		SDL_FreeSurface( loadedImage );

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

int main(int arg, char** argv)
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return 1;
	}

	// Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if ( screen == NULL )
	{
		return 1;
	}

	// Set the window caption
	SDL_WM_SetCaption( "Hello World", NULL );

	// Load the images
	message = load_image( "hello.bmp" );
	background = load_image( "background.bmp" );

	apply_surface( 0, 0, background, screen );
	apply_surface( 320, 0, background, screen );
	apply_surface( 0, 240, background, screen );
	apply_surface( 320, 240, background, screen );

	apply_surface( 180, 140, message, screen );


	// Update the screen
	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	// Wait 2 seconds
	SDL_Delay( 2000 );

	SDL_FreeSurface( message );
	SDL_FreeSurface( background );

	SDL_Quit();

	return 0;
}