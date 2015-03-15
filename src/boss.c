#include "boss.h"
#include "projectile.h"
#include "ability.h"
#include <string.h>
#include <stdio.h>


extern SDL_Surface *screen;
extern Uint32 NOW;


static int __yBound;
static int __moveSpeed;
struct Path *path;
Ability *abilityList = NULL;


void BossThink( Entity *self );
void BossTouch( Entity *self, Entity *other );
void BossDie( Entity *self );
void BossMove( Entity *self );


void LoadBoss( Entity *self, char *filename )
{
	FILE *bossfile = NULL;
	char buf[ 128 ];
	char bossimagepath[ 128 ];
	char projdefpath[ 128 ];
	char abildefpath[ 128 ];
	Sprite *stemp;
	Ability *atemp;
	int numAbil = 0;

	bossfile = fopen( filename, "r" );
	if( bossfile == NULL )
	{
		fprintf( stderr, "LoadBoss: FATAL: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( bossfile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), bossfile );
		}
		else if( strncmp( buf, "sprite:", 128 ) == 0 )
		{
			fscanf( bossfile, "%s", bossimagepath );
		}
		else if( strncmp( buf, "height:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &self->height );
		}
		else if( strncmp( buf, "width:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &self->width );
		}
		else if( strncmp( buf, "columns:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &self->numFrames );
		}
		else if( strncmp( buf, "delay:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &self->frameDelay );
		}
		else if( strncmp( buf, "health:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &self->health );
		}
		else if( strncmp( buf, "proj:", 128 ) == 0 )
		{
			fscanf( bossfile, "%s", projdefpath );
		}
		else if( strncmp( buf, "vel:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &__moveSpeed );
		}
		else if( strncmp( buf, "ability:", 128 ) == 0 )
		{
			fscanf( bossfile, "%s", abildefpath );

			numAbil++;

			atemp = (Ability *)malloc( sizeof( Ability ) * numAbil );
			memcpy( atemp, abilityList, sizeof( Ability ) * ( numAbil - 1 ) );
			abilityList = atemp;

			LoadAbility( &abilityList[ numAbil - 1 ], abildefpath );
		}
	}

	fclose( bossfile );

	stemp = LoadSprite( bossimagepath, self->width, self->height );
	if( !stemp )
	{
		fprintf( stderr, "LoadBoss: FATAL: could not open sprite file: %s\n", bossimagepath );
		exit( -1 );
	}

	LoadProjectile( self, projdefpath );

	self->sprite = stemp;
}


Entity *InitBoss( char *filename )
{
	Entity *self = NULL;
	float x;

	self = NewEnt();
	if( self == NULL )
	{
		fprintf( stderr, "InitBoss: ERROR, could not make a boss entity:\n" );
		exit( -1 );
	}

	self->classname = "boss";
	self->self = self;
	self->owner = NULL;
	self->opponent = NULL;

	LoadBoss( self, filename );

	self->frame = 0;
	self->drawNextFrame = NOW + self->frameDelay;
	self->visible = 1;

	x = ( screen->w / 2 ) - ( self->width / 2 );
	self->position[ 0 ] = x;
	self->position[ 1 ] = 1;
	__yBound = screen->w / 3;

	self->movedir = MOVE_RANDOM;
	VectorClear( self->velocity );

	self->deadflag = 0;
	
	self->damage = 0;
	
	self->Move = BossMove;

	return self;
}


void BossMove( Entity *self )
{
	vec2_t check;

	if( !self->movedir )
	{
		return;
	}

	if( path == NULL )
	{
		path = (struct Path *)malloc( sizeof( struct Path ) );

		if( self->movedir & MOVE_RANDOM )
		{
			srand( NOW );
			path->pathto[ 0 ] = rand() % ( screen->w - self->width );
			path->pathto[ 1 ] = rand() % __yBound;
		}

		path->next = NULL;
	}

	VectorClear( check );

	if( VectorCompare( self->velocity, check ) )
	{
		CalculateVelocity( self );
	}

	VectorAdd( self->position, self->velocity, check );
	
	if( ( ( check[ 0 ] <= path->pathto[ 0 ] ) && ( path->pathto[ 0 ] <= self->position[ 0 ] ) ) || 
		( ( check[ 0 ] >= path->pathto[ 0 ] ) && ( path->pathto[ 0 ] >= self->position[ 0 ] ) ) )
	{
		VectorCopy( path->pathto, self->position );
		VectorClear( self->velocity );
		RemovePath();
	}
	else
	{
		VectorCopy( check, self->position );
	}
}


void CalculateVelocity( Entity *self )
{
	float ang;
	float hypot;
	vec2_t dist;

	VectorSubtract( path->pathto, self->position, dist );
	hypot = sqrt( pow( dist[ 0 ], 2 ) + pow( dist[ 1 ], 2 ) );

	self->velocity[ 0 ] = __moveSpeed * ( dist[ 0 ] / hypot );
	self->velocity[ 1 ] = __moveSpeed * ( dist[ 1 ] / hypot );
}


void RemovePath()
{
	struct Path *temp;

	temp = path;
	free( path );
	path = temp->next;
}
