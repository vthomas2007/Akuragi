#include "ResourceManager.h"

void ResourceManager::addImage( string id, string filePath )
{
	images[id] = new Image( filePath );
}

SDL_Surface* ResourceManager::getImage( const string& id ) const
{
	unordered_map< string, Image* >::const_iterator iter = images.find( id );
	return ( iter != images.end() ) ? iter->second->get() : NULL;
}