#include <string.h>
#include <stdio.h>
#include "player.h"
#include "firingmode.h"
#include "projectile.h"


extern Uint32 NOW;
extern Uint8 *keys;
extern SDL_Surface *screen;


static int __numModes = 0;
static int __curMode = 0;
static int __nextFire = 0;
static int __canModeSwitch = 0;
static float __maxVelocity = 7.500;
static FireMode * __modeList = NULL;


void PlayerThink( Entity *self );
void PlayerTouch( Entity *self, Entity *other );
void PlayerDie( Entity *self );
void PlayerMove( Entity *self );
void PlayerDraw( Entity *self );
void PlayerFree( Entity *self );


int LoadPlayer( Entity *self, char *filename )
{
	FILE *file = NULL;
	char buf[ 128 ];
	char path[ FILE_PATH_LEN ];
	Sprite *temp;
	int loadedmodes = 0;
	int fpl;

	file = fopen( filename, "r" );
	if( !file )
	{
		fprintf( stderr, "ERROR: LoadPlayer: could not open file: %s\n", filename );
		return 0;
	}
	
	/* go through the def file */
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
				fprintf( stderr, "ERROR: LoadPlayer: could not open player sprite\n" );
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
		else if( strncmp( buf, "firemode:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
			LoadFiringMode( &__modeList[ loadedmodes ], path );
			loadedmodes++;
		}	
	}

	fclose( file );

	fprintf( stdout, "LoadPlayer: player loaded\n" );
	return 1;
}


Entity *InitPlayer( char *filename )
{
	Entity *self = NULL;

	self = NewEnt();
	if( self == NULL )
	{
		fprintf( stderr, "InitPlayer: FATAL: could not load player\n" );
		exit( 1 );
	}

	Vec2Clear( self->position );
	Vec2Clear( self->velocity );
	Vec4Clear( self->bbox );
	Vec2Clear( self->offset );

	self->numActors = 0;
	self->deadflag = 0;
	self->thinkrate = 1;
	self->state = 0;

	InitFireModeList();

	if( !LoadPlayer( self, filename ) )
	{
		FreeEnt( self );
		return NULL;
	}

	self->Draw = PlayerDraw;
	self->Think = PlayerThink;
	self->Touch = PlayerTouch;
	self->Die = PlayerDie;
	self->Free = PlayerFree;

	self->owner = NULL;
	self->self = self;

	self->position[ 0 ] = ( screen->w / 2 ) - ( self->w / 2 ) - 15;
	self->position[ 1 ] = screen->h - self->h;
	self->origin[ 0 ] = self->position[ 0 ] + (self->w / 2 );
	self->origin[ 1 ] = self->position[ 1 ] + (self->h / 2 );

	self->trapped = 1;
	self->canCollide = 0;
	self->group = 2;
	self->visible = 0;
	self->inuse = 1;

	fprintf( stdout, "InitPlayer: player initialized\n" );
	return self;
}


void PlayerThink( Entity *self )
{
	CheckInput( self );

	if( __nextFire <= 0 )
	{
		Fire( self, &__modeList[ __curMode ] );
		__nextFire = __modeList[ __curMode ].fireRate;
	}
	else
	{
		__nextFire--;
	}

	if( __canModeSwitch )
	{
		__canModeSwitch--;
	}
}


void CheckInput( Entity *self )
{	
	Vec2Clear( self->velocity );

	if( keys[ SDLK_w ] )
	{
		self->velocity[ 1 ] = -__maxVelocity;
	}
	else if( keys[ SDLK_s ] )
	{
		self->velocity[ 1 ] = __maxVelocity;
	}

	if( keys[ SDLK_a ] )
	{
		self->velocity[ 0 ] = -__maxVelocity;
	}
	else if( keys[ SDLK_d ] )
	{
		self->velocity[ 0 ] = __maxVelocity;
	}
	
	if( keys[ SDLK_SPACE ] && !__canModeSwitch )
	{
		ChangeFireMode( self );
	}
}


void ChangeFireMode( Entity *self )
{
	do
	{
		__curMode++;
		if( __curMode >= MAX_FIRE_MODES )
		{
			__curMode = 0;
		}		
	}while( !__modeList[ __curMode ].loaded );

	Fire( self, &__modeList[ __curMode ] );
	__nextFire = __modeList[ __curMode ].fireRate;

	__canModeSwitch = 5;
}


void PlayerTouch( Entity *self, Entity *other )
{
}


void PlayerDie( Entity *self )
{
}


void PlayerFree( Entity *self )
{
	int i;

	if( self->sprite )
	{
		FreeSprite( self->sprite );
	}

	for( i = 0; i < self->numActors; i++ )
	{
		free( self->actors[ i ] );
		self->actors[ i ] = NULL;
	}

	fprintf( stdout, "PlayerFree: player freed\n" );
}


void PlayerDraw( Entity *self )
{
	int f;

	f = UseActor( self->actors[ self->state ] );
	DrawSprite( self->sprite, screen, self->position[ 0 ], self->position[ 1 ], f );
}


void InitFireModeList()
{
	if( __modeList )
	{
		ClearFireModeList();
	}
	else
	{
		__modeList = ( FireMode *)malloc( sizeof( FireMode ) * MAX_FIRE_MODES );
	}

    if ( !__modeList )
    {
        fprintf( stderr, "FATAL: InitFireModeList: cannot allocate firing mode list\n" );
        exit( -1 );
    }

	memset( __modeList, 0, sizeof( FireMode ) * MAX_FIRE_MODES );
	fprintf( stdout, "InitFireModeList: initialized\n" );
}


void ClearFireModeList()
{
	int i;

	for( i = 0; i < MAX_FIRE_MODES; i++ )
	{
		if( __modeList[ i ].loaded )
		{
			FreeFireMode( &__modeList[ i ] );
		}
	}
}
