#pragma once

#include "SDL.h"
#include <string>

class Image
{
public:
	Image();
	Image( const std::string& filePath );
	~Image();

	SDL_Surface* get();

private:
	SDL_Surface* image;
};