#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "square.h"
#include "GameObject.h"
#include "EnemyManager.h"
#include "timer.h"
#include "utility_functions.h"
#include "constants.h"

#include <string>
#include <sstream>

using namespace Akuragi::UtilFunctions;
using namespace Akuragi::Constants;

SDL_Surface *square = NULL;
SDL_Surface *dot = NULL;
SDL_Surface *screen = NULL;

SDL_Event my_event;

TTF_Font *font = NULL;

SDL_Color textColor = { 0, 0, 0 };


int main(int arg, char** argv)
{
	bool quit = false;

	if ( init() == false )
	{
		return 1;
	}

	screen = init_screen();

	// This is where "load files" used to be
	// TODO: Offload all of this to a resource manager
	square = load_image( "square.bmp" );
	dot = load_image( "black-circle.png" );
	font = TTF_OpenFont( "lazy.ttf", 36 );
	if ( square == NULL || dot == NULL || font == NULL )
	{
		return 1;
	}

	Square mySquare(square);
	EnemyManager enemyManager( dot, screen );
	//enemyManager.addEnemy();

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
			mySquare.handle_input( my_event );

			if ( my_event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		// Move the square
		mySquare.move();

		// Fill the screen white
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		// Show the square on the screen
		mySquare.show( screen );

		// Update and show the enemies
		enemyManager.update(frame);

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

	//clean_up();
	SDL_FreeSurface( square );
	TTF_CloseFont( font );
	TTF_Quit();
	SDL_Quit();

	return 0;
}