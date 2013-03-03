#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;

SDL_Event my_event;

TTF_Font *font = NULL;

SDL_Color textColor = { 255, 255, 255 };

SDL_Surface *buttonSheet = NULL;
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

SDL_Rect clips[4];

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
	// Make a temporary rectangle to hold the offsets and assign their values
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	// Blit the surface
	SDL_BlitSurface( source, clip, destination, &offset );
}

// The button
class Button
{
	private:
	// The attributes of the button
	SDL_Rect box;

	// The part of the button that will be shown
	SDL_Rect *clip;

	public:
	// Initialize the variables
	Button( int x, int y, int w, int h );

	// Handle events and set sprite region
	void handle_events();

	// Shows the button on the screen
	void show();
};


Button::Button( int x, int y, int w, int h )
{
	// Set the button's attributes
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;

	// Set the default sprite
	clip = &clips[ CLIP_MOUSEOUT ];
}

void Button::handle_events()
{
	// The mouse offsets
	int x = 0, y = 0;

	// If the mouse moved
	if ( my_event.type == SDL_MOUSEMOTION )
	{
		// Get the mouse offsets
		x = my_event.motion.x;
		y = my_event.motion.y;

		// If the mouse is over the button
		if ( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
		{
			// Set the button sprite
			clip = &clips[ CLIP_MOUSEOVER ];
		}
		else
		{
			clip = &clips[ CLIP_MOUSEOUT ];
		}
	}

	// If a mouse button was pressed
	if ( my_event.type == SDL_MOUSEBUTTONDOWN )
	{
		// If the left mouse button was pressed
		if ( my_event.button.button == SDL_BUTTON_LEFT )
		{
			// Get the mouse offsets
			x = my_event.button.x;
			y = my_event.button.y;

			// If the mouse is over the button
			if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
			{
				// Set the button sprite
				clip = &clips[ CLIP_MOUSEDOWN ];
			}
		}
	}

	// If a mouse button was released
	if ( my_event.type == SDL_MOUSEBUTTONUP )
	{
		// If the left mouse button was released
		if ( my_event.button.button == SDL_BUTTON_LEFT )
		{
			// Get the mouse offsets
			x = my_event.button.x;
			y = my_event.button.y;

			// If the mouse is over the button
			if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
			{
				// Set the button sprite
				clip = &clips[ CLIP_MOUSEUP ];
			}
		}
	}
}

void Button::show()
{
	// Show the button
	apply_surface( box.x, box.y, buttonSheet, screen, clip );
}


void set_clips()
{
	// Clpi the sprite sheet
	clips[ CLIP_MOUSEOVER ].x = 0;
    clips[ CLIP_MOUSEOVER ].y = 0;
    clips[ CLIP_MOUSEOVER ].w = 320;
    clips[ CLIP_MOUSEOVER ].h = 240;

    clips[ CLIP_MOUSEOUT ].x = 320;
    clips[ CLIP_MOUSEOUT ].y = 0;
    clips[ CLIP_MOUSEOUT ].w = 320;
    clips[ CLIP_MOUSEOUT ].h = 240;

    clips[ CLIP_MOUSEDOWN ].x = 0;
    clips[ CLIP_MOUSEDOWN ].y = 240;
    clips[ CLIP_MOUSEDOWN ].w = 320;
    clips[ CLIP_MOUSEDOWN ].h = 240;
    
    clips[ CLIP_MOUSEUP ].x = 320;
    clips[ CLIP_MOUSEUP ].y = 240;
    clips[ CLIP_MOUSEUP ].w = 320;
    clips[ CLIP_MOUSEUP ].h = 240;
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
	buttonSheet = load_image( "button.png" );

	font = TTF_OpenFont( "lazy.ttf", 28 );

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
	SDL_FreeSurface( buttonSheet );

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

	// Clip the sprite sheet
	set_clips();

	// Make the button
	Button myButton( 170, 120, 320, 240 );

	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	while ( quit == false )
	{
		if ( SDL_PollEvent( &my_event ) )
		{
			myButton.handle_events();

			if ( my_event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		// Fill the screen white
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		// Show the button
		myButton.show();

		// Update the screen
		if ( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}
	}

	clean_up();

	return 0;
}