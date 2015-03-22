#include "boss.h"
#include "projectile.h"
#include "ability.h"
#include <string.h>
#include <stdio.h>


extern SDL_Surface *screen;
extern Uint32 NOW;


static int __yBound;
static int __moveSpeed;
static int __numAbilities;
Uint32 cooldown;
int lock;
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

	__numAbilities = 0;

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

			__numAbilities++;

			atemp = (Ability *)malloc( sizeof( Ability ) * __numAbilities );
			memcpy( atemp, abilityList, sizeof( Ability ) * ( __numAbilities - 1 ) );
			abilityList = atemp;

			LoadAbility( &abilityList[ __numAbilities - 1 ], abildefpath, self );
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

	self->movetype = MOVE_RANDOM;
	self->path = NULL;
	VectorClear( self->velocity );

	self->deadflag = 0;
	
	self->damage = 0;
	
	self->Move = BossMove;
	self->Think = BossThink;

	cooldown = 0;
	lock = 0;

	return self;
}


void BossMove( Entity *self )
{
	vec2_t check;

	//make sure we can move
	if( !self->movetype )
	{
		return;
	}

	//check if there is a place to go
	if( !self->path )
	{
		//check if we want to move
		if( self->movetype & MOVE_RANDOM )
		{
			srand( NOW );
			check[ 0 ] = rand() % ( screen->w - self->width );
			check[ 1 ] = rand() % __yBound;

			AddPath( self, check );
		}
		else
		{
			return;
		}
	}

	VectorClear( check );

	//don't recalculate our velocity unless we have to
	if( VectorCompare( self->velocity, check ) )
	{
		CalculateVelocity( self );
	}

	VectorAdd( self->position, self->velocity, check );
	
	//don't overshoot our destination
	if( ( ( check[ 0 ] <= self->path->pos[ 0 ] ) && ( self->path->pos[ 0 ] <= self->position[ 0 ] ) ) || 
		( ( check[ 0 ] >= self->path->pos[ 0 ] ) && ( self->path->pos[ 0 ] >= self->position[ 0 ] ) ) )
	{
		VectorCopy( self->path->pos, self->position );
		VectorClear( self->velocity );
		NextPosition( self );
	}
	else
	{
		VectorCopy( check, self->position );
	}
}


void CalculateVelocity( Entity *self )
{
	float hypot;
	vec2_t dist;

	VectorSubtract( self->path->pos, self->position, dist );
	hypot = sqrt( pow( dist[ 0 ], 2 ) + pow( dist[ 1 ], 2 ) );

	self->velocity[ 0 ] = __moveSpeed * ( dist[ 0 ] / hypot );
	self->velocity[ 1 ] = __moveSpeed * ( dist[ 1 ] / hypot );
}


void BossThink( Entity *self )
{
	UseAbilities( self );
}


void UseAbilities( Entity *self )
{
	int i;

	for( i = 0; i < __numAbilities; i++ )
	{		
		//use abilities that are being used
		if( abilityList[ i ].inuse )
		{
			UseAbility( &abilityList[ i ] );
		}
	}
	
	//use a random ability 
	if( cooldown < NOW  )
	{
		i = rand() % __numAbilities;

		//make sure the ability can be used 
		if( !( abilityList[ i ].inuse || abilityList[ i ].startTime >= NOW ) )
		{
			cooldown = NOW + abilityList[ i ].cooldown + abilityList[ i ].duration;
			lock = abilityList[ i ].lock;

			abilityList[ i ].inuse = 1;
			abilityList[ i ].startTime = NOW;

			UseAbility( &abilityList[ i ] );
		}
	}
}


void EndAbilities()
{
	int i;

	for( i = 0; i < __numAbilities; i++ )
	{
		if( abilityList[ i ].duration != -1 )
		{
			abilityList[ i ].inuse = 0;
		}	
	}
}


void EndAllAbilities()
{
	int i;

	for( i = 0; i < __numAbilities; i++ )
	{
		abilityList[ i ].inuse = 0;
	}
}
