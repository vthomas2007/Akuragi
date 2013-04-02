#include "Scene.h"
#include <list>
#include "SDL.h"
#include "GameObject.h"
#include "utility_functions.h"
typedef std::list< std::pair< SDL_Rect, SDL_Color > >::iterator rectIter;
typedef std::list< GameObject >::iterator goIter;

using namespace Akuragi::UtilFunctions;

void Scene::show( SDL_Surface* dest )
{
	// Draw each rectangle
	for ( rectIter iter = rects.begin(), end = rects.end(); iter != end; iter++ )
	{
		SDL_FillRect( dest, &iter->first, SDL_MapRGB( dest->format, iter->second.r, iter->second.g, iter->second.b ) );
	}

	// Draw each game object
	for ( goIter iter = gameObjects.begin(), end = gameObjects.end(); iter != end; iter++ )
	{
		apply_surface( (int)iter->getX(), (int)iter->getY(), iter->getImage(), dest );
	}
}

void Scene::addRect( SDL_Rect rect, SDL_Color color )
{
	rects.push_back( std::make_pair( rect, color ) );
}

void Scene::addRect( int x, int y, int w, int h, int r, int g, int b )
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_Color color = { r, g, b };

	addRect( rect, color );
}

void Scene::addGameObject( GameObject gameObject )
{
	gameObjects.push_back( gameObject );
}

void Scene::addGameObject( SDL_Surface* image, int x, int y )
{
	gameObjects.push_back( GameObject( image, (float)x, (float)y, 0, 0 ) );
}