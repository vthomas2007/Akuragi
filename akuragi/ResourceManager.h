#pragma once

#include "SDL.h"
#include <string>
#include <unordered_map>
#include "Image.h"

using std::string;
using std::unordered_map;

class ResourceManager
{
public:
	void addImage( string id, string filePath );

	SDL_Surface* getImage( const string& id ) const;
private:
	unordered_map< string, Image* > images;
};