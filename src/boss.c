#include "boss.h"
#include "projectile.h"
#include "ability.h"
#include <string.h>
#include <stdio.h>


extern SDL_Surface *screen;
extern Uint32 NOW;
extern TTF_Font *font;


static int __moveRate;
static int __moveSpeed;
static Ability * __abilityList = NULL;


Uint32 lock = 0;
Uint32 cooldown = 0;


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
	int loadedabilities = 0;

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
		else if( strncmp( buf, "health:", 128 ) == 0 )
		{
			fscanf( file, "%i", &self->health );
		}
		else if( strncmp( buf, "move_speed:", 128 ) == 0 )
		{
			fscanf( file, "%i", &__moveSpeed );
		}
		else if( strncmp( buf, "move_rate:", 128 ) == 0 )
		{
			fscanf( file, "%i", &__moveRate );
		}
		else if( strncmp( buf, "ability:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
			LoadAbility( &__abilityList[ loadedabilities ], path, self );
			loadedabilities++;
		} 
	}

	fclose( file );

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

	InitAbilityList();

	if( !LoadBoss( self, filename ) )
	{
		FreeEnt( self );
		return NULL;
	}

	self->numActors = 0;
	self->deadflag = 0;
	self->thinkrate = 1;
	self->state = 0;
	self->maxHealth = self->health;

	self->Draw = BossDraw;
	self->Think = BossThink;
	self->Touch = BossTouch;
	self->Die = BossDie;
	self->Free = BossFree;
	self->Move = BossMove;

	self->owner = NULL;
	self->self = self;

	self->position[ 0 ] = ( screen->w / 2 ) - ( self->w / 2 );
	self->position[ 1 ] = 50;
	self->origin[ 0 ] = self->position[ 0 ] + (self->w / 2 );
	self->origin[ 1 ] = self->position[ 1 ] + (self->h / 2 );
	self->bbox[ 0 ] = self->position[ 0 ];
	self->bbox[ 1 ] = self->position[ 1 ];
	self->bbox[ 2 ] = self->w;
	self->bbox[ 3 ] = self->h;

	self->trapped = 1;
	self->canCollide = 0;
	self->group = 4;
	self->visible = 0;
	self->inuse = 1;

	fprintf( stdout, "InitBoss: boss initialized\n" );
	return self;
}


void InitAbilityList()
{
	if( __abilityList )
	{
		ClearAbilityList();
	}
	else
	{
		__abilityList = ( Ability *)malloc( sizeof( Ability ) * MAX_ABILITIES );
	}

    if ( !__abilityList )
    {
        fprintf( stderr, "FATAL: InitAbilityList: cannot allocate ability list\n" );
        exit( -1 );
    }

	memset( __abilityList, 0, sizeof( Ability ) * MAX_ABILITIES );
	fprintf( stdout, "InitAbilityList: initialized\n" );
}


void ClearAbilityList()
{
	int i;

	for( i = 0; i < MAX_ABILITIES; i++ )
	{
		if( __abilityList[ i ].loaded )
		{
			FreeAbility( &__abilityList[ i ] );
		}
	}
}


void BossMove( Entity *self )
{
	self->bbox[ 0 ] = self->position[ 0 ];
	self->bbox[ 1 ] = self->position[ 1 ];
	/*
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
	if( !lock && cooldown < NOW )
	{
		PickAbility();
	}

	CheckAbilities( self );
}


void CheckAbilities( Entity *self )
{
	int i;

	for( i = 0; i < MAX_ABILITIES; i++ )
	{
		if( __abilityList[ i ].inuse )
		{
			UseAbility( &__abilityList[ i ] );
		}
		else if( __abilityList[ i ].startTime && __abilityList[ i ].startTime <= NOW && !__abilityList[ i ].inuse )
		{
			StartAbility( &__abilityList[ i ] );
		}
	}
}


void PickAbility()
{
	int i;
	float num;

	for( i = 0; i < MAX_ABILITIES; i++ )
	{
		if( __abilityList[ i ].loaded && !__abilityList[ i ].inuse && !__abilityList[ i ].startTime )
		{
			num = crandom();
			
			if( num > 0.000000 )
			{
				StartAbility( &__abilityList[ i ] );
				break;
			}
			else
			{
				i++;
			}
		}
	}
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
	char message[ 10 ];
	SDL_Surface *temp;
	SDL_Surface *txt;
	SDL_Rect src, dest;
	SDL_Color c1, c2;
	int hp;

	f = UseActor( self->actors[ self->state ] );
	DrawSprite( self->sprite, screen, self->position[ 0 ], self->position[ 1 ], f );

	hp = ( float )self->health / self->maxHealth * 100;
	sprintf( message, "%i%%", hp );
	
	c1.r = 255;
	c1.g = 255;
	c1.b = 255;
	c2.r = 0;
	c2.r = 0;
	c2.r = 0;

	temp = TTF_RenderText( font, message, c1, c2 );
	txt = SDL_DisplayFormat( temp );
	SDL_SetColorKey( txt, SDL_SRCCOLORKEY, SDL_MapRGB( txt->format, 255, 255, 255 ) );
	SDL_FreeSurface( temp );

	src.x = 0;
	src.y = 0;
	src.w = txt->w;
	src.h = txt->h;

	dest.x = screen->w - 40;
	dest.y = 5;
	dest.w = txt->w;
	dest.h = txt->h;
	SDL_BlitSurface( txt, &src, screen, &dest );
}


void BossTouch( Entity *self, Entity *other )
{
	self->health -= other->damage;
}


void BossDie( Entity *self )
{
}
