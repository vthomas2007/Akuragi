#include "SceneDeck.h"

SceneDeck::SceneDeck()
	:currentScene(0), prevButton(NULL), nextButton(NULL), menuButton(NULL), exit(false)
{
}

SceneDeck::SceneDeck( GameObject* prevButton, GameObject* menuButton, GameObject* nextButton )
	:currentScene(0), prevButton(prevButton), menuButton(menuButton), nextButton(nextButton)
{
}

void SceneDeck::addScene( Scene scene )
{
	scenes.push_back( scene );
}

void SceneDeck::clear()
{
	scenes.clear();
}

void SceneDeck::forward()
{
	if ( currentScene < scenes.size() - 1 )
	{
		currentScene++;
	}
}

void SceneDeck::back()
{
	if ( currentScene > 0 )
	{
		currentScene--;
	}
}

void SceneDeck::reset()
{
	currentScene = 0;
	exit = false;
}

void SceneDeck::show( SDL_Surface* dest )
{
	if ( scenes.size() > 0 && currentScene >= 0 && currentScene < scenes.size() )
	{
		SDL_FillRect( dest, &dest->clip_rect, SDL_MapRGB( dest->format, 0xFF, 0xFF, 0xFF ) );
		scenes[currentScene].show( dest );
		showNavigation( dest );
	}
}

void SceneDeck::handle_input( SDL_Event& event )
{
	if ( event.type == SDL_MOUSEBUTTONDOWN )
	{
		if ( event.button.button = SDL_BUTTON_LEFT )
		{
			int x = event.button.x;
			int y = event.button.y;

			// Check if previous button was clicked
			if ( nextButton != NULL &&
				 x > (int)nextButton->getX() &&
				 x < (int)nextButton->getRightX() && 
				 y > (int)nextButton->getY() &&
				 y < (int)nextButton->getBottomY() )
			{
				forward();
			}
			// Check if next button was clicked
			else if ( prevButton != NULL &&
					  x > (int)prevButton->getX() &&
					  x < (int)prevButton->getRightX() && 
					  y > (int)prevButton->getY() &&
					  y < (int)prevButton->getBottomY() )
			{
				back();
			}
			// Check if menu button was clicked
			else if ( menuButton != NULL && 
					  x > (int)menuButton->getX() &&
					  x < (int)menuButton->getRightX() && 
					  y > (int)menuButton->getY() &&
					  y < (int)menuButton->getBottomY() )
			{
				exit = true;
			}

		}
	}
}

void SceneDeck::showNavigation( SDL_Surface* dest )
{
	if ( currentScene < scenes.size() - 1  && nextButton != NULL )
	{
		nextButton->show( dest );
	}
	if ( currentScene > 0 && prevButton != NULL )
	{
		prevButton->show( dest );
	}
	if ( menuButton != NULL )
	{
		menuButton->show( dest );
	}
}

bool SceneDeck::getExitStatus() const
{
	return exit;
}