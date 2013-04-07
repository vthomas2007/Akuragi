#pragma once

#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

class Music
{
public:
	Music();
	Music( const std::string& filePath );
	~Music();

	Mix_Music* get();
private:
	Mix_Music* music;
};