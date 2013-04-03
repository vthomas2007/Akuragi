#pragma once
#include "Scene.h"
#include "SDL.h"
#include <vector>
#include "GameObject.h"

class SceneDeck
{
public:
	SceneDeck();
	SceneDeck( GameObject* prevButton, GameObject* menuButton, GameObject* nextButton );

	void addScene( Scene scene );
	void clear();
	void forward();
	void back();
	void reset();
	void show( SDL_Surface* dest );
	void handle_input( SDL_Event& event );
	bool getExitStatus() const;

private:
	std::vector< Scene > scenes;
	// TODO: Figure out if this should be unsigned or not to handle comparisons with scene.size()
	int currentScene;
	GameObject* nextButton;
	GameObject* menuButton;
	GameObject* prevButton;

	bool exit;

	void showNavigation( SDL_Surface* dest );
};