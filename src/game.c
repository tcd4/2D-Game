#include <stdlib.h>
#include "graphics.h"
#include "sprites.h"
#include "mouse.h"
#include "audio.h"
#include "level.h"
#include "entity.h"
#include "player.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;
Uint8 *keys;

void Init_All();
void Draw();

/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main( int argc, char *argv[] )
{
	int done;
	int keyn;
	Level *level;
	Entity *player;

	Init_All();

	level = LoadLevel( "def/Levels/Flame_Hell.txt" );
	player = InitPlayer( "def/Characters/Marisa/Marisa.txt" );
	player->visible = 1;

	done = 0;
	do
	{
		ResetBuffer ();

		Draw();

		//UpdateEnts();

		NextFrame();
		SDL_PumpEvents();
		keys = SDL_GetKeyState( &keyn );
		if( keys[ SDLK_ESCAPE ] )
		{
			done = 1;
		}
	}while( !done );

	exit( 0 );
	return 0;
}


void Draw()
{
		DrawLevels();
		DrawEntList();
}


void CleanUpAll()
{
	FreeEntList();
	ClearSoundList();
	CloseLevelSystem();
	CloseSprites();
	
	fflush( stdout );
}


void Init_All()
{
	Init_Graphics();
	InitAudio();

	InitLevelSystem();
	InitEntitySystem();

	atexit( CleanUpAll );
}
