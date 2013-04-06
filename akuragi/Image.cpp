#include "Image.h"

#include "utility_functions.h"

Image::Image()
	:image(NULL)
{
}

Image::Image( const std::string& filePath )
{
	image = Akuragi::UtilFunctions::load_image( filePath );
}

Image::~Image()
{
	SDL_FreeSurface( image );
}

SDL_Surface* Image::get() const
{
	return image;
}