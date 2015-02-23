#include "player.h"
#include <string.h>
#include <stdio.h>


void LoadPlayer( Entity *self, char *charname )
{
	FILE *charfile = NULL;
	char buf[ 1024 ];
	char spriteimagepath[ 128 ]; 
	char name[1024];
	Sprite *temp;
	
	charfile = fopen( "def/characters.txt", "r" );
	if( charfile == NULL )
	{
		fprintf( stderr, "LoadPlayer: ERROR, could not open file: def/characters.txt\n" );
		return;
	}

	while( fscanf( charfile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), charfile );
		}
		else if( strncmp( buf, "character:", 1024 ) == 0 )
		{
			fscanf( charfile, "%s", name );

			if( strcmp( name, charname ) == 0 )
			{
				while( fscanf( charfile, "%s", buf ) != EOF )
				{
					if( buf[ 0 ] == '#' )
					{
						fgets( buf, sizeof( buf ), charfile );
					}
					else if( strncmp( buf, "character:", 1024 ) == 0 )
					{
						break;
					}
					else if( strncmp( buf, "image:", 1024 ) == 0 )
					{
						fscanf( charfile, "%s", spriteimagepath );
					}
					else if( strncmp( buf, "width:", 1024 ) == 0 )
					{
						fscanf( charfile, "%i", &self->width );
					}
					else if( strncmp( buf, "height:", 1024 ) == 0 )
					{
						fscanf( charfile, "%i", &self->height );
					}
					else if( strncmp( buf, "numFrames:", 1024 ) == 0 )
					{
						fscanf( charfile, "%i", &self->numFrames );
					}
				}

				break;
			}
		}
	}

	fclose( charfile );

	temp = LoadSprite( spriteimagepath, self->width, self->height );
	if( !temp )
	{
		fprintf( stderr, "LoadPlayer: ERROR, could not open sprite file: %s\n", spriteimagepath );
		FreeSprite( temp );
		return;
	}

	self->sprite = temp;
}


void InitPlayer()
{
	Entity *self = newEnt();

	self->classname = "player";

	LoadPlayer( self, "Marisa" );
	self->frame = 0;
	self->visible = 1;

	self->position[ 0 ] = 220 - ( self->width / 2 );
	self->position[ 1 ] = 480 - self->height;
}
