#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <sstream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;

SDL_Surface *seconds = NULL;
SDL_Surface *startStop = NULL;

SDL_Event my_event;

TTF_Font *font = NULL;

SDL_Color textColor = { 0, 0, 0 };

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
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

	// Initialize SDL_ttf
	if ( TTF_Init() == -1 )
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

	font = TTF_OpenFont( "lazy.ttf", 36 );

	if ( background == NULL || font == NULL )
	{
		return false;
	}

	return true;
}

void clean_up()
{
	SDL_FreeSurface( background );
	SDL_FreeSurface( message );

	SDL_FreeSurface( startStop );

	TTF_CloseFont( font );
	TTF_Quit();

	SDL_Quit();
}

int main(int arg, char** argv)
{
	bool quit = false;

	// The timer starting time
	Uint32 start = 0;

	// The timer start/stop flag
	bool running = true;


	if ( init() == false )
	{
		return 1;
	}

	if ( load_files() == false )
	{
		return 1;
	}

	// Start the timer
	start = SDL_GetTicks();

	startStop = TTF_RenderText_Solid( font, "Press S to start or stop the timer", textColor );

	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	while ( quit == false )
	{
		// While there are events to handle
		while ( SDL_PollEvent( &my_event ) )
		{
			// If a key was pressed
			if ( my_event.type == SDL_KEYDOWN )
			{
				// If 's' was pressed
				if ( my_event.key.keysym.sym == SDLK_s )
				{
					// If the timer is running
					if ( running )
					{
						// Stop the timer
						running = false;
						start = 0;
					}
					else
					{
						// Start the timer
						running = true;
						start = SDL_GetTicks();
					}
				}
			}
			else if ( my_event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		apply_surface( 0, 0, background, screen );

		apply_surface( (SCREEN_WIDTH - startStop->w ) / 2, 200, startStop, screen );

		// If the timer is running
		if ( running )
		{
			// The timer's time as a string
			std::stringstream time;

			// Convert the timer's time to a string
			time << "Timer: " << SDL_GetTicks() - start;

			// Render the time surface
			seconds = TTF_RenderText_Solid( font, time.str().c_str(), textColor );

			// Apply the time surface
			apply_surface( ( SCREEN_WIDTH - seconds->w ) / 2, 50, seconds, screen );

			// Free the time surface
			SDL_FreeSurface( seconds );
		}

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;    
        }
	}

	clean_up();

	return 0;
}