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
SDL_Surface *gameTitleText = NULL;
SDL_Surface *startGameText = NULL;
SDL_Surface *pausedText = NULL;

SDL_Event event;

TTF_Font *font = NULL;

SDL_Color blackTextColor = { 0, 0, 0 };
SDL_Color whiteTextColor = { 255, 255, 255 };


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
	gameTitleText = TTF_RenderText_Solid( font, "Akuragi", blackTextColor );
	startGameText = TTF_RenderText_Solid( font, "Press 'Enter' to begin", blackTextColor );
	pausedText = TTF_RenderText_Solid( font, "Press 'p' or 'enter' to resume", whiteTextColor );
	
	if ( square == NULL || blackCircle == NULL || whiteCircle == NULL || font == NULL )
	{
		return 1;
	}

	gameState currentState = INIT;

	Player player( whiteCircle, blackCircle );
	EnemyManager enemyManager( whiteCircle, blackCircle, screen );

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
		while ( SDL_PollEvent( &event ) )
		{
			// Look into the best way to organize all of this TODO fix
			if ( currentState == INIT )
			{
				if ( event.type == SDL_KEYDOWN )
				{
					if ( event.key.keysym.sym == SDLK_RETURN )
					{
						currentState = ACTIVE;
					}
				}
			}
			else if ( currentState == ACTIVE )
			{
				// Kind of reduntant TODO fix
				if ( event.type == SDL_KEYDOWN )
				{
					if ( event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_RETURN)
					{
						currentState = PAUSED;
					}
				}
				player.handle_input( event );
			}
			else if ( currentState == PAUSED )
			{
				if ( event.type == SDL_KEYDOWN )
				{
					if ( event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_RETURN )
					{
						currentState = ACTIVE;
					}
				}
			}
			
			if ( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		if ( currentState == INIT )
		{
			// Show a menu and stuff here, re-initialize any game variables so that you can
			// restart the game
			SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
			apply_surface( 100, 100, gameTitleText, screen );
			apply_surface( 100, 200, startGameText, screen );
		}
		else if ( currentState == ACTIVE )
		{
			if ( player.getRespawnTimer() == 0 )
			{
				// Fill the screen white
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

				// Move the player
				player.move();

				// Show the player on the screen
				player.show( screen );

				// Update and show the enemies
				enemyManager.update(frame, &player.getBoundingBox());

				// Check for enemy/player collisions
				if ( handle_enemy_player_collisions( enemyManager, player ) )
				{
					enemyManager.reset();
					player.die();
					if ( player.getLives() == 0 )
					{
						currentState = GAME_OVER;
					}
				}
			}
			else
			{
				Uint8 fillColor = (Uint8)(((float)player.getRespawnTimer() / (float)(PLAYER_RESPAWN_TIME)) * 255.0f);
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, fillColor, fillColor, fillColor ) );
				player.decrementRespawnTimer();
			}
		}
		else if ( currentState == PAUSED )
		{
			SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
			apply_surface( 50, 200, pausedText, screen );
		}
		else if ( currentState == GAME_OVER )
		{
			// Give option to restart the game
			player.reset();
			currentState = INIT;
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