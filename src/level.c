#include "level.h"
#include <string.h>
#include <stdio.h>


static Level __level;
extern SDL_Surface *screen;


void LoadLevel( char *filename )
{
	FILE *levelfile = NULL;
	char buf[ 128 ];
	char bgimagepath[ 128 ];
	int w, h;
	Sprite *temp;

	levelfile = fopen( filename, "r" );
	if( levelfile == NULL )
	{
		fprintf( stderr, "LoadLevel: FATAL: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( levelfile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), levelfile );
		}
		else if( strncmp( buf, "sprite:", 128 ) == 0 )
		{
			fscanf( levelfile, "%s", bgimagepath );
		}
		else if( strncmp( buf, "width:", 128 ) == 0 )
		{
			fscanf( levelfile, "%i", &w );
		}
		else if( strncmp( buf, "height:", 128 ) == 0 )
		{
			fscanf( levelfile, "%i", &h );
		}
	}

	fclose( levelfile );

	temp = LoadSprite( bgimagepath, w, h );
	if( !temp )
	{
		fprintf( stderr, "LoadLevel: FATAL: could not open sprite file: %s\n", bgimagepath );
		exit( -1 );
	}

	CloseLevel();

	__level.bgImage = temp;
	__level.loaded = 1;
}


void DrawLevel()
{
	if ( !__level.loaded )
	{
		return;
	}

	if ( __level.bgImage )
	{
		DrawSprite( __level.bgImage, screen, 0, 0, 0 );
	}
}


void CloseLevel()
{
	if ( !__level.loaded )
	{
		return;
	}

	if ( __level.bgImage != NULL )
	{
		FreeSprite( __level.bgImage );
		__level.bgImage = NULL;
	}
}


void InitLevelSystem()
{
	memset( &__level, 0, sizeof( Level ) );
}
