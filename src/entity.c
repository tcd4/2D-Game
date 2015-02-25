#include "entity.h"


extern SDL_Surface *screen;
extern SDL_Event	Event;
extern SDL_Rect		Camera;
extern Uint32		NOW;


static Entity		* __entList = NULL;
int			numEnts;


void InitEntList()
{
	numEnts = 0;

	__entList = (Entity *)malloc( sizeof( Entity ) * MAX_ENTITIES );

    if ( __entList == NULL )
    {
        fprintf( stderr, "Init__entList: FATAL: cannot allocate entity list\n" );
        exit( -1 );
        return;
    }

	memset( __entList, 0, sizeof( Entity ) * MAX_ENTITIES );
}


void FreeEntList()
{
	int i;

	for(i = 0; i < MAX_ENTITIES; i++ )
	{
		if( __entList[ i ].inuse )
		{
			FreeEnt( &__entList[ i ] );
		}
	}
}


void FreeEntsExcept( Entity *ent )
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( ( __entList[ i ].inuse ) && ( ent != &__entList[ i ] ) )
		{
			FreeEnt( &__entList[ i ] );
		}
	}
}


void FreeEnt( Entity *ent )
{
	numEnts--;

	ent->inuse = 0;

	memset( ent, 0, sizeof( Entity ) );
}


Entity *NewEnt()
{
	int i;

	if( numEnts + 1 >= MAX_ENTITIES )
	{
		return NULL;
	}

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( !__entList[ i ].inuse )
		{
			__entList[ i ].inuse = 1;
			numEnts++;

			fprintf( stderr, "new ent created at index %i\n", i );
			return &__entList[ i ];
		}
	}

	fprintf( stderr, "new ent created at index %i\n" );
	return NULL;
}


void ThinkEntities()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( ( __entList[ i ].inuse ) && ( __entList[ i ].Think != NULL ) )
		{
			if( __entList[ i ].nextthink <= NOW )
			{
				__entList[ i ].Think( &__entList[ i ] );
				__entList[ i ].nextthink = NOW + __entList[ i ].thinkrate;
			}
		}
	}
}


void DrawEntList()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( ( __entList[ i ].inuse ) && ( __entList[ i ].visible ) )
		{
			DrawEnt( &__entList[ i ] );

			if( ( __entList[ i ].numFrames > 0 ) && ( __entList[ i ].drawNextFrame <= NOW ) )
			{
				__entList[ i ].frame = ( __entList[ i ].frame + 1 ) % __entList[ i ].numFrames;
				__entList[ i ].drawNextFrame = NOW + __entList[ i ].frameDelay;
			}
		}
	}
}


void DrawEnt( Entity *ent )
{
	if( ent->sprite != NULL )
	{
		DrawSprite( ent->sprite, screen, ent->position[ 0 ], ent->position[ 1 ], ent->frame );
	}
}
