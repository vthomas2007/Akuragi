#pragma once

#include "SDL.h"
#include <string>

class Image
{
public:
	Image();
	Image( const std::string& filePath );
	~Image();

	SDL_Surface* get() const;

private:
	SDL_Surface* image;
};