#include "level.h"
#include "player.h"
#include "boss.h"
#include <string.h>
#include <stdio.h>


static Level *__levelList = NULL;
extern SDL_Surface *screen;


Level *LoadLevel( char *filename )
{
	FILE *file = NULL;
	char buf[ 128 ];
	char img[ FILE_NAME_LEN ];
	Level *level;

	/* initialize the level */
	level = InitLevel();
	if( !level )
	{
		return NULL;
	}

	/* open the def file */
	file = fopen( filename, "r" );
	if( !file )
	{
		fprintf( stderr, "LoadLevel: ERROR: could not open file: %s\n", filename );
		return NULL;
	}

	/* go through the def file */
	while( fscanf( file, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), file );
		}
		else if( strncmp( buf, "name:", 128 ) == 0 )
		{
			fscanf( file, "%s", level->name );
		}
		else if( strncmp( buf, "background:", 128 ) == 0 )
		{
			fscanf( file, "%s", img );
			LoadBackground( img, level );
		}
		else if( strncmp( buf, "layer:", 128 ) == 0 )
		{
			fscanf( file, "%s", img );
			LoadLayer( img, level );
		}
		else if( strncmp( buf, "size:", 128 ) == 0 )
		{
			fscanf( file, "%i,%i", &level->w, &level->h );		
		}
	}

	fclose( file );

	strcpy( level->filename, filename );
	level->loaded = 1;

	fprintf( stdout, "LoadLevel: %s loaded\n", level->name );
	return level;
}


void LoadBackground( char *filename, Level *level )
{
	Sprite *temp;

	temp = LoadSprite( filename, level->w, level->h );
	if( !temp )
	{
		fprintf( stderr, "LoadLevel: ERROR: could not open sprite file: %s\n", filename );
		return;
	}

	fprintf( stdout, "LoadBackground: background loaded\n" );
	level->background = temp;
}


void LoadLayer( char *filename, Level *level )
{
	Sprite *temp;
	Sprite *templist;

	temp = LoadSprite( filename, level->w, level->h );
	if( !temp )
	{
		fprintf( stderr, "LoadLayer: ERROR: could not open sprite file: %s\n", filename );
		return;
	}

	/* increment the count and increase the size of the array */
	level->layerCount++;
	templist = ( Sprite * )malloc( sizeof( Sprite ) * level->layerCount );
	memcpy( templist, level->layers, sizeof( Sprite ) * ( level->layerCount - 1 ) );
	level->layers = templist;
	level->layers[ level->layerCount - 1 ] = *temp;

	fprintf( stdout, "LoadLayer: layer loaded\n" );
}


void DrawLevels()
{
	int i;

	for( i = 0; i < MAX_LEVELS; i++ )
	{
		if( __levelList[ i ].loaded )
		{
			DrawLevel( &__levelList[ i ] );
		}
	}
}


void DrawLevel( Level *level )
{
	if( level->background )
	{
		DrawSprite( level->background, screen, 0, 0, 0 );
	}
}


void FreeLevel( Level *level )
{
	int i;

	level->loaded = 0;
	level->active = 0;
	level->Update = NULL;
	
	FreeSprite( level->background );
	
	for( i = 0; i < level->layerCount; i++ )
	{
		FreeSprite( level->layers + i );
	}
}


void CloseLevelSystem()
{
	int i;

	for( i = 0; i < MAX_LEVELS; i++ )
	{
		if( __levelList[ i ].loaded )
		{
			FreeLevel( &__levelList[ i ] );
		}
	}

	fprintf( stdout, "CloseLevelSystem: level system closed\n" );
}


Level *InitLevel()
{
	int i;

	for( i = 0; i < MAX_LEVELS; i++ )
	{
		if( !__levelList[ i ].loaded )
		{			
			Vec2Clear( __levelList[ i ].position );
			fprintf( stdout, "InitLevel: initialized\n" );

			return &__levelList[ i ];
		}
	}

	fprintf( stderr, "ERROR: InitLevel: maximum number of levels reached" );
	return NULL;
}


void InitLevelSystem()
{
	__levelList = ( Level * )malloc( sizeof( Level ) * MAX_LEVELS );

	if( !__levelList )
	{
		fprintf( stderr, "FATAL: InitLevelSystem: cannot allocate memory for __levelList\n" );
        exit( -1 );
	}

	memset( __levelList, 0, sizeof( Level ) * MAX_LEVELS );
	fprintf( stdout, "InitLevelSystem: initialized\n" );
}
