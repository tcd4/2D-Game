#include "entity.h"


extern SDL_Surface *screen;
extern SDL_Event Event;
extern SDL_Rect Camera;
extern Uint32 NOW;


static Entity * __entList = NULL;
int	numEnts;


void InitEntitySystem()
{
	numEnts = 0;

	__entList = (Entity *)malloc( sizeof( Entity ) * MAX_ENTITIES );

    if ( __entList == NULL )
    {
        fprintf( stderr, "FATAL: InitEntntList: cannot allocate entity list\n" );
        exit( -1 );
    }

	memset( __entList, 0, sizeof( Entity ) * MAX_ENTITIES );
	fprintf( stdout, "InitEntList: initialized\n" );
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

	fprintf( stdout, "FreeEntList: all entities freed\n" );
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

	if( ent->Free )
	{
		ent->Free( ent );
	}

	ent->Draw = NULL;
	ent->Free = NULL;
	ent->Think = NULL;
	ent->Touch = NULL;
	ent->Die = NULL;
	ent->Move = NULL;
}


Entity *NewEnt()
{
	int i;

	if( numEnts + 1 >= MAX_ENTITIES )
	{
		fprintf( stderr, "ERROR: NewEnt: hit entity cap\n" );
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

	fprintf( stderr, "ERROR: NewEnt: all entities are in use\n" );
	return NULL;
}


void UpdateEnts()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( __entList[ i ].inuse )
		{
			if( __entList[ i ].Move != NULL )
			{
				__entList[ i ].Move( &__entList[ i ] );
			}

			if( __entList[ i ].Think != NULL )
			{
				if( __entList[ i ].nextthink <= NOW )
				{
					__entList[ i ].Think( &__entList[ i ] );
					__entList[ i ].nextthink = NOW + __entList[ i ].thinkrate;
				}
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
		}
	}
}


void DrawEnt( Entity *ent )
{
	if( ent->Draw )
	{
		ent->Draw( ent );
	}
	else if( !ent->sprite )
	{
		fprintf( stderr, "ERROR: DrawEnt: no sprite or draw function for entity\n" );		
	}
	else
	{
		DrawSprite( ent->sprite, screen, ent->position[ 0 ], ent->position[ 1 ], 0 );
	}
}
