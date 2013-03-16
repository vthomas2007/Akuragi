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
#include <fstream>
#include <iostream>
std::ofstream outputFile( "debug_output.txt", std::ios::out );

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
SDL_Color livesTextColor = { 200, 200, 200 };
SDL_Color whiteTextColor = { 255, 255, 255 };


int main(int arg, char** argv)
{
	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
	Mix_Music* katamariMusic = Mix_LoadMUS( "music1.ogg" );
	Mix_Music* ikarugaMusic = Mix_LoadMUS( "music2.ogg" );
	Mix_PlayMusic( katamariMusic, -1 );

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
	font = TTF_OpenFont( "Luhyouone.ttf", 72 );
	TTF_Font* labelFont = TTF_OpenFont( "Luhyouone.ttf", 48 );
	TTF_Font* livesFont = TTF_OpenFont( "Luhyouone.ttf", 192 );
	gameTitleText = TTF_RenderText_Solid( font, "Akuragi", blackTextColor );
	startGameText = TTF_RenderText_Solid( font, "Press 'Enter' to begin", blackTextColor );
	//startGameText = TTF_RenderText_Solid( font, itos( gameTitleText->h ).c_str() , blackTextColor );
	pausedText = TTF_RenderText_Solid( font, "Press 'p' or 'enter' to resume", whiteTextColor );
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

	
	if ( square == NULL || blackCircle == NULL || whiteCircle == NULL || font == NULL )
	{
		return 1;
	}

	int previousScore = -1;
	// Temporary for easier testing, TODO: change initial state back to INIT
	//gameState currentState = INIT;
	gameState currentState = ACTIVE;

	Player player( whiteCircle, blackCircle );
	EnemyManager enemyManager( whiteCircle, blackCircle, screen );

	int frame = 0;
	bool cap = true;
	Timer fps;
	Timer musicTimer;
	musicTimer.start();

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
				polarity oldPolarity = player.getPolarity();
				player.handle_input( event );
				// If the player changes polarity, change the music as well
				if ( player.getPolarity() != oldPolarity )
				{
					//Mix_RewindMusic();
					if ( player.getPolarity() == BLACK )
					{
						double startPosition = ( musicTimer.get_ticks() / 1000 ) % 240;
						outputFile << fps.get_ticks() << " " << itos((int)startPosition) << std::endl;
						Mix_PlayMusic( ikarugaMusic, -1 );
						Mix_SetMusicPosition( startPosition );
					}
					else
					{
						double startPosition = ( musicTimer.get_ticks() / 1000 ) % 357;
						outputFile << fps.get_ticks() << " " << itos((int)startPosition) << std::endl;
						Mix_PlayMusic( katamariMusic, -1 );
						Mix_SetMusicPosition( startPosition );
						//Mix_SetMusicPosition( 60.0f );
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
			apply_surface( 100, 100, gameTitleText, screen );
			apply_surface( 100, 200, startGameText, screen );

			if ( previousScore >= 0 )
			{
				previousScoreText = TTF_RenderText_Solid( labelFont, (std::string("Previous Score: ") + itos(previousScore)).c_str(), livesTextColor );
				apply_surface(100, 400, previousScoreText, screen );
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

				multiplierText = TTF_RenderText_Solid( labelFont, (std::string("Multiplier: ") + itos(player.getMultiplier())).c_str(), blackTextColor );
				apply_surface( 420, 650, multiplierText, screen );

				livesText = TTF_RenderText_Solid( livesFont, itos(player.getLives()).c_str(), livesTextColor );
				apply_surface( 350, 155, livesText, screen );

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
			previousScore = player.getScore();
			player.reset();
			currentState = INIT;
		}

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;    
        }

		frame++;
		outputFile << itos(fps.get_ticks()) << std::endl;

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