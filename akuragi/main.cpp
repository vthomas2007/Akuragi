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
SDL_Surface *blackNextButton = NULL;
SDL_Surface *whiteNextButton = NULL;
SDL_Surface *blackPrevButton = NULL;
SDL_Surface *whitePrevButton = NULL;
SDL_Surface *blackMenuButton = NULL;
SDL_Surface *whiteMenuButton = NULL;
SDL_Surface *quitButton = NULL;

// Instructions Surfaces
SDL_Surface *instControlsHeading;
SDL_Surface *instArrowKeys;
SDL_Surface *instSpace;

SDL_Surface *instPolarityHeading;
SDL_Surface *instDeath1;
SDL_Surface *instDeath2;
SDL_Surface *instAbsorb1;
SDL_Surface *instAbsorb2;
SDL_Surface *instAbsorb3;

SDL_Surface *instMultiplierHeading;
SDL_Surface *instMultiplier1;
SDL_Surface *instMultiplier2;
SDL_Surface *instMultiplierReset1;
SDL_Surface *instMultiplierReset2;

SDL_Surface *instTipsHeading;
SDL_Surface *instBigScore1;
SDL_Surface *instBigScore2;
SDL_Surface *instCautionTip1;
SDL_Surface *instCautionTip2;

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
	whiteNextButton = load_image( "next-white.png" );
	blackNextButton = load_image( "next-black.png" );
	whitePrevButton = load_image( "previous-white.png" );
	blackPrevButton = load_image( "previous-black.png" );
	whiteMenuButton = load_image( "menu-white.png" );
	blackMenuButton = load_image( "menu-black.png" );
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
	TTF_Font* instructionsFont = TTF_OpenFont( "Luhyouone.ttf", 36 );
	TTF_Font* livesFont = TTF_OpenFont( "Luhyouone.ttf", 192 );
	//gameTitle = TTF_RenderText_Solid( font, "Akuragi", blackTextColor );
	startGameText = TTF_RenderText_Solid( font, "Press 'Enter' to begin", blackTextColor );
	//startGameText = TTF_RenderText_Solid( font, itos( gameTitleText->h ).c_str() , blackTextColor );
	pausedText = TTF_RenderText_Solid( labelFont, "Press 'p' or 'enter' to resume", whiteTextColor );
	SDL_Surface* scoreText = NULL;
	SDL_Surface* multiplierText = NULL;
	SDL_Surface* livesText = NULL;
	SDL_Surface* previousScoreText = NULL;

	SDL_Rect titleBackground;
	titleBackground.x = 0;
	titleBackground.y = 160;
	titleBackground.w = SCREEN_WIDTH;
	titleBackground.h = 115;

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

	// FOR THE LOVE OF GOD WRITE A RESOURCE MANAGER PLEASE
	GameObject* nextButton = new GameObject( whiteNextButton, 500, INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0, 0 );
	GameObject* menuButton = new GameObject( whiteMenuButton, 400, INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0, 0 );
	GameObject* prevButton = new GameObject( whitePrevButton, 250, INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0, 0 );
	SceneDeck instructions( prevButton, menuButton, nextButton );

	Scene controlsScene;
	controlsScene.addRect( bottomFrame, blackTextColor );
	controlsScene.addRect( topFrame, blackTextColor );
	controlsScene.addRect( rightFrame, blackTextColor );
	controlsScene.addRect( leftFrame, blackTextColor );
	instControlsHeading = TTF_RenderText_Solid( labelFont, "Controls", blackTextColor );
	instArrowKeys = TTF_RenderText_Solid( instructionsFont, "Use the arrow keys to move", blackTextColor );
	instSpace = TTF_RenderText_Solid( instructionsFont, "Use the space bar to switch polarity", blackTextColor );
	controlsScene.addGameObject( instControlsHeading, (SCREEN_WIDTH / 2) - (instControlsHeading->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	controlsScene.addGameObject( instArrowKeys, (SCREEN_WIDTH / 2) - (instArrowKeys->w / 2), 250 );
	controlsScene.addGameObject( instSpace, (SCREEN_WIDTH / 2) - (instSpace->w / 2), 350);
	instructions.addScene( controlsScene );

	Scene polarityScene;
	polarityScene.addRect( bottomFrame, blackTextColor );
	polarityScene.addRect( topFrame, blackTextColor );
	polarityScene.addRect( rightFrame, blackTextColor );
	polarityScene.addRect( leftFrame, blackTextColor );
	instPolarityHeading = TTF_RenderText_Solid( labelFont, "Polarity", blackTextColor );
	instDeath1 = TTF_RenderText_Solid( instructionsFont, "Touching an enemy of opposite polarity will", blackTextColor );
	instDeath2 = TTF_RenderText_Solid( instructionsFont, "cause you to lose a life", blackTextColor );
	instAbsorb1 = TTF_RenderText_Solid( instructionsFont, "Touching an emey of the same polarity will", blackTextColor );
	instAbsorb2 = TTF_RenderText_Solid( instructionsFont, "absorb it, give you points, and increase", blackTextColor );
	instAbsorb3 = TTF_RenderText_Solid( instructionsFont, " your score multiplier", blackTextColor );
	polarityScene.addGameObject( instPolarityHeading, (SCREEN_WIDTH / 2) - (instPolarityHeading->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	polarityScene.addGameObject( instDeath1, (SCREEN_WIDTH / 2) - (instDeath1->w / 2), 200 );
	polarityScene.addGameObject( instDeath2, (SCREEN_WIDTH / 2) - (instDeath2->w / 2), 250 );
	polarityScene.addGameObject( instAbsorb1, (SCREEN_WIDTH / 2) - (instAbsorb1->w / 2), 350 );
	polarityScene.addGameObject( instAbsorb2, (SCREEN_WIDTH / 2) - (instAbsorb2->w / 2), 400 );
	polarityScene.addGameObject( instAbsorb3, (SCREEN_WIDTH / 2) - (instAbsorb3->w / 2), 450 );
	instructions.addScene( polarityScene );

	Scene multiplierScene;
	multiplierScene.addRect( bottomFrame, blackTextColor );
	multiplierScene.addRect( topFrame, blackTextColor );
	multiplierScene.addRect( rightFrame, blackTextColor );
	multiplierScene.addRect( leftFrame, blackTextColor );
	instMultiplierHeading = TTF_RenderText_Solid( labelFont, "Score Multiplier", blackTextColor );
	instMultiplier1 = TTF_RenderText_Solid( instructionsFont, "The bigger your score multiplier, the more", blackTextColor );
	instMultiplier2 = TTF_RenderText_Solid( instructionsFont, "points you get from absorbing enemies", blackTextColor );
	instMultiplierReset1 = TTF_RenderText_Solid( instructionsFont, "However, switching polarity resets", blackTextColor );
	instMultiplierReset2 = TTF_RenderText_Solid( instructionsFont, "your score multiplier", blackTextColor );
	multiplierScene.addGameObject( instMultiplierHeading, (SCREEN_WIDTH / 2) - (instMultiplierHeading->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	multiplierScene.addGameObject( instMultiplier1, (SCREEN_WIDTH / 2) - (instMultiplier1->w / 2), 200 );
	multiplierScene.addGameObject( instMultiplier2, (SCREEN_WIDTH / 2) - (instMultiplier2->w / 2), 250 );
	multiplierScene.addGameObject( instMultiplierReset1, (SCREEN_WIDTH / 2) - (instMultiplierReset1->w / 2), 400 );
	multiplierScene.addGameObject( instMultiplierReset2, (SCREEN_WIDTH / 2) - (instMultiplierReset2->w / 2), 450 );
	instructions.addScene( multiplierScene );

	Scene tipsScene;
	tipsScene.addRect( bottomFrame, blackTextColor );
	tipsScene.addRect( topFrame, blackTextColor );
	tipsScene.addRect( rightFrame, blackTextColor );
	tipsScene.addRect( leftFrame, blackTextColor );
	instTipsHeading = TTF_RenderText_Solid( labelFont, "Pro Tip", blackTextColor );
	instBigScore1 = TTF_RenderText_Solid( instructionsFont, "To rack up a huge score, only switch", blackTextColor );
	instBigScore2 = TTF_RenderText_Solid( instructionsFont, "polarity when absolutely necessary!", blackTextColor );
	tipsScene.addGameObject( instTipsHeading, (SCREEN_WIDTH / 2) - (instTipsHeading->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	tipsScene.addGameObject( instBigScore1, (SCREEN_WIDTH / 2) - (instBigScore1->w / 2), 300 );
	tipsScene.addGameObject( instBigScore2, (SCREEN_WIDTH / 2) - (instBigScore2->w / 2), 350 );
	instructions.addScene( tipsScene );

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
							instructionsButton = blackInstructionsButton;
						else if ( event.type == SDL_MOUSEBUTTONDOWN )
						{
							currentState = INSTRUCTIONS;
							instructionsButton = whiteInstructionsButton;
						}
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
						if ( event.type == SDL_MOUSEMOTION )
							quitButton = blackQuitButton;
						else if ( event.type == SDL_MOUSEBUTTONDOWN )
							quit = true;
					}
					else
					{
						quitButton = whiteQuitButton;
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
			SDL_FillRect( screen, &titleBackground, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
			apply_surface( 200, 50, gameTitle, screen );
			apply_surface( 50, 375, startGameText, screen );
			apply_surface( INSTRUCTIONS_BUTTON_X, INSTRUCTIONS_BUTTON_Y, instructionsButton, screen );
			apply_surface( QUIT_BUTTON_X, QUIT_BUTTON_Y, quitButton, screen );

			if ( previousScore >= 0 )
			{
				previousScoreText = TTF_RenderText_Solid( labelFont, (std::string("Previous Score: ") + itos(previousScore)).c_str(), livesTextColor );
				apply_surface((SCREEN_WIDTH / 2) - (previousScoreText->w / 2), 650, previousScoreText, screen );
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
	delete nextButton;
	delete menuButton;
	delete prevButton;

	// TODO: WRITE A RESOURCE MANAGER AND FREE ALL YOUR LOOSE SURFACES
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