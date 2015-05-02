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
void BossFree( Entity *self );
void BossDraw( Entity *self );


int LoadBoss( Entity *self, char *filename )
{
	FILE *file = NULL;
	char buf[ 128 ];
	char path[ FILE_PATH_LEN ];
	Sprite *temp;
	int fpl;

	file = fopen( filename, "r" );
	if( !file )
	{
		fprintf( stderr, "ERROR: LoadBoss: could not open file: %s\n", filename );
		return 0;
	}

	while( fscanf( file, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), file );
		}
		else if( strncmp( buf, "name:", 128 ) == 0 )
		{
			fscanf( file, "%s", self->name );
		}
		else if( strncmp( buf, "sprite:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );

			temp = LoadSprite( path, self->w, self->h, fpl );
			if( !temp )
			{
				fprintf( stderr, "ERROR: LoadBoss: could not open boss sprite\n" );
				return 0;
			}
			
			self->sprite = temp;
		}
		else if( strncmp( buf, "size:", 128 ) == 0 )
		{
			fscanf( file, "%i,%i", &self->w, &self->h );		
		}
		else if( strncmp( buf, "framesperline:", 128 ) == 0 )
		{
			fscanf( file, "%i", &fpl );
		}
		else if( strncmp( buf, "bbox:", 128 ) == 0 )
		{
			fscanf( file, "%i,%i", &self->bbox[ 3 ], &self->bbox[ 4 ] );
		}
		else if( strncmp( buf, "offset:", 128 ) == 0 )
		{
			fscanf( file, "%i,%i", &self->offset[ 0 ], &self->offset[ 1 ] );
		}
		else if( strncmp( buf, "actor:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
	
			if( self->numActors + 1 <= MAX_ACTORS )
			{	
				self->actors[ self->numActors ] = NewActor( path );

				if( !self->actors )
				{
					self->numActors--;
				}
				
				self->numActors++;
			}
		}
	}

	fclose( file );

	/*
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
			fscanf( bossfile, "%i", &self->h );
		}
		else if( strncmp( buf, "width:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &self->w );
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

	stemp = LoadSprite( bossimagepath, self->w, self->h );
	if( !stemp )
	{
		fprintf( stderr, "LoadBoss: FATAL: could not open sprite file: %s\n", bossimagepath );
		exit( -1 );
	}

	LoadProjectile( self, projdefpath );

	self->sprite = stemp;*/
	fprintf( stdout, "LoadBoss: %s loaded\n", self->name );
	return 1;
}


Entity *InitBoss( char *filename )
{
	Entity *self = NULL;

	self = NewEnt();
	if( !self )
	{
		fprintf( stderr, "FATAL: InitBoss: could not load boss\n" );
		exit( 1 );
	}

	Vec2Clear( self->position );
	Vec2Clear( self->velocity );
	Vec4Clear( self->bbox );
	Vec2Clear( self->offset );

	if( !LoadBoss( self, filename ) )
	{
		FreeEnt( self );
		return NULL;
	}

	self->numActors = 0;
	self->deadflag = 0;
	self->thinkrate = 1;
	self->state = 0;

	self->Draw = BossDraw;
	self->Think = BossThink;
	self->Touch = BossTouch;
	self->Die = BossDie;
	self->Free = BossFree;
	self->Move = BossMove;

	self->owner = NULL;
	self->self = self;

	self->position[ 0 ] = ( screen->w / 2 ) - ( self->w / 2 );
	self->position[ 1 ] = 20;
	
	self->trapped = 1;
	self->canCollide = 0;
	self->group = 4;
	self->visible = 0;
	self->inuse = 1;

	fprintf( stdout, "InitBoss: boss initialized\n" );
	return self;
	/*
	Entity *self = NULL;
	float x;

	self = NewEnt();
	if( self == NULL )
	{
		fprintf( stderr, "InitBoss: ERROR, could not make a boss entity:\n" );
		exit( -1 );
	}

	//self->name = "boss";
	self->self = self;
	self->owner = NULL;

	LoadBoss( self, filename );

	self->visible = 1;

	x = ( screen->w / 2 ) - ( self->w / 2 );
	self->position[ 0 ] = x;
	self->position[ 1 ] = 1;
	__yBound = screen->w / 3;

	self->movetype = MOVE_RANDOM;

	Vec2Clear( self->velocity );

	self->deadflag = 0;
	
	self->Move = BossMove;
	self->Think = BossThink;

	cooldown = 0;
	lock = 0;

	return self;*/
	return NULL;
}


