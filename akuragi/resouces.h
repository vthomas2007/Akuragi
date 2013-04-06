#pragma once

#include "ResourceManager.h"

ResourceManager resourceManager;

void load_files()
{
	resourceManager.addImage( "title-black", "title-black.png" );
}