#include <string.h>
#include <stdio.h>
#include "player.h"
#include "projectile.h"


extern Uint32 NOW;
extern Uint8 *keys;
extern SDL_Surface *screen;


static int __numModes = 0;
static int __curMode = 0;
static Uint32 __canModeSwitch = 0;
static float __maxVelocity = 7.500;
//Mode *modeList = NULL;


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

	file = fopen( filename, "r" );
	if( !file )
	{
		fprintf( stderr, "ERROR: LoadPlayer: could not open file: %s\n", filename );
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

			temp = LoadSprite( path, self->w, self->h );
			if( temp )
			{
				self->sprite = temp;
			}
			else
			{
				fprintf( stderr, "ERROR: LoadPlayer: could not open character sprite\n" );
			}
		}
		else if( strncmp( buf, "size:", 128 ) == 0 )
		{
			fscanf( file, "%i,%i", &self->w, &self->h );		
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
		else if( strncmp( buf, "mode:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
			//LoadFiringMode( self, path );
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
	}

	Vec2Clear( self->position );
	Vec2Clear( self->velocity );
	Vec4Clear( self->bbox );
	Vec2Clear( self->offset );

	self->numActors = 0;
	self->deadflag = 0;
	self->thinkrate = 2;
	self->state = 0;

	if( !LoadPlayer( self, filename ) )
	{
		FreeEnt( self );
		return NULL;
	}

	self->Draw = PlayerDraw;
	self->Think = PlayerThink;
	self->Touch = PlayerTouch;
	self->Die = PlayerDie;
	self->Move = PlayerMove;
	self->Free = PlayerFree;

	self->owner = NULL;
	self->self = self;

	self->position[ 0 ] = ( screen->w / 2 ) - ( self->w / 2 );
	self->position[ 1 ] = screen->h - self->h;

	self->visible = 0;
	self->inuse = 1;

	fprintf( stdout, "InitPlayer: player initialized\n" );
	return self;
}


void PlayerThink( Entity *self )
{
	CheckInput( self );
	/*
	if( modeList[ __curMode ].nextFire <= NOW )
	{
		Fire( self, &modeList[ __curMode ] );

		modeList[ __curMode ].nextFire = NOW + modeList[ __curMode ].fireRate;
	}*/
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
	
	/*
	if( keys[ SDLK_SPACE ] )
	{
		if( __canModeSwitch <= NOW )
		{
			//__canModeSwitch = NOW + MODE_SWITCH_CD;

			//__curMode = ( __curMode + 1 ) % __numModes;		
		}
	}*/
}


void PlayerTouch( Entity *self, Entity *other )
{
}


void PlayerDie( Entity *self )
{
}


void PlayerMove( Entity *self )
{
	if( self->position[ 0 ] < 0 )
	{
		self->position[ 0 ] = 0;
	}
	else if( (self->position[ 0 ] + self->w ) > screen->w )
	{
		self->position[ 0 ] = screen->w - self->w;
	}

	if( self->position[ 1 ] < 0 )
	{
		self->position[ 1 ] = 0;
	}
	else if( (self->position[ 1 ] + self->h ) > screen->h )
	{
		self->position[ 1 ] = screen->h - self->h;
	}
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

	if( !self->sprite )
	{
		return;
	}

	f = UseActor( self->actors[ self->state ] );
	DrawSprite( self->sprite, screen, self->position[ 0 ], self->position[ 1 ], f );
}
