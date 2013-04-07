#include "ResourceManager.h"
#include <utility>

using std::string;
using std::unordered_map;

typedef unordered_map< string, Image* >::iterator imageIter;
typedef unordered_map< string, Color* >::iterator colorIter;
typedef unordered_map< string, Font* >::iterator fontIter;
typedef unordered_map< string, Text* >::iterator textIter;
typedef unordered_map< string, Rect* >::iterator rectIter;


ResourceManager::~ResourceManager()
{
	for ( imageIter iter = images.begin(), end = images.end(); iter != end; iter++ )
		delete iter->second;

	for ( colorIter iter = colors.begin(), end = colors.end(); iter != end; iter++ )
		delete iter->second;

	for ( fontIter iter = fonts.begin(), end = fonts.end(); iter != end; iter++ )
		delete iter->second;

	for ( textIter iter = text.begin(), end = text.end(); iter != end; iter++ )
		delete iter->second;

	for ( rectIter iter = rects.begin(), end = rects.end(); iter != end; iter++ )
		delete iter->second;
}

// TODO: figure out how to template this (absurdly) redundant code
void ResourceManager::addImage( const string& id, const string& filePath )
{
	images[id] = new Image( filePath );
}

void ResourceManager::addColor( const string& id, int r, int g, int b )
{
	colors[id] = new Color( r, g, b );
}

void ResourceManager::addFont( const string& id, const string& filePath, int fontSize )
{
	fonts[id] = new Font( filePath, fontSize );
}

void ResourceManager::addText( const string& id, TTF_Font* font, const string& inputText, SDL_Color* color )
{
	text[id] = new Text( font, inputText, color );
}

void ResourceManager::addRect( const string& id, int x, int y, int w, int h )
{
	rects[id] = new Rect( x, y, w, h );
}


SDL_Surface* ResourceManager::getImage( const string& id ) 
{
	imageIter iter = images.find( id );
	return ( iter != images.end() ) ? iter->second->get() : NULL;
}

SDL_Color* ResourceManager::getColor( const string& id )
{
	colorIter iter = colors.find( id );
	return ( iter != colors.end() ) ? iter->second->get() : NULL;
}

TTF_Font* ResourceManager::getFont( const string& id )
{
	fontIter iter = fonts.find( id );
	return ( iter != fonts.end() ) ? iter->second->get() : NULL;
}

SDL_Surface* ResourceManager::getText( const string& id )
{
	textIter iter = text.find( id );
	return ( iter != text.end() ) ? iter->second->get() : NULL;
}

SDL_Rect* ResourceManager::getRect( const string& id )
{
	rectIter iter = rects.find( id );
	return ( iter != rects.end() ) ? iter->second->get() : NULL;
}



