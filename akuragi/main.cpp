#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "square.h"
#include "GameObject.h"
#include "Player.h"
#include "EnemyManager.h"
#include "timer.h"
#include "utility_functions.h"
#include "constants.h"

#include <string>
#include <sstream>

using namespace Akuragi::UtilFunctions;
using namespace Akuragi::Constants;

SDL_Surface *square = NULL;
SDL_Surface *blackCircle = NULL;
SDL_Surface *whiteCircle = NULL;
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
	blackCircle = load_image( "black-circle.png" );
	whiteCircle = load_image( "white-circle.png" );
	font = TTF_OpenFont( "lazy.ttf", 36 );
	if ( square == NULL || blackCircle == NULL || whiteCircle == NULL || font == NULL )
	{
		return 1;
	}

	//Square mySquare(square);
	Player player( whiteCircle, blackCircle, 0, 0, 0, 0);
	EnemyManager enemyManager( whiteCircle, blackCircle, screen );
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
			player.handle_input( my_event );

			if ( my_event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		// Fill the screen white
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		// Move the square
		player.move();
		// TODO: Move this functionality to the player class and override cuz this ain't good
		if ( ( player.getX() < 0 ) || ( player.getX() + player.getImage()->w > SCREEN_WIDTH ) )
		{
			player.setX(player.getX() - player.getXVel());
		}

		if ( ( player.getY() < 0 ) || ( player.getY() + player.getImage()->h > SCREEN_HEIGHT ) )
		{
			player.setY(player.getY() - player.getYVel());
		}

		// Show the square on the screen
		player.show( screen );

		// Update and show the enemies
		enemyManager.update(frame, &player.getSpawnBuffer());

		// Check for enemy/player collisions
		if ( handle_enemy_player_collisions( enemyManager, player ) )
		{
			enemyManager.reset();
		}

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
	SDL_FreeSurface( blackCircle );
	SDL_FreeSurface( whiteCircle );
	SDL_FreeSurface( screen );
	TTF_CloseFont( font );
	TTF_Quit();
	SDL_Quit();

	return 0;
}