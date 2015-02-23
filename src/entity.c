#include "entity.h"


extern SDL_Surface *screen;
extern SDL_Event	Event;
extern SDL_Rect		Camera;
extern Uint32		NOW;


Entity		* entList = NULL;
int			numEnts;


void InitEntList()
{
	int i;
	
	numEnts = 0;

	entList = (Entity *)malloc( sizeof( Entity ) * MAX_ENTITIES );

    if (entList == NULL)
    {
        fprintf(stderr,"initEntityList: FATAL: cannot allocate entity list\n");
        exit(-1);
        return;
    }

	memset( entList, 0, sizeof( Entity ) * MAX_ENTITIES );

	//fprintf( stderr, "entList initialized\n" );
}


void FreeEntList()
{
	int i;

	for(i = 0; i < MAX_ENTITIES; i++ )
	{
		if( entList[ i ].inuse )
		{
			FreeEnt( &entList[ i ] );
		}
	}
}


void FreeEntsExcept( Entity *ent )
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( ( entList[ i ].inuse ) && ( ent != &entList[ i ] ) )
		{
			FreeEnt( &entList[ i ] );
		}
	}
}


void FreeEnt( Entity *ent )
{
	numEnts--;

	ent->inuse = 0;

	memset( ent, 0, sizeof( Entity ) );
}


Entity *newEnt()
{
	int i;
	//fprintf( stderr, "call to newEnt\n" );
	if( numEnts + 1 >= MAX_ENTITIES )
	{
		return NULL;
	}

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( !entList[ i ].inuse )
		{
			entList[ i ].inuse = 1;
			numEnts++;
			//fprintf( stderr, "new ent created at index %i\n", i );
			return &entList[ i ];
		}
	}

	return NULL;
}


void ThinkEntities()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( ( entList[ i ].inuse ) && ( entList[ i ].Think != NULL ) )
		{
			if( entList[ i ].nextthink <= NOW )
			{
				entList[ i ].Think( &entList[ i ] );
				entList[ i ].nextthink = NOW + entList[ i ].thinkrate;
			}
		}
	}
}


void DrawEntList()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( ( entList[ i ].inuse ) && ( entList[ i ].visible ) )
		{
			DrawEnt( &entList[ i ] );

			if( entList[ i ].numFrames > 0 )
			{
				entList[ i ].frame = ( entList[ i ].frame + 1 ) % entList[ i ].numFrames;
			}
		}
	}
}


void DrawEnt( Entity *ent )
{
	if( !ent->sprite == NULL )
	{
		DrawSprite( ent->sprite, screen, ent->position[ 0 ], ent->position[ 1 ], ent->frame );
	}
}
