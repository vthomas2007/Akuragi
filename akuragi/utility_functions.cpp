#include "utility_functions.h"
#include "constants.h"
#include <cstdlib>
#include <ctime>

using namespace Akuragi::Constants;

typedef std::vector<GameObject>::const_iterator goIter;

namespace Akuragi
{
	namespace UtilFunctions
	{
		bool init()
		{
			// Initialize all SDL subsystems
			if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
			{
				return false;
			}

			// Initialize SDL_ttf
			if ( TTF_Init() == -1 )
			{
				return false;
			}

			// Set the window caption
			SDL_WM_SetCaption( GAME_TITLE.c_str(), NULL );

			// Seed the random number generator
			srand(time(NULL));

			return true;
		}

		SDL_Surface* init_screen()
		{
			return SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
		}

		void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
		{
			// Make a temporary rectangle to hold the offsets and assign their values
			SDL_Rect offset;
			offset.x = x;
			offset.y = y;

			// Blit the surface
			SDL_BlitSurface( source, clip, destination, &offset );
		}

		SDL_Surface *load_image( std::string filename )
		{

			// Temporary storage for the image that's loaded
			SDL_Surface* loadedImage = NULL;

			// The optimized image that will be used
			SDL_Surface* optimizedImage = NULL;

			// Load the image
			loadedImage = IMG_Load( filename.c_str() );

			if (loadedImage != NULL)
			{
				// Create an optimized image
				optimizedImage = SDL_DisplayFormat( loadedImage );

				// Free the old image
				SDL_FreeSurface( loadedImage );

				// If the image as successfully optimized
				if ( optimizedImage != NULL )
				{
					// Map the color key
					Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );

					// Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
					SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
				}

				return optimizedImage;
			}
		}

		/*
		bool load_files()
		{
			// Load the image
			square = load_image( "square.bmp" );

			font = TTF_OpenFont( "lazy.ttf", 36 );

			if ( square == NULL || font == NULL )
			{
				return false;
			}

			return true;
		}
		*/

		bool check_enemy_player_collisions( const std::vector<GameObject>& enemies, const GameObject& player )
		{
			for ( goIter iter = enemies.begin(), end = enemies.end(); iter != end; iter++ )
			{
				if ( check_circular_collision( *iter, player ) )
				{
					return true;
				}
			}

			return false;
		}

