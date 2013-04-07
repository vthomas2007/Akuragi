#pragma once

#include "ResourceManager.h"
#include "constants.h"

using namespace Akuragi::Constants;

// I normally hate variable names like this, but I think it will ultimately lead to more readable
// code throughout the rest of the project
// TODO: At least put this in a namespace
ResourceManager rm;

void load_images()
{
	rm.addImage( "title-black", "title-black.png" );
	rm.addImage( "quit-white", "quit-white.png" );
	rm.addImage( "quit-black", "quit-black.png" );
	rm.addImage( "next-white", "next-white.png" );
	rm.addImage( "next-black", "next-black.png" );
	rm.addImage( "prev-white", "previous-white.png" );
	rm.addImage( "prev-black", "previous-black.png" );
	rm.addImage( "menu-white", "menu-white.png" );
	rm.addImage( "menu-black", "menu-black.png" );
	rm.addImage( "instructions-white", "instructions-white.png" );
	rm.addImage( "instructions-black", "instructions-black.png" );
	rm.addImage( "black-circle", "blank-black.png" );
	rm.addImage( "white-circle", "blank-white.png" );
	rm.addImage( "face", "happy-white.png" );
	rm.addImage( "bowling-ball", "bowling-ball-black.png" );
}

void load_colors()
{
	rm.addColor( "black", 0, 0, 0 );		// blackTextColor
	rm.addColor( "gray" , 200, 200, 200 );  // livesTextColor
	rm.addColor( "white", 255, 255, 255 );	// whiteTextColor
}

void load_fonts()
{
	rm.addFont( "huge"  , "Luhyouone.ttf", 192 ); // livesFont
	rm.addFont( "big"   , "Luhyouone.ttf", 72 );  // font
	rm.addFont( "medium", "Luhyouone.ttf", 48 );  // labelFont
	rm.addFont( "small" , "Luhyouone.ttf", 36 );  // instructionsFont
}

void load_text()
{
	rm.addText( "start-game", rm.getFont("big"), "Press 'Enter' to begin", rm.getColor( "black" ) ); // startGameText
	rm.addText( "paused", rm.getFont("medium"), "Press 'p' or 'enter' to resume", rm.getColor( "white" ) ); // pausedText

	rm.addText( "inst-controls-heading", rm.getFont("medium"), "Controls", rm.getColor( "black" ) ); // instControlsHeading
	rm.addText( "inst-arrow-keys", rm.getFont("small"), "Use the arrow keys to move", rm.getColor( "black" ) ); // instArrowKeys
	rm.addText( "inst-space", rm.getFont("small"), "Use the space bar to switch polarity", rm.getColor( "black" ) ); // instSpace

	rm.addText( "inst-polarity-heading", rm.getFont("medium"), "Polarity", rm.getColor( "black" ) ); // instPolarityHeading
	rm.addText( "inst-death-1", rm.getFont("small"), "Touching an enemy of opposite polarity will", rm.getColor( "black" ) ); // instDeath1
	rm.addText( "inst-death-2", rm.getFont("small"), "cause you to lose a life", rm.getColor( "black" ) ); // instDeath2
	rm.addText( "inst-absorb-1", rm.getFont("small"), "Touching an enemy of the same polarity will", rm.getColor( "black" ) ); // instAbsorb1
	rm.addText( "inst-absorb-2", rm.getFont("small"), "absorb it, giving you points and increasing", rm.getColor( "black" ) ); // instAbsorb2
	rm.addText( "inst-absorb-3", rm.getFont("small"), "your score multiplier", rm.getColor( "black" ) ); // instAbsorb3

	rm.addText( "inst-multiplier-heading", rm.getFont("medium"), "Score Multiplier", rm.getColor( "black" ) ); // instMultiplierHeading
	rm.addText( "inst-multiplier-1", rm.getFont("small"), "The bigger your score multiplier, the more", rm.getColor( "black" ) ); // instMultiplier1
	rm.addText( "inst-multiplier-2", rm.getFont("small"), "points you get from absorbing enemies", rm.getColor( "black" ) ); // instMultiplier2
	rm.addText( "inst-multiplier-reset-1", rm.getFont("small"), "However, switching polarity resets", rm.getColor( "black" ) ); // instMultiplierReset1
	rm.addText( "inst-multiplier-reset-2", rm.getFont("small"), "your score multiplier", rm.getColor( "black" ) ); // instMultiplierReset2

	rm.addText( "inst-tips-heading", rm.getFont("medium"), "Pro Tip", rm.getColor( "black" ) ); // instTipsHeading
	rm.addText( "inst-big-score-1", rm.getFont("small"), "To rack up a huge score, only switch", rm.getColor( "black" ) ); // instBigScore1
	rm.addText( "inst-big-score-2", rm.getFont("small"), "polarity when absolutely necessary!", rm.getColor( "black" ) ); // instBigScore2

}

void load_rects()
{
	//rm.addRect( "" ); // 
	rm.addRect( "title-background", 0, 160, SCREEN_WIDTH, 115 ); // titleBackground
	rm.addRect( "left-frame", 0, 0, PLAYABLE_X_OFFSET, SCREEN_HEIGHT ); // leftFrame
	rm.addRect( "right-frame", SCREEN_WIDTH - PLAYABLE_X_OFFSET, 0, PLAYABLE_X_OFFSET, SCREEN_HEIGHT ); // rightFrame
	rm.addRect( "top-frame", PLAYABLE_X_OFFSET, 0, PLAYABLE_WIDTH, PLAYABLE_Y_OFFSET ); // topFrame
	rm.addRect( "middle-frame", PLAYABLE_X_OFFSET, PLAYABLE_Y_OFFSET + PLAYABLE_HEIGHT, PLAYABLE_WIDTH, PLAYABLE_X_OFFSET ); // middleFrame
	rm.addRect( "bottom-frame", PLAYABLE_X_OFFSET, SCREEN_HEIGHT - PLAYABLE_Y_OFFSET, PLAYABLE_WIDTH, PLAYABLE_Y_OFFSET ); // bottomFrame
}

void load_files()
{
	load_images();
	load_colors();
	load_fonts();
	load_text();
	load_rects();
	//quitButton              = whiteQuitButton;
	//instructionsButton = whiteInstructionsButton;
}