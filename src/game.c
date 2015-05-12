#include <stdlib.h>
#include "game.h"
#include "graphics.h"
#include "sprites.h"
#include "mouse.h"
#include "audio.h"
#include "level.h"
#include "entity.h"
#include "player.h"
#include "boss.h"


extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;
extern Uint32 NOW;
extern TTF_Font *font;
Uint8 *keys;


static Level	*__level = NULL;
static Entity	*__player = NULL;
static Entity	*__boss = NULL;
int				levelTime = 0;
int				paused = 0;
int				gameOver = 0;


void Init_All();
void Draw();


/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main( int argc, char *argv[] )
{
	int done;
	int keyn;

	Init_All();

	GameStart( "def/Characters/Marisa/Marisa.txt", "def/Bosses/Flame_Empress/Flame_Empress.txt", "def/Levels/Flame_Hell.txt" );

	done = 0;
	do
	{
		ResetBuffer();

		keys = SDL_GetKeyState( &keyn );

		GameThink();

		Draw();

		if( !paused )
		{
			MoveEnts();
			UpdateEnts();
		}

		NextFrame();
		SDL_PumpEvents();
		
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
	TTF_CloseFont( font );
	
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


void GameStart( char *player, char *boss, char *level )
{
	GameClear();

	paused = 1;
	levelTime = NOW;

	__level = LoadLevel( level );

	__player = InitPlayer( player );
	__player->visible = 1;

	__boss = InitBoss( boss );
	__boss->visible = 1;
}


void GameClear()
{
	FreeEntList();
	__level = NULL;
	__player = NULL;
	__boss = NULL;
}


void GameThink()
{
	if( gameOver )
	{
		return;
	}

	if( NOW > levelTime + LEVEL_DELAY )
	{
		paused = 0;
	}
}
