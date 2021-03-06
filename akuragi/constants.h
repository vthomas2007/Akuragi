#pragma once

#include <string>
#include "ResourceManager.h"

namespace Akuragi
{
	namespace Constants
	{
		const std::string GAME_TITLE("Akuragi");

		const int SCREEN_BPP = 32;
		const int SCREEN_WIDTH = 840;
		const int SCREEN_HEIGHT = 740;

		const int FRAMES_PER_SECOND = 60;

		const int PLAYABLE_WIDTH = 800;
		const int PLAYABLE_HEIGHT = 600;

		const int PLAYABLE_X_OFFSET = 20;
		const int PLAYABLE_Y_OFFSET = 20;

		const int LEFT_BORDER = PLAYABLE_X_OFFSET;
		const int RIGHT_BORDER = PLAYABLE_X_OFFSET + PLAYABLE_WIDTH;
		const int TOP_BORDER = PLAYABLE_Y_OFFSET;
		const int BOTTOM_BORDER = PLAYABLE_Y_OFFSET + PLAYABLE_HEIGHT;

		const int ABSORB_POINTS = 10;

		const int ENEMY_SPAWN_BUFFER = 100;

		const float PLAYER_SPEED = 10.0f;
		const int PLAYER_RESPAWN_TIME = 30;
		const int PLAYER_LIVES = 3; // TODO: Determine if these values should live in Player.h

		enum gameState { INIT, INSTRUCTIONS, ACTIVE, PAUSED, GAME_OVER };

		// Title screen stuff
		const int INSTRUCTIONS_BUTTON_X = 200;
		const int INSTRUCTIONS_BUTTON_WIDTH = 200;
		const int INSTRUCTIONS_BUTTON_Y = 550;
		const int INSTRUCTIONS_BUTTON_HEIGHT = 55;

		const int QUIT_BUTTON_X = 550;
		const int QUIT_BUTTON_WIDTH = 93;
		const int QUIT_BUTTON_Y = 550;
		const int QUIT_BUTTON_HEIGHT = 56;

		// Instructions stuff
		const int INSTRUCTIONS_HEADING_Y_OFFSET = 50;
		const int INSTRUCTIONS_NAVIGATION_Y_OFFSET = 600;
	}
}