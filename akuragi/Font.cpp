#include "Font.h"

Font::Font()
	:font(NULL)
{
}

Font::Font( const std::string& filePath, int fontSize )
{
	font = TTF_OpenFont( filePath.c_str(), fontSize );
}

Font::~Font()
{
	TTF_CloseFont( font );
}

TTF_Font* Font::get()
{
	return font;
}