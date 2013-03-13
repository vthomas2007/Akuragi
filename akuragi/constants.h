#include <string>

namespace Akuragi
{
	namespace Constants
	{
		const std::string GAME_TITLE("Akuragi");

		const int SCREEN_WIDTH = 640;
		const int SCREEN_HEIGHT = 480;
		const int SCREEN_BPP = 32;

		const int FRAMES_PER_SECOND = 60;

		const int ENEMY_SPAWN_BUFFER = 100;

		const float PLAYER_SPEED = 10.0f;
		const int PLAYER_RESPAWN_TIME = 30;
		const int PLAYER_LIVES = 3; // TODO: Determine if these values should live in Player.h

		enum gameState { INIT, ACTIVE, PAUSED, GAME_OVER };
	}
}