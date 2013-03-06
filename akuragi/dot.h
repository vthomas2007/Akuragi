#include "SDL.h"

class Dot
{
	private:
		// The X and Y offsets
		int x, y;

		// The velocity of the dot
		int xVel, yVel;

		SDL_Surface* image;
	public:
		// Ctor
		Dot();

		void handle_input(const SDL_Event& event);
		void move();
		void show(SDL_Surface* dest);
};