		bool check_circular_collision( const GameObject& A, const GameObject& B )
		{
			//Initialize temporary variables to calculate the "center" of the object... may need to revisit class structure later
			float AhalfWidth = (float)(A.getImage()->w / 2);
			float AhalfHeight = (float)(A.getImage()->h / 2);
			float BhalfWidth = (float)(B.getImage()->w / 2);
			float BhalfHeight = (float)(B.getImage()->h / 2);

			//Initialize input variables for x positions, y positions, and radii
			float Aradius = AhalfWidth; // TODO: Make this less fragile
			float Ax1 = A.getXOld() + AhalfWidth;
			float Ax2 = A.getX() + AhalfWidth;
			float Ay1 = A.getYOld() + AhalfHeight;
			float Ay2 = A.getY() + AhalfHeight;
			float Bradius = BhalfWidth;
			float Bx1 = B.getXOld() + BhalfWidth;
			float Bx2 = B.getX() + BhalfWidth;
			float By1 = B.getYOld() + BhalfHeight;
			float By2 = B.getY() + BhalfHeight;
			float combinedRadius = Aradius + Bradius;
			/*
			if (DEBUG) outputFile << std::endl;
			if (DEBUG) outputFile << "-----------------" << std::endl;
			if (DEBUG) outputFile << "Ax1: " << Ax1 << " Ay1: " << Ay1 << std::endl;
			if (DEBUG) outputFile << "Ax2: " << Ax2 << " Ay2: " << Ay2 << std::endl;
			if (DEBUG) outputFile << "Bx1: " << Bx1 << " By1: " << By1 << std::endl;
			if (DEBUG) outputFile << "Bx2: " << Bx2 << " By2: " << By2 << std::endl;
			if (DEBUG) outputFile << "Aradius: " << Aradius << " Bradius: " << Bradius << std::endl;
			*/
			//Calculate x/y velocities (note: MAY be able to use the properties of the circles, but if it changes directions
			//at the wrong time in the main loop this WILL screw up )
			float AxVel = Ax2 - Ax1;
			float AyVel = Ay2 - Ay1;
			float BxVel = Bx2 - Bx1;
			float ByVel = By2 - By1;

			//if (DEBUG) outputFile << "AxVel: " << AxVel << " AyVel " << AyVel << std::endl;
			//if (DEBUG) outputFile << "BxVel: " << BxVel << " ByVel " << ByVel << std::endl;

			//Calculate the relative velocity vector
			float RelX = BxVel - AxVel;
			float RelY = ByVel - AyVel;

			//if (DEBUG) outputFile << "RelX: " << RelX << " RelY " << RelY << std::endl;

			//Calculate the vector from B to A
			float BAx = Bx1 - Ax1;
			float BAy = By1 - Ay1;

			//if (DEBUG) outputFile << "BAx " << BAx << " BAy " << BAy << std::endl;

			//Project BA onto Rel: Magnitude = (Rel dot BA) / |Rel|^2 )
			float numerator = ( RelX * BAx ) + ( RelY * BAy );
			float denominator = ( RelX * RelX ) + ( RelY * RelY );
			float magnitude;
			if ( denominator != 0  )
			{
				magnitude = ( numerator / denominator );
			}
			else
			{
				//report an error (not really, I think this just means there was no relative motion
				//and should be handled well before this point
				return 0;
			}

			//if (DEBUG) outputFile << "numerator: " << numerator << std::endl;
			//if (DEBUG) outputFile << "denominator: " << denominator << std::endl;
			//if (DEBUG) outputFile << "magnitude: " << magnitude << std::endl;

			//Project BA onto Rel: magnitude * Rel
			float projX = magnitude * RelX;
			float projY = magnitude * RelY;

			//if (DEBUG) outputFile << "projX: " << projX << " projY: " << projY << std::endl;

			//To calculate closestX and closestY, take CLAMP cases into consideration to calc closestX and closestY
			//(also, keeping it readable now but... optimize the shit out of this)
			//(consider Steve's suggestion)
			//Dear God, please forgive me for ever writing this code
			float closestX;
			float closestY;
			//int BxRel = Bx1 + RelX;
			//int ByRel = By1 + RelY;

			if ( RelX > 0.0f )
			{
				if ( projX >= RelX )
				{
					closestX = RelX;
					closestY = RelY;
				}
				else if ( projX <= 0.0f )
				{
					closestX = 0.0f;
					closestY = 0.0f;
				}
				else
				{
					closestX = projX;
					closestY = projY;
				}
			}
			else if ( RelX < 0.0f )
			{
				if ( projX <= RelX )
				{
					closestX = RelX;
					closestY = RelY;
				}
				else if ( projX >= 0.0f )
				{
					closestX = 0.0f;
					closestY = 0.0f;
				}
				else
				{
					closestX = projX;
					closestY = projY;
				}
			}
			else
			{
				if ( RelY > 0.0f )
				{
					if ( projY >= RelY )
					{
						closestX = RelX;
						closestY = RelY;
					}
					else if ( projY <= 0.0f )
					{
						closestX = 0.0f;
						closestY = 0.0f;
					}
					else
					{
						closestX = projX;
						closestY = projY;
					}
				}
				else if ( RelY < 0 )
				{
					if ( projY <= RelY )
					{
						closestX = RelX;
						closestY = RelY;
					}
					else if ( projY >= 0 )
					{
						closestX = 0.0f;
						closestY = 0.0f;
					}
					else
					{
						closestX = projX;
						closestY = projY;
					}
				}
				//"no motion" case... doesn't really matter what you do (?)
				else
				{
					closestX = projX;
					closestY = projY;
				}
			}

			//if (DEBUG) outputFile << "closestX: " << closestX << " closestY: " << closestY << std::endl;
			//if (DEBUG) outputFile << "( BAx - closestX ): " << (BAx - closestX) << " ( BAy - closestY ): " << (BAy - closestY) << std::endl;

			//Calculate the distance squared (since this is just going to be used relative to radii squared
			float distanceSquared = ( BAx - closestX )*( BAx - closestX ) + ( BAy - closestY )*( BAy - closestY );    //could probably optimize easily

			//Square the combined radius
			float combinedRadiusSquared = combinedRadius * combinedRadius;

			//if (DEBUG) outputFile << "Comparing " << combinedRadiusSquared << " to " << distanceSquared << std::endl;

			//Finally, compared the distances to determine if there is a collision
			if ( distanceSquared <= combinedRadiusSquared )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool check_collision( SDL_Rect A, SDL_Rect B )
		{
			// The sides of the rectangles
			int leftA, leftB;
			int rightA, rightB;
			int topA, topB;
			int bottomA, bottomB;

			// Calculate the sides of rect A
			leftA = A.x;
			rightA = A.x + A.w;
			topA = A.y;
			bottomA = A.y + A.h;

			// Calculate the sides of rect B
			leftB = B.x;
			rightB = B.x + B.w;
			topB = B.y;
			bottomB = B.y + B.h;

			if ( bottomA <= topB || topA >= bottomB )
			{
				return false;
			}

			if ( rightA <= leftB || leftA >= rightB )
			{
				return false;
			}

			return true;
		}
		/*
		void clean_up()
		{
			SDL_FreeSurface( square );

			TTF_CloseFont( font );
			TTF_Quit();

			SDL_Quit();
		}
		*/
	}
}