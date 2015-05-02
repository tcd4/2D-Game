#include "mouse.h"


extern SDL_Surface *screen;


void InitMouse()
{
	Msprite = LoadSprite( "images/character1.png", 44, 40, 1 );
	if( Msprite == NULL )
	{
		fprintf( stdout,"mouse didn't load\n" );
	}

	Mouse.state = 0;
	Mouse.shown = 0;
	Mouse.frame = 0;
}


void DrawMouse()
{
	int mx, my;
	SDL_GetMouseState( &mx, &my );
	
	if( Msprite != NULL ) 
	{
		DrawSprite( Msprite, screen, mx, my, Mouse.frame );
	}
	Mouse.frame = ( Mouse.frame + 1 ) % 8;
	Mouse.x = mx;
	Mouse.y = my;
}
