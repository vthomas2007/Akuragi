#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(SDL_Surface* whiteImage, SDL_Surface* blackImage, float x, float y, float xVel, float yVel);
	virtual ~Player() { };

	// TODO: Create a subclass for player objects and move these functions into it
	void handle_input( const SDL_Event& event );
	SDL_Rect getSpawnBuffer() const;
	void togglePolarity();

private:
	SDL_Surface* whiteImage;
	SDL_Surface* blackImage;
};