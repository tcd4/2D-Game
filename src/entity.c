#include "entity.h"


extern SDL_Surface *screen;
extern SDL_Event Event;
extern SDL_Rect Camera;
extern Uint32 NOW;
extern Uint32 FRAME;


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


void UpdateEnts()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( __entList[ i ].inuse )
		{
			if( ( __entList[ i ].Think ) && (  __entList[ i ].nextThink < NOW ) )
			{
				__entList[ i ].Think( &__entList[ i ] );
			}		
		}
	}
}


void MoveEnts()
{
	int i;

	for( i = 0; i < MAX_ENTITIES; i++ )
	{
		if( __entList[ i ].inuse )
		{
			MoveEnt( &__entList[ i ] );

			if( __entList[ i ].Move )
			{
				__entList[ i ].Move( &__entList[ i ] );
			}
		}
	}
}


void MoveEnt( Entity *ent )
{
	Vec2Add( ent->position, ent->velocity, ent->position );
	Vec2Add( ent->origin, ent->velocity, ent->origin );

	if( ent->canCollide )
	{
		CheckForCollision( ent );
	}

	if( ent->trapped )
	{
		if( ent->position[ 0 ] < 0 )
		{
			ent->position[ 0 ] = 0;
			ent->origin[ 0 ] = ent->w / 2;
		}
		else if( ( ent->position[ 0 ] + ent->w ) > screen->w )
		{
			ent->position[ 0 ] = screen->w - ent->w;
			ent->origin[ 0 ] = screen->w - ( ent->w / 2 );
		}

		if( ent->position[ 1 ] < 0 )
		{
			ent->position[ 1 ] = 0;
			ent->origin[ 1 ] = ent->h / 2;
		}
		else if( ( ent->position[ 1 ] + ent->h ) > screen->h )
		{
			ent->position[ 1 ] = screen->h - ent->h;
			ent->origin[ 1 ] = screen->h - ( ent->h / 2 );
		}
	}
}


int OutOfBounds( Entity *ent )
{
	if( ent->position[ 0 ] > screen->w )
	{
		return 1;
	}
	else if( ent->position[ 1 ] > screen->h )
	{
		return 1;
	}
	else if( ent->position[ 0 ] + ent->w < 0 )
	{
		return 1;
	}
	else if( ent->position[ 1 ] + ent->h < 0 )
	{
		return 1;
	}

	return 0;
}


void ChangeState( Entity *ent, char *state )
{
	int i;

	for( i = 0; i < MAX_ACTORS; i++ )
	{
		if( ent->actors[ i ] && ( strcmp( state, ent->actors[ i ]->action ) == 0 ) )
		{
			ResetActor( ent->actors[ i ] );
			ent->state = i;
		}
	}
}


void CheckForCollision( Entity *ent )
{
	int i;

	for( i = 0; i < numEnts; i++ )
	{
		if( ( strncmp( __entList[ i ].name, "projectile", TYPE_NAME_LEN ) != 0 ) && !( ent->group & __entList[ i ].group ) )
		{
			if( ( ent->position[ 0 ] >= __entList[ i ].bbox[ 0 ] ) && ( ent->position[ 0 ] <= __entList[ i ].bbox[ 0 ] + __entList[ i ].bbox[ 2 ] ) )
			{
				if( ( ent->position[ 1 ] >= __entList[ i ].bbox[ 1 ] ) && ( ent->position[ 1 ] <= __entList[ i ].bbox[ 1 ] + __entList[ i ].bbox[ 3 ] ) )
				{
					ent->Touch( ent, &__entList[ i ] );
					__entList[ i ].Touch( &__entList[ i ], ent );
				}
			}
		}		
	}
}