void BossMove( Entity *self )
{/*
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

			self->path = AddPath( self->path, check );
		}
		else
		{
			return;
		}
	}

	Vec2Clear( check );

	//don't recalculate our velocity unless we have to
	if( Vec2Compare( self->velocity, check ) )
	{
		CalculateVelocity( self );
	}

	Vec2Add( self->position, self->velocity, check );
	
	//don't overshoot our destination
	if( ( ( check[ 0 ] <= self->path->pos[ 0 ] ) && ( self->path->pos[ 0 ] <= self->position[ 0 ] ) ) || 
		( ( check[ 0 ] >= self->path->pos[ 0 ] ) && ( self->path->pos[ 0 ] >= self->position[ 0 ] ) ) )
	{
		Vec2Copy( self->path->pos, self->position );
		Vec2Clear( self->velocity );
		NextPosition( self );
	}
	else
	{
		Vec2Copy( check, self->position );
	}*/
}


void CalculateVelocity( Entity *self )
{/*
	float hypot;
	vec2_t dist;

	//Vec2Subtract( self->path->pos, self->position, dist );
	hypot = sqrt( pow( dist[ 0 ], 2 ) + pow( dist[ 1 ], 2 ) );

	self->velocity[ 0 ] = __moveSpeed * ( dist[ 0 ] / hypot );
	self->velocity[ 1 ] = __moveSpeed * ( dist[ 1 ] / hypot );*/
}


void BossThink( Entity *self )
{
	//CheckAbilities( self );
	//UseRandomAbility( self );
}


void CheckAbilities( Entity *self )
{/*
	int i;

	for( i = 0; i < __numAbilities; i++ )
	{		
		//use abilities that are being used or abilities that are supposed to start
		if( abilityList[ i ].inuse )
		{
			//make sure the ability hasn't ended
			if( ( abilityList[ i ].endTime ) && ( abilityList[ i ].endTime <= NOW ) )
			{
				StopAbility( self, i );
				continue;
			}

			if( ( abilityList[ i ].nextfire ) && ( abilityList[ i ].nextfire <= NOW ) )
			{
				UseAbility( &abilityList[ i ] );
			}
		}
		else if( ( abilityList[ i ].startTime ) && ( abilityList[ i ].startTime <= NOW ) )
		{
			UseAbility( &abilityList[ i ] );
		}
	}*/
}


void UseRandomAbility( Entity *self )
{/*
	int i;

	if( ( cooldown < NOW ) && ( !lock ) )
	{
		i = rand() % __numAbilities;

		//make sure the ability can be used 
		if( ( !abilityList[ i ].inuse ) || ( abilityList[ i ].startTime >= NOW ) )
		{
			cooldown = NOW + abilityList[ i ].cooldown + abilityList[ i ].duration;
			lock = abilityList[ i ].lock;
			self->movetype = abilityList[ i ].movement;
			//self->path = ClearPathing( self->path );

			abilityList[ i ].inuse = 1;
			abilityList[ i ].startTime = NOW;
			if( abilityList[ i ].duration != -1 )
			{
				abilityList[ i ].endTime = NOW + abilityList[ i ].duration;
			}
		
			//EndAbilities();
			UseAbility( &abilityList[ i ] );
		}
	}*/
}


void StopAbility( Entity *self, int index )
{/*
	abilityList[ index ].inuse = 0;
	abilityList[ index ].startTime = 0;
	lock -= abilityList[ index ].lock;
	self->movetype = MOVE_RANDOM;*/
}


void EndAbilities()
{/*
	int i;

	for( i = 0; i < __numAbilities; i++ )
	{
		if( abilityList[ i ].duration != -1 )
		{
			abilityList[ i ].inuse = 0;
		}	
	}*/
}


void EndAllAbilities()
{/*
	int i;

	for( i = 0; i < __numAbilities; i++ )
	{
		abilityList[ i ].inuse = 0;
	}*/
}


void BossFree( Entity *self )
{
}


void BossDraw( Entity *self )
{
	int f;

	f = UseActor( self->actors[ self->state ] );
	DrawSprite( self->sprite, screen, self->position[ 0 ], self->position[ 1 ], f );
}


void BossTouch( Entity *self, Entity *other )
{
}


void BossDie( Entity *self )
{
}
