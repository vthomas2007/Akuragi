#include <string>

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

		enum gameState { INIT, ACTIVE, PAUSED, GAME_OVER };
	}
}