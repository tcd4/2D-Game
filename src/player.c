#include "player.h"
#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern Uint32 NOW;
extern Uint8 *keys;


void PlayerThink( Entity *self );
void PlayerTouch( Entity *self, Entity *other );
void PlayerDie( Entity *self );
void PlayerMove( Entity *self );


void LoadPlayer( Entity *self, char *filename )
{
	FILE *charfile = NULL;
	char buf[ 128 ];
	char charimagepath[ 128 ];
	char projdefpath[ 128 ];
	int w, h;
	int col;
	int delay;
	Sprite *temp;
	
	charfile = fopen( filename, "r" );
	if( charfile == NULL )
	{
		fprintf( stderr, "LoadPlayer: FATAL: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( charfile, "%s", buf ) != EOF )
	{
		if( buf[0] == '#' )
		{
			fgets( buf, sizeof( buf ), charfile );
		}
		else if( strncmp( buf, "sprite:", 128 ) == 0 )
		{
			fscanf( charfile, "%s", charimagepath );
		}
		else if( strncmp( buf, "height:", 128 ) == 0 )
		{
			fscanf( charfile, "%i", &h );
		}
		else if( strncmp( buf, "width:", 128 ) == 0 )
		{
			fscanf( charfile, "%i", &w );
		}
		else if( strncmp( buf, "columns:", 128 ) == 0 )
		{
			fscanf( charfile, "%i", &col );
		}
		else if( strncmp( buf, "delay:", 128 ) == 0 )
		{
			fscanf( charfile, "%i", &delay );
		}
		else if( strncmp( buf, "proj:", 128 ) == 0 )
		{
			fscanf( charfile, "%s", projdefpath );
		}
	}

	fclose( charfile );

	temp = LoadSprite( charimagepath, w, h );
	if( !temp )
	{
		fprintf( stderr, "LoadPlayer: FATAL: could not open sprite file: %s\n", charimagepath );
		exit( -1 );
	}

	LoadProjectile( self, projdefpath );

	self->sprite = temp;
	self->width = w;
	self->height = h;
	self->numFrames = col;
	self->frameDelay = delay;
}


void InitPlayer()
{
	Entity *self = NULL;

	self = NewEnt();
	if( self == NULL )
	{
		fprintf( stderr, "InitPlayer: ERROR, could not make a player entity:\n" );
		exit( -1 );
	}

	self->classname = "player";
	self->self = self;
	self->owner = NULL;
	self->opponent = NULL;

	LoadPlayer( self, "def/Marisa.txt" );
	self->frame = 0;
	self->drawNextFrame = NOW + self->frameDelay;
	self->visible = 1;

	self->position[ 0 ] = 220 - ( self->width / 2 );
	self->position[ 1 ] = 480 - self->height;
	self->movedir = MOVE_NO;
	self->velocity = 0;

	self->deadflag = 0;
	
	self->thinkrate = 50;
	self->nextthink = NOW + self->thinkrate;
	
	self->Think = PlayerThink;
	self->Touch = PlayerTouch;
	self->Die = PlayerDie;
	self->Move = PlayerMove;
	/*
	self->World_Touch = PlayerWorldTouch;
	*/
}


void PlayerThink( Entity *self )
{
	if( keys[ SDLK_w ] )
	{
		self->movedir |= MOVE_UP;
	}
	else
	{
		self->movedir &= ~MOVE_UP;
	}
	
	if( keys[ SDLK_a ] )
	{
		self->movedir |= MOVE_LEFT;
	}
	else
	{
		self->movedir &= ~MOVE_LEFT;
	}

	if( keys[ SDLK_s ] )
	{
		self->movedir |= MOVE_DOWN;
	}
	else
	{
		self->movedir &= ~MOVE_DOWN;
	}

	if( keys[ SDLK_d ] )
	{
		self->movedir |= MOVE_RIGHT;
	}
	else
	{
		self->movedir &= ~MOVE_RIGHT;
	}
}


void PlayerTouch( Entity *self, Entity *other )
{
}


void PlayerDie( Entity *self )
{
}


void PlayerMove( Entity *self )
{
	if( self->movedir & MOVE_UP )
	{
		self->position[ 1 ] -= 1;
	}

	if( self->movedir & MOVE_DOWN )
	{
		self->position[ 1 ] += 1;
	}

	if( self->movedir & MOVE_LEFT )
	{
		self->position[ 0 ] -= 1;
	}

	if( self->movedir & MOVE_RIGHT )
	{
		self->position[ 0 ] += 1;
	}
}
