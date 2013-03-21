#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

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

// TODO: Delete, temporary for debugging
//#include <fstream>
//#include <iostream>
//std::ofstream outputFile( "debug_output.txt", std::ios::out );

// For the love of god create a resource manager
SDL_Surface *square = NULL;
SDL_Surface *blackCircle = NULL;
SDL_Surface *whiteCircle = NULL;
SDL_Surface *face = NULL;
SDL_Surface *bowlingBall = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *gameTitle = NULL;
SDL_Surface *startGameText = NULL;
SDL_Surface *pausedText = NULL;
SDL_Surface *blackInstructionsButton = NULL;
SDL_Surface *whiteInstructionsButton = NULL;
SDL_Surface *instructionsButton = NULL;
SDL_Surface *blackQuitButton = NULL;
SDL_Surface *whiteQuitButton = NULL;
SDL_Surface *quitButton = NULL;

SDL_Event event;

TTF_Font *font = NULL;

SDL_Color blackTextColor = { 0, 0, 0 };
SDL_Color livesTextColor = { 200, 200, 200 };
SDL_Color whiteTextColor = { 255, 255, 255 };


int main(int arg, char** argv)
{
	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
	Mix_Music* katamariMusic = Mix_LoadMUS( "music1.ogg" );
	Mix_Music* ikarugaMusic = Mix_LoadMUS( "music2.ogg" );
	Mix_PlayMusic( katamariMusic, -1 );
	Mix_VolumeMusic( 0 );

	//Mix_Chunk *gameOverSound = Mix_LoadWAV( "gameover.wav" );
	Mix_Chunk *deadSound = Mix_LoadWAV( "dead.wav" );

	bool quit = false;

	if ( init() == false )
	{
		return 1;
	}

	screen = init_screen();

	// This is where "load files" used to be
	// TODO: Offload all of this to a resource manager
	//square = load_image( "square.bmp" );
	gameTitle = load_image( "title-black.png" );
	whiteQuitButton = load_image( "quit-white.png" );
	blackQuitButton = load_image( "quit-black.png" );
	quitButton = whiteQuitButton;
	whiteInstructionsButton = load_image( "instructions-white.png" );
	blackInstructionsButton = load_image( "instructions-black.png" );
	instructionsButton = whiteInstructionsButton;
	blackCircle = load_image( "blank-black.png" );
	whiteCircle = load_image( "blank-white.png" );
	face = load_image( "happy-white.png" );
	bowlingBall = load_image( "bowling-ball-black.png" );
	font = TTF_OpenFont( "Luhyouone.ttf", 72 );
	TTF_Font* labelFont = TTF_OpenFont( "Luhyouone.ttf", 48 );
	TTF_Font* livesFont = TTF_OpenFont( "Luhyouone.ttf", 192 );
	//gameTitle = TTF_RenderText_Solid( font, "Akuragi", blackTextColor );
	startGameText = TTF_RenderText_Solid( font, "Press 'Enter' to begin", blackTextColor );
	//startGameText = TTF_RenderText_Solid( font, itos( gameTitleText->h ).c_str() , blackTextColor );
	pausedText = TTF_RenderText_Solid( labelFont, "Press 'p' or 'enter' to resume", whiteTextColor );
	SDL_Surface* scoreText = NULL;
	SDL_Surface* multiplierText = NULL;
	SDL_Surface* livesText = NULL;
	SDL_Surface* previousScoreText = NULL;

	SDL_Rect leftFrame;
	leftFrame.x = 0;
	leftFrame.y = 0;
	leftFrame.w = PLAYABLE_X_OFFSET;
	leftFrame.h = SCREEN_HEIGHT;
	
	SDL_Rect rightFrame;
	rightFrame.x = SCREEN_WIDTH - PLAYABLE_X_OFFSET;
	rightFrame.y = 0;
	rightFrame.w = PLAYABLE_X_OFFSET;
	rightFrame.h = SCREEN_HEIGHT;
	
	SDL_Rect topFrame;
	topFrame.x = PLAYABLE_X_OFFSET;
	topFrame.y = 0;
	topFrame.w = PLAYABLE_WIDTH;
	topFrame.h = PLAYABLE_Y_OFFSET;
	
	SDL_Rect middleFrame;
	middleFrame.x = PLAYABLE_X_OFFSET;
	middleFrame.y = PLAYABLE_Y_OFFSET + PLAYABLE_HEIGHT;
	middleFrame.w = PLAYABLE_WIDTH;
	middleFrame.h = PLAYABLE_X_OFFSET;
	
	SDL_Rect bottomFrame;
	bottomFrame.x = PLAYABLE_X_OFFSET;
	bottomFrame.y = SCREEN_HEIGHT - PLAYABLE_Y_OFFSET;
	bottomFrame.w = PLAYABLE_WIDTH;
	bottomFrame.h = PLAYABLE_Y_OFFSET;
	
	Uint32 frameColor = SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 );
	// END TODO

	// Todo: Add checks to ensure resources are present
	//if ( square == NULL || blackCircle == NULL || whiteCircle == NULL || font == NULL )

	int previousScore = -1;
	gameState currentState = INIT;

	Player player( face, bowlingBall );
	EnemyManager enemyManager( whiteCircle, blackCircle, screen );

	int frame = 0;
	bool cap = true;
	Timer fps;

	Timer musicTimer;
	musicTimer.start();
	bool resetMusic = false; // hackity hack hack

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
						// TODO make this suck less
						if ( resetMusic )
						{
							Mix_PlayMusic( katamariMusic, -1 );
							musicTimer.start();
							resetMusic = false;
						}
						currentState = ACTIVE;
					}
				}
				else if ( event.type == SDL_MOUSEMOTION )
				{
					int x = event.motion.x;
					int y = event.motion.y;

					if ( x > INSTRUCTIONS_BUTTON_X && 
						 x < INSTRUCTIONS_BUTTON_X + INSTRUCTIONS_BUTTON_WIDTH &&
						 y > INSTRUCTIONS_BUTTON_Y &&
						 y < INSTRUCTIONS_BUTTON_Y + INSTRUCTIONS_BUTTON_HEIGHT
						)
					{
						instructionsButton = blackInstructionsButton;
					}
					else
					{
						instructionsButton = whiteInstructionsButton;
					}

					if ( x > QUIT_BUTTON_X && 
						 x < QUIT_BUTTON_X + QUIT_BUTTON_WIDTH &&
						 y > QUIT_BUTTON_Y &&
						 y < QUIT_BUTTON_Y + QUIT_BUTTON_HEIGHT
						)
					{
						quitButton = blackQuitButton;
					}
					else
					{
						quitButton = whiteQuitButton;
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
						Mix_PlayMusic( ikarugaMusic, -1 );
						Mix_SetMusicPosition( startPosition );
					}
					else
					{
						double startPosition = ( musicTimer.get_ticks() / 1000 ) % 357;
						Mix_PlayMusic( katamariMusic, -1 );
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
			apply_surface( 270, 100, gameTitle, screen );
			apply_surface( 50, 375, startGameText, screen );
			apply_surface( INSTRUCTIONS_BUTTON_X, INSTRUCTIONS_BUTTON_Y, instructionsButton, screen );
			apply_surface( QUIT_BUTTON_X, QUIT_BUTTON_Y, quitButton, screen );

			if ( previousScore >= 0 )
			{
				previousScoreText = TTF_RenderText_Solid( labelFont, (std::string("Previous Score: ") + itos(previousScore)).c_str(), livesTextColor );
				apply_surface(200, 500, previousScoreText, screen );
			}
		}
		else if ( currentState == ACTIVE )
		{
			if ( player.getRespawnTimer() == 0 )
			{
				// Fill the screen white
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

				// Draw the frame
				SDL_FillRect( screen, &leftFrame, frameColor );
				SDL_FillRect( screen, &rightFrame, frameColor );
				SDL_FillRect( screen, &topFrame, frameColor );
				SDL_FillRect( screen, &bottomFrame, frameColor );
				SDL_FillRect( screen, &middleFrame, frameColor );

				// Draw the score, lives, and multiplier text
				scoreText = TTF_RenderText_Solid( labelFont, (std::string("Score: ") + itos(player.getScore())).c_str(), blackTextColor );
				apply_surface( 30, 650, scoreText, screen );
				SDL_FreeSurface( scoreText );

				multiplierText = TTF_RenderText_Solid( labelFont, (std::string("Multiplier: ") + itos(player.getMultiplier())).c_str(), blackTextColor );
				apply_surface( 420, 650, multiplierText, screen );
				SDL_FreeSurface( multiplierText );

				livesText = TTF_RenderText_Solid( livesFont, itos(player.getLives()).c_str(), livesTextColor );
				apply_surface( 350, 155, livesText, screen );
				SDL_FreeSurface( livesText );

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
						Mix_PlayChannel( -1, deadSound, 0 );
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
	SDL_FreeSurface( square );
	SDL_FreeSurface( blackCircle );
	SDL_FreeSurface( whiteCircle );
	SDL_FreeSurface( screen );
	TTF_CloseFont( font );
	TTF_CloseFont( labelFont );
	TTF_Quit();
	SDL_Quit();

	return 0;
}