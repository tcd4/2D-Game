#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"
#include "actor.h"


extern Uint32 FRAME;


Actor *NewActor( char *filename )
{
	FILE *file = NULL;
	char buf[ 128 ];
	Actor *actor;

	actor = ( Actor * )malloc( sizeof( Actor ) );
	if( !actor )
	{
		fprintf( stderr, "ERROR: NewActor: could not allocate memory for actor" );
		return NULL;
	}

	file = fopen( filename, "r" );
	if( !file )
	{
		fprintf( stderr, "ERROR: NewActor: could not open file: %s\n", filename );
		return NULL;
	}
	
	while( fscanf( file, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), file );
		}
		else if( strncmp( buf, "action:", 128 ) == 0 )
		{
			fscanf( file, "%s", actor->action );
		}
		else if( strncmp( buf, "start:", 128 ) == 0 )
		{
			fscanf( file, "%i", &actor->animStart );
		}
		else if( strncmp( buf, "end:", 128 ) == 0 )
		{
			fscanf( file, "%i", &actor->animEnd );
		}
		else if( strncmp( buf, "type:", 128 ) == 0 )
		{
			fscanf( file, "%i", &actor->type );
		}
		else if( strncmp( buf, "rate:", 128 ) == 0 )
		{
			fscanf( file, "%i", &actor->frameRate );
		}
	}

	fclose( file );
	
	actor->numFrames = 1 + actor->animEnd - actor->animStart;
	actor->frame = actor->animStart;
	actor->direction = 1;
	actor->inuse = 0;

	return actor;
}


int UseActor( Actor *actor )
{
	int i;

	if( !actor )
	{
		return 0;
	}

	i = actor->frame;
	if( !( FRAME % actor->frameRate ) && ( actor->type != ANIM_NO ) )
	{
		actor->frame += actor->direction;

		if( actor->frame > actor->animEnd )
		{
			if( actor->type == ANIM_LOOP )
			{
				actor->frame = actor->animStart;
			}
			else if( actor->type == ANIM_OSCI )
			{
				actor->frame -= 2;
				actor->direction = -actor->direction;
			}
			else
			{
				actor->frame = actor->animEnd;
			}
		}
		else if( actor->frame < actor->animStart )
		{
			actor->frame += 2;
			actor->direction = -actor->direction;
		}
	}

	return i;
}
