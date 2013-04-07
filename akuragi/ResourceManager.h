#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <unordered_map>
#include "Image.h"
#include "Color.h"
#include "Font.h"
#include "Text.h"
#include "Rect.h"



class ResourceManager
{
public:
	ResourceManager() {};
	~ResourceManager();

	void addImage( const std::string& id, const std::string &filePath );
	void addColor( const std::string& id, int r, int g, int b );
	void addFont(  const std::string& id, const std::string& filePath, int fontSize );
	void addText(  const std::string& id, TTF_Font* font, const std::string& inputText, SDL_Color* color );
	void addRect(  const std::string& id, int x, int y, int w, int h );

	// TODO: Figure out what can be const here (if anything)
	SDL_Surface* getImage( const std::string& id );
	SDL_Color*   getColor( const std::string& id );
	TTF_Font*    getFont ( const std::string& id );
	SDL_Surface* getText ( const std::string& id );
	SDL_Rect*	 getRect ( const std::string& id );

private:
	std::unordered_map< std::string, Image* > images;
	std::unordered_map< std::string, Color* > colors;
	std::unordered_map< std::string, Font* > fonts;
	std::unordered_map< std::string, Text* > text;
	std::unordered_map< std::string, Rect* > rects;
};
