#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

class Font
{
public:
	Font();
	Font( const std::string& filePath, int fontSize );
	~Font();

	TTF_Font* get();

private:
	TTF_Font* font;
};