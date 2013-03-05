#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <sstream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int FRAMES_PER_SECOND = 20;

const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

SDL_Surface *dot = NULL;
SDL_Surface *screen = NULL;

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


// Dot class ------------------------------------------------------------------
class Dot
{
	private:
		// The X and Y offsets
		int x, y;

		// The velocity of the dot
		int xVel, yVel;

	public:
		// Ctor
		Dot();

		void handle_input();
		void move();
		void show();
};

Dot::Dot()
{
	x = 0;
	y = 0;
	xVel = 0;
	yVel = 0;
}

void Dot::handle_input()
{
	// If a key was pressed
	if ( my_event.type == SDL_KEYDOWN )
	{
		// Adjust the velocity
		switch ( my_event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel += DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel -= DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel += DOT_WIDTH / 2; break;
		}
	}
	// If a key was released
	else if ( my_event.type == SDL_KEYUP )
	{
		// Adjust the velocity
		switch ( my_event.key.keysym.sym )
		{
			case SDLK_UP: yVel += DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel += DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel -= DOT_WIDTH / 2; break;
		}
	}
}

void Dot::move()
{
	x += xVel;
	if ( ( x < 0 ) || ( x + DOT_WIDTH > SCREEN_WIDTH ) )
	{
		x -= xVel;
	}

	y += yVel;
	if ( ( y < 0 ) || ( y + DOT_HEIGHT > SCREEN_HEIGHT ) )
	{
		y -= yVel;
	}
}

void Dot::show()
{
	apply_surface( x, y, dot, screen );
}

// Timer class ------------------------------------------------------------------
class Timer
{
private:
	// The clock time when the timer started
	int startTicks;

	// The ticks stored when the timer is paused
	int pausedTicks;

	// The timer status
	bool paused;
	bool started;

public:
	Timer();

	// Clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	// Get the timer's time
	int get_ticks();

	// Check the status of the timer
	bool is_started();
	bool is_paused();
};

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::start()
{
	// Start the timer
	started = true;

	// Unpause the timer
	paused = false;

	// Get the current clock time
	startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	started = false;
	paused = false;
}

int Timer::get_ticks()
{
	if ( started )
	{
		if ( paused )
		{
			return pausedTicks;
		}
		else
		{
			return SDL_GetTicks() - startTicks;
		}
	}

	return 0;
}

void Timer::pause()
{
	if ( started && !paused )
	{
		paused = true;
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()
{
	if ( paused )
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
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
	dot = load_image( "dot.bmp" );

	font = TTF_OpenFont( "lazy.ttf", 36 );

	if ( dot == NULL || font == NULL )
	{
		return false;
	}

	return true;
}

void clean_up()
{
	SDL_FreeSurface( dot );

	TTF_CloseFont( font );
	TTF_Quit();

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

	Dot myDot;

	int frame = 0;
	bool cap = true;
	Timer fps;

	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	while ( quit == false )
	{
		// Start the frame timer
		fps.start();

		// While there are events to handle
		while ( SDL_PollEvent( &my_event ) )
		{
			myDot.handle_input();

			if ( my_event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		// Move the dot
		myDot.move();

		// Fill the screen white
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		// Show the dot on the screen
		myDot.show();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;    
        }

		frame++;

		// Cap the frame rate if necessary
		if ( cap && fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		{
			// Sleep the remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
		}
	}

	clean_up();

	return 0;
}