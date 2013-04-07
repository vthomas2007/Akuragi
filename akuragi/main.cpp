#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "resouces.h"
#include "ResourceManager.h"
#include "square.h"
#include "GameObject.h"
#include "Player.h"
#include "EnemyManager.h"
#include "timer.h"
#include "utility_functions.h"
#include "constants.h"
#include "Scene.h"
#include "SceneDeck.h"

#include <string>
#include <sstream>

using namespace Akuragi::UtilFunctions;
using namespace Akuragi::Constants;

// TODO: Delete, temporary for debugging
//#include <fstream>
//#include <iostream>
//std::ofstream outputFile( "debug_output.txt", std::ios::out );

SDL_Surface *screen = NULL;

SDL_Event event;

int main(int arg, char** argv)
{
	if ( init() == false )
	{
		return 1;
	}

	screen = init_screen();
	load_files();

	// Initialize Instructions
	GameObject* prevButton = new GameObject( rm.getImage( "prev-white" ), 250.0f, (float)INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0.0f, 0.0f );
	GameObject* nextButton = new GameObject( rm.getImage( "next-white" ), 500.0f, (float)INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0.0f, 0.0f );
	GameObject* menuButton = new GameObject( rm.getImage( "menu-white" ), 400.0f, (float)INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0.0f, 0.0f );
	SceneDeck instructions = initializeInstructions( prevButton, menuButton, nextButton );

	// Todo: Add checks to ensure resources are present
	//if ( square == NULL || blackCircle == NULL || whiteCircle == NULL || font == NULL )

	int previousScore = -1;
	gameState currentState = INIT;

	Player player( rm.getImage( "face" ), rm.getImage( "bowling-ball" ) );
	EnemyManager enemyManager( rm.getImage( "white-circle" ), rm.getImage( "black-circle" ), screen );
	SDL_Surface *instructionsButton = NULL;
	SDL_Surface *quitButton = NULL;
	Uint32 frameColor = SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 );

	int frame = 0;
	bool cap = true;
	Timer fps;

	Mix_PlayMusic( rm.getMusic( "katamari" ), -1 );
	Timer musicTimer;
	musicTimer.start();
	bool resetMusic = false; // hackity hack hack

	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	bool quit = false;
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
						// TODO make this suck less
						if ( resetMusic )
						{
//							Mix_PlayMusic( katamariMusic, -1 );
							musicTimer.start();
							resetMusic = false;
						}
						currentState = ACTIVE;
					}
				}
				else if ( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN )
				{
					int x = event.motion.x;
					int y = event.motion.y;

					if ( x > INSTRUCTIONS_BUTTON_X && 
						 x < INSTRUCTIONS_BUTTON_X + INSTRUCTIONS_BUTTON_WIDTH &&
						 y > INSTRUCTIONS_BUTTON_Y &&
						 y < INSTRUCTIONS_BUTTON_Y + INSTRUCTIONS_BUTTON_HEIGHT
						)
					{
						if ( event.type == SDL_MOUSEMOTION )
							instructionsButton = rm.getImage( "instructions-black" );
						else if ( event.type == SDL_MOUSEBUTTONDOWN )
						{
							currentState = INSTRUCTIONS;
							instructionsButton = rm.getImage( "instructions-white" );
						}
					}
					else
					{
						instructionsButton = rm.getImage( "instructions-white" );
					}

					if ( x > QUIT_BUTTON_X && 
						 x < QUIT_BUTTON_X + QUIT_BUTTON_WIDTH &&
						 y > QUIT_BUTTON_Y &&
						 y < QUIT_BUTTON_Y + QUIT_BUTTON_HEIGHT
						)
					{
						if ( event.type == SDL_MOUSEMOTION )
							quitButton = rm.getImage( "quit-black" );
						else if ( event.type == SDL_MOUSEBUTTONDOWN )
							quit = true;
					}
					else
					{
						quitButton = rm.getImage( "quit-white" );
					}

				}
			}
			else if ( currentState == INSTRUCTIONS )
			{
				instructions.handle_input( event );
				if ( instructions.getExitStatus() == true )
				{
					instructions.reset();
					currentState = INIT;
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
				polarity oldPolarity = player.getPolarity();
				player.handle_input( event );
				// If the player changes polarity, change the music as well
				if ( player.getPolarity() != oldPolarity )
				{
					//Mix_PlayChannel( -1, polarityShiftSound, 0 );
					//Mix_RewindMusic();
					if ( player.getPolarity() == BLACK )
					{
						double startPosition = ( musicTimer.get_ticks() / 1000 ) % 240;
						Mix_PlayMusic( rm.getMusic("ikaruga"), -1 );
						Mix_SetMusicPosition( startPosition );
					}
					else
					{
						double startPosition = ( musicTimer.get_ticks() / 1000 ) % 357;
						Mix_PlayMusic( rm.getMusic("katamari"), -1 );
						Mix_SetMusicPosition( startPosition );
					}
				}
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
			SDL_FillRect( screen, rm.getRect("title-background"), SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
			apply_surface( 200, 50, rm.getImage( "title-black" ), screen );
			apply_surface( 50, 375, rm.getText( "start-game" ), screen );
			apply_surface( INSTRUCTIONS_BUTTON_X, INSTRUCTIONS_BUTTON_Y, instructionsButton, screen );
			apply_surface( QUIT_BUTTON_X, QUIT_BUTTON_Y, quitButton, screen );

			if ( previousScore >= 0 )
			{
				Text previousScoreText( rm.getFont("medium"), std::string("Previous Score: ") + itos(previousScore), rm.getColor("gray") );
				apply_surface((SCREEN_WIDTH / 2) - (previousScoreText.get()->w / 2), 650, previousScoreText.get(), screen );
			}
		}
		else if ( currentState == INSTRUCTIONS )
		{
			instructions.show( screen );
		}
		else if ( currentState == ACTIVE )
		{
			if ( player.getRespawnTimer() == 0 )
			{
				// Fill the screen white
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

				// Draw the frame
				SDL_FillRect( screen, rm.getRect("left-frame"), frameColor );
				SDL_FillRect( screen, rm.getRect("right-frame"), frameColor );
				SDL_FillRect( screen, rm.getRect("top-frame"), frameColor );
				SDL_FillRect( screen, rm.getRect("middle-frame"), frameColor );
				SDL_FillRect( screen, rm.getRect("bottom-frame"), frameColor );

				// Draw the score, lives, and multiplier text
				// Todo: See if there's a way to simply update the text portion of this in the resource manager instead of having to keep constructing/destructing the entire
				// wrapper object
				Text scoreText( rm.getFont("medium"), std::string("Score: ") + itos(player.getScore()), rm.getColor("black") );
				apply_surface( 30, 650, scoreText.get(), screen );

				Text multiplierText( rm.getFont("medium"), std::string("Multiplier: ") + itos(player.getMultiplier()), rm.getColor("black") );
				apply_surface( 420, 650, multiplierText.get(), screen );

				Text livesText( rm.getFont("huge"), itos(player.getLives()), rm.getColor("gray") );
				apply_surface( 350, 155, livesText.get(), screen );

				// Move the player
				player.move();

				// Show the player on the screen
				player.show( screen );

				// Update and show the enemies
				enemyManager.update(frame, &player.getBoundingBox());

				// Check for enemy/player collisions
				// "collision" in this context means a player hit an enemy of OPPOSITE polarity
				if ( handle_enemy_player_collisions( enemyManager, player ) )
				{

					enemyManager.reset();
					player.die();
					if ( player.getLives() == 0 )
					{
						currentState = GAME_OVER;
					}
					else
					{
						Mix_PlayChannel( -1, rm.getSound("dead"), 0 );
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
			apply_surface( 50, 200, rm.getText("paused"), screen );
		}
		else if ( currentState == GAME_OVER )
		{
			//Mix_PlayChannel( -1, gameOverSound, 0 );
			// Give option to restart the game
			previousScore = player.getScore();
			if ( player.getPolarity() == BLACK )
			{
				resetMusic = true;
			}
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
	delete nextButton;
	delete menuButton;
	delete prevButton;

	// TODO: WRITE A RESOURCE MANAGER AND FREE ALL YOUR LOOSE SURFACES
	SDL_FreeSurface( screen );
	rm.clear();
	TTF_Quit();
	SDL_Quit();

	return 0;
}