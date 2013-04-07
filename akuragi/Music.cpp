#include "Music.h"
#include "SDL_mixer.h"

Music::Music()
	:music(NULL)
{
}

Music::Music( const std::string& filePath )
{
	music = Mix_LoadMUS( filePath.c_str() );
}

Music::~Music()
{
	Mix_FreeMusic( music );
}

Mix_Music* Music::get()
{
	return music;
}