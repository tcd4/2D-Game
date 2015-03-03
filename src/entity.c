#include "entity.h"


extern SDL_Surface *screen;
extern SDL_Event Event;
extern SDL_Rect Camera;
extern Uint32 NOW;


static Entity * __entList = NULL;
int	numEnts;


void InitEntList()
{
	numEnts = 0;

	__entList = (Entity *)malloc( sizeof( Entity ) * MAX_ENTITIES );

    if ( __entList == NULL )
    {
        fprintf( stderr, "Init__entList: FATAL: cannot allocate entity list\n" );
        exit( -1 );
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

	fprintf( stderr, "freed ent\n" );
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

			return &__entList[ i ];
		}
	}

	return NULL;
}


void UpdateEnts()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( __entList[ i ].inuse )
		{
			if( __entList[ i ].Think != NULL )
			{
				if( __entList[ i ].nextthink <= NOW )
				{
					__entList[ i ].Think( &__entList[ i ] );
					__entList[ i ].nextthink = NOW + __entList[ i ].thinkrate;
				}
			}

			if( __entList[ i ].Move != NULL )
			{
				__entList[ i ].Move( &__entList[ i ] );
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
