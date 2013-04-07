#pragma once

#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

class Sound
{
public:
	Sound();
	Sound( const std::string& filePath );
	~Sound();
	Mix_Chunk* get();

private:
	Mix_Chunk* sound;
};