#include "SceneDeck.h"

SceneDeck::SceneDeck()
	:currentScene(0), prevButton(NULL), nextButton(NULL), menuButton(NULL)
{
}

SceneDeck::SceneDeck( GameObject* nextButton, GameObject* menuButton, GameObject* prevButton )
	:currentScene(0), nextButton(nextButton), menuButton(menuButton), prevButton(prevButton)
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
}

void SceneDeck::show( SDL_Surface* dest )
{
	if ( scenes.size() > 0 && currentScene >= 0 && currentScene < scenes.size() )
	{
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
				// TODO: Figure out how to pass back a notification
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