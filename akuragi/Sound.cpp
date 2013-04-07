#include "Sound.h"
#include "SDL_mixer.h"

Sound::Sound()
	:sound(NULL)
{
}

Sound::Sound( const std::string& filePath )
{
	sound = Mix_LoadWAV( filePath.c_str() );
}

Sound::~Sound()
{
	Mix_FreeChunk( sound );
}

Mix_Chunk* Sound::get()
{
	return sound;
}