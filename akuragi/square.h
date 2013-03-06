#include "SDL.h"

const int SQUARE_WIDTH = 20;
const int SQUARE_HEIGHT = 20;

class Square {
private:
	SDL_Rect box;
	int xVel, yVel;

	SDL_Surface* image;

public:
	Square();
	Square(SDL_Surface*);

	void handle_input(const SDL_Event& event);
	void move();
	void show(SDL_Surface* dest);
};