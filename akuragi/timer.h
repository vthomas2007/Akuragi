#include "SDL.h"

class Timer
{
private:
	// The clock time when the timer started
	int startTicks;

	// The ticks stored when the timer is paused
	int pausedTicks;

	// The timer status
	bool paused;
	bool started;

public:
	Timer();

	// Clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	// Get the timer's time
	int get_ticks();

	// Check the status of the timer
	bool is_started();
	bool is_paused();
};