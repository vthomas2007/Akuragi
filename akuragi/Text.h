#pragma once

#include "SDL.h"
#include "Font.h"
#include "Color.h"
#include <string>

class Text
{
public:
	Text();
	// Todo: Probably want a ctor that just takes const char* instead of const std::string&
	Text( TTF_Font* font, const std::string& inputText, SDL_Color* color );
	~Text();

	SDL_Surface* get();

private:
	SDL_Surface* text;
};