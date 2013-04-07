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

// For the love of god create a resource manager
/*
SDL_Surface *square = NULL;
SDL_Surface *blackCircle = NULL;
SDL_Surface *whiteCircle = NULL;
SDL_Surface *face = NULL;
SDL_Surface *bowlingBall = NULL;
SDL_Surface *gameTitle = NULL;
SDL_Surface *startGameText = NULL;
SDL_Surface *pausedText = NULL;
SDL_Surface *blackInstructionsButton = NULL;
SDL_Surface *whiteInstructionsButton = NULL;
SDL_Surface *blackQuitButton = NULL;
SDL_Surface *whiteQuitButton = NULL;
SDL_Surface *blackNextButton = NULL;
SDL_Surface *whiteNextButton = NULL;
SDL_Surface *blackPrevButton = NULL;
SDL_Surface *whitePrevButton = NULL;
SDL_Surface *blackMenuButton = NULL;
SDL_Surface *whiteMenuButton = NULL;

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

TTF_Font *font = NULL;

SDL_Color blackTextColor = { 0, 0, 0 };
SDL_Color livesTextColor = { 200, 200, 200 };
SDL_Color whiteTextColor = { 255, 255, 255 };

*/

SDL_Event event;

int main(int arg, char** argv)
{

	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
	
	//Mix_Music* katamariMusic = Mix_LoadMUS( "music1.ogg" );
	//Mix_Music* ikarugaMusic = Mix_LoadMUS( "music2.ogg" );
//	Mix_PlayMusic( katamariMusic, -1 );
	Mix_VolumeMusic( 0 );

	//Mix_Chunk *gameOverSound = Mix_LoadWAV( "gameover.wav" );
	//Mix_Chunk *deadSound = Mix_LoadWAV( "dead.wav" );

	SDL_Surface *instructionsButton = NULL;
	SDL_Surface *quitButton = NULL;

	bool quit = false;

	if ( init() == false )
	{
		return 1;
	}

	screen = init_screen();

	// This is where "load files" used to be
	load_files();
	// TODO: Offload all of this to a resource manager
	//square = load_image( "square.bmp" );
	
	//font = TTF_OpenFont( "Luhyouone.ttf", 72 );
	//TTF_Font* labelFont = TTF_OpenFont( "Luhyouone.ttf", 48 );
	//TTF_Font* instructionsFont = TTF_OpenFont( "Luhyouone.ttf", 36 );
	//TTF_Font* livesFont = TTF_OpenFont( "Luhyouone.ttf", 192 );
	//gameTitle = TTF_RenderText_Solid( font, "Akuragi", blackTextColor );
	//startGameText = TTF_RenderText_Solid( font, "Press 'Enter' to begin", blackTextColor );
	//startGameText = TTF_RenderText_Solid( font, itos( gameTitleText->h ).c_str() , blackTextColor );
	//pausedText = TTF_RenderText_Solid( labelFont, "Press 'p' or 'enter' to resume", whiteTextColor );
	
	/*
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
	*/
	Uint32 frameColor = SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 );

	// FOR THE LOVE OF GOD WRITE A RESOURCE MANAGER PLEASE
	GameObject* nextButton = new GameObject( rm.getImage( "next-white" ), 500.0f, (float)INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0.0f, 0.0f );
	GameObject* menuButton = new GameObject( rm.getImage( "menu-white" ), 400.0f, (float)INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0.0f, 0.0f );
	GameObject* prevButton = new GameObject( rm.getImage( "prev-white" ), 250.0f, (float)INSTRUCTIONS_NAVIGATION_Y_OFFSET, 0.0f, 0.0f );
	SceneDeck instructions( prevButton, menuButton, nextButton );

	Scene controlsScene;
	controlsScene.addRect( rm.getRect("bottom-frame"), rm.getColor("black") );
	controlsScene.addRect( rm.getRect("top-frame"), rm.getColor("black") );
	controlsScene.addRect( rm.getRect("left-frame"), rm.getColor("black") );
	controlsScene.addRect( rm.getRect("right-frame"), rm.getColor("black") );
	//instControlsHeading = TTF_RenderText_Solid( labelFont, "Controls", blackTextColor );
	//instArrowKeys = TTF_RenderText_Solid( instructionsFont, "Use the arrow keys to move", blackTextColor );
	//instSpace = TTF_RenderText_Solid( instructionsFont, "Use the space bar to switch polarity", blackTextColor );
	// After all that refactoring and... now I'm calling a map lookup twice per line
	controlsScene.addGameObject( rm.getText("inst-controls-heading"), (SCREEN_WIDTH / 2) - (rm.getText("inst-controls-heading")->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	controlsScene.addGameObject( rm.getText("inst-arrow-keys"),	(SCREEN_WIDTH / 2) - (rm.getText("inst-arrow-keys")->w / 2), 250 );
	controlsScene.addGameObject( rm.getText("inst-space"), (SCREEN_WIDTH / 2) - (rm.getText("inst-space")->w / 2), 350);
	instructions.addScene( controlsScene );

	Scene polarityScene;
	polarityScene.addRect( rm.getRect("bottom-frame"), rm.getColor("black") );
	polarityScene.addRect( rm.getRect("top-frame"), rm.getColor("black") );
	polarityScene.addRect( rm.getRect("left-frame"), rm.getColor("black") );
	polarityScene.addRect( rm.getRect("right-frame"), rm.getColor("black") );
	// instPolarityHeading = TTF_RenderText_Solid( labelFont, "Polarity", blackTextColor );
	// instDeath1  = TTF_RenderText_Solid( instructionsFont, "Touching an enemy of opposite polarity will", blackTextColor );
	// instDeath2  = TTF_RenderText_Solid( instructionsFont, "cause you to lose a life", blackTextColor );
	// instAbsorb1 = TTF_RenderText_Solid( instructionsFont, "Touching an emey of the same polarity will", blackTextColor );
	// instAbsorb2 = TTF_RenderText_Solid( instructionsFont, "absorb it, give you points, and increase", blackTextColor );
	// instAbsorb3 = TTF_RenderText_Solid( instructionsFont, " your score multiplier", blackTextColor );
	polarityScene.addGameObject( rm.getText("inst-controls-heading"), (SCREEN_WIDTH / 2) - (rm.getText("inst-controls-heading")->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	polarityScene.addGameObject( rm.getText("inst-death-1"), (SCREEN_WIDTH / 2) - (rm.getText("inst-death-1")->w / 2), 200 );
	polarityScene.addGameObject( rm.getText("inst-death-2"), (SCREEN_WIDTH / 2) - (rm.getText("inst-death-2")->w / 2), 250 );
	polarityScene.addGameObject( rm.getText("inst-absorb-1"), (SCREEN_WIDTH / 2) - (rm.getText("inst-absorb-1")->w / 2), 350 );
	polarityScene.addGameObject( rm.getText("inst-absorb-2"), (SCREEN_WIDTH / 2) - (rm.getText("inst-absorb-2")->w / 2), 400 );
	polarityScene.addGameObject( rm.getText("inst-absorb-3"), (SCREEN_WIDTH / 2) - (rm.getText("inst-absorb-3")->w / 2), 450 );
	instructions.addScene( polarityScene );

	Scene multiplierScene;
	multiplierScene.addRect( rm.getRect("bottom-frame"), rm.getColor("black") );
	multiplierScene.addRect( rm.getRect("top-frame"), rm.getColor("black") );
	multiplierScene.addRect( rm.getRect("left-frame"), rm.getColor("black") );
	multiplierScene.addRect( rm.getRect("right-frame"), rm.getColor("black") );
	// instMultiplierHeading	= TTF_RenderText_Solid( labelFont, "Score Multiplier", blackTextColor );
	// instMultiplier1			= TTF_RenderText_Solid( instructionsFont, "The bigger your score multiplier, the more", blackTextColor );
	// instMultiplier2			= TTF_RenderText_Solid( instructionsFont, "points you get from absorbing enemies", blackTextColor );
	// instMultiplierReset1	= TTF_RenderText_Solid( instructionsFont, "However, switching polarity resets", blackTextColor );
	// instMultiplierReset2	= TTF_RenderText_Solid( instructionsFont, "your score multiplier", blackTextColor );
	multiplierScene.addGameObject( rm.getText("inst-multiplier-heading"), (SCREEN_WIDTH / 2) - (rm.getText("inst-multiplier-heading")->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	multiplierScene.addGameObject( rm.getText("inst-multiplier-1"), (SCREEN_WIDTH / 2) - (rm.getText("inst-multiplier-1")->w / 2), 200 );
	multiplierScene.addGameObject( rm.getText("inst-multiplier-2"), (SCREEN_WIDTH / 2) - (rm.getText("inst-multiplier-2")->w / 2), 250 );
	multiplierScene.addGameObject( rm.getText("inst-multiplier-reset-1"), (SCREEN_WIDTH / 2) - (rm.getText("inst-multiplier-reset-1")->w / 2), 400 );
	multiplierScene.addGameObject( rm.getText("inst-multiplier-reset-2"), (SCREEN_WIDTH / 2) - (rm.getText("inst-multiplier-reset-2")->w / 2), 450 );
	instructions.addScene( multiplierScene );

	Scene tipsScene;
	tipsScene.addRect( rm.getRect("bottom-frame"), rm.getColor("black") );
	tipsScene.addRect( rm.getRect("top-frame"), rm.getColor("black") );
	tipsScene.addRect( rm.getRect("left-frame"), rm.getColor("black") );
	tipsScene.addRect( rm.getRect("right-frame"), rm.getColor("black") );
	// instTipsHeading = TTF_RenderText_Solid( labelFont, "Pro Tip", blackTextColor );
	// instBigScore1 = TTF_RenderText_Solid( instructionsFont, "To rack up a huge score, only switch", blackTextColor );
	// instBigScore2 = TTF_RenderText_Solid( instructionsFont, "polarity when absolutely necessary!", blackTextColor );
	tipsScene.addGameObject( rm.getText("inst-tips-heading"), (SCREEN_WIDTH / 2) - (rm.getText("inst-tips-heading")->w / 2), INSTRUCTIONS_HEADING_Y_OFFSET );
	tipsScene.addGameObject( rm.getText("inst-big-score-1"), (SCREEN_WIDTH / 2) - (rm.getText("inst-big-score-1")->w / 2), 300 );
	tipsScene.addGameObject( rm.getText("inst-big-score-2"), (SCREEN_WIDTH / 2) - (rm.getText("inst-big-score-2")->w / 2), 350 );
	instructions.addScene( tipsScene );

	// END TODO

	// Todo: Add checks to ensure resources are present
	//if ( square == NULL || blackCircle == NULL || whiteCircle == NULL || font == NULL )

	int previousScore = -1;
	gameState currentState = INIT;

	Player player( rm.getImage( "face" ), rm.getImage( "bowling-ball" ) );
	EnemyManager enemyManager( rm.getImage( "white-circle" ), rm.getImage( "black-circle" ), screen );

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
//						Mix_PlayMusic( ikarugaMusic, -1 );
						Mix_SetMusicPosition( startPosition );
					}
					else
					{
						double startPosition = ( musicTimer.get_ticks() / 1000 ) % 357;
//						Mix_PlayMusic( katamariMusic, -1 );
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
			//apply_surface( 200, 50, gameTitle, screen );
			apply_surface( 50, 375, rm.getText( "start-game" ), screen );
			apply_surface( INSTRUCTIONS_BUTTON_X, INSTRUCTIONS_BUTTON_Y, instructionsButton, screen );
			apply_surface( QUIT_BUTTON_X, QUIT_BUTTON_Y, quitButton, screen );

			if ( previousScore >= 0 )
			{
				//SDL_Surface* previousScoreText = TTF_RenderText_Solid( rm.getFont("medium"), (std::string("Previous Score: ") + itos(previousScore)).c_str(), rm.getColor("gray") );
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
//						Mix_PlayChannel( -1, deadSound, 0 );
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