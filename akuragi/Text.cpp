#include "Text.h"
#include "SDL_ttf.h"

Text::Text()
	:text(NULL)
{
}

Text::Text( TTF_Font* font, const std::string& inputText, SDL_Color* color )
{
	text = TTF_RenderText_Solid( font, inputText.c_str(), *(color) );
}

Text::~Text()
{
	SDL_FreeSurface( text );
}

SDL_Surface* Text::get()
{
	return text;
}