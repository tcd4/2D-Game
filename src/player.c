#include "player.h"
#include "firingmode.h"
#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern Uint32 NOW;
extern Uint8 *keys;
extern SDL_Surface *screen;


static int __numModes = 0;
static int __curMode = 0;
Mode *modeList = NULL;

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
	char modedefpath[ 128 ];
	int w, h;
	int col;
	int delay;
	int loadedmodes = 0;
	Sprite *temp;

	charfile = fopen( filename, "r" );
	if( charfile == NULL )
	{
		fprintf( stderr, "LoadPlayer: FATAL: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( charfile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
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
		else if( strncmp( buf, "numMode:", 128 ) == 0 )
		{
			fscanf( charfile, "%i", &__numModes );
			modeList = (Mode *)malloc( sizeof( Mode ) * __numModes );

			if( modeList == NULL )
			{
				fprintf( stderr, "LoadPlayer: FATAL: cannot allocate mode list\n" );
				exit( -1 );
			}
		}
		else if( strncmp( buf, "mode:", 128 ) == 0 )
		{
			fscanf( charfile, "%s", modedefpath );
			LoadMode( &modeList[ loadedmodes ], modedefpath );
			loadedmodes++;
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
	float x, y;

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

	LoadPlayer( self, "def/Marisa/Marisa.txt" );
	self->frame = 0;
	self->drawNextFrame = NOW + self->frameDelay;
	self->visible = 1;

	x = ( screen->w / 2 ) - ( self->width / 2 );
	y = screen->h - self->height;
	self->position[ 0 ] = x;
	self->position[ 1 ] = y;
	self->movedir = MOVE_NO;
	self->velocity[ 0 ] = 0;
	self->velocity[ 0 ] = 0;

	self->deadflag = 0;
	
	self->damage = 0;

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
	CheckInput( self );

	if( modeList[ __curMode ].nextFire <= NOW )
	{
		Fire( self, &modeList[ __curMode ] );

		modeList[ __curMode ].nextFire = NOW + modeList[ __curMode ].fireRate;
	}
}


void CheckInput( Entity *self )
{
	self->movedir = 0;

	if( keys[ SDLK_w ] )
	{
		self->movedir |= MOVE_UP;
	}
	else if( keys[ SDLK_s ] )
	{
		self->movedir |= MOVE_DOWN;
	}

	if( keys[ SDLK_a ] )
	{
		self->movedir |= MOVE_LEFT;
	}
	else if( keys[ SDLK_d ] )
	{
		self->movedir |= MOVE_RIGHT;
	}

	if( keys[ SDLK_SPACE ] )
	{
		__curMode = ( __curMode + 1 ) % __numModes;
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
	if( !self->movedir )
	{
		return;
	}

	if( self->movedir & MOVE_LEFT )
	{
		/* instantly go the opposite direction instead of slowing down first */
		if( self->velocity[ 0 ] > 0 )
		{
			self->velocity[ 0 ] = 0;
		}

		self->velocity[ 0 ]--;
	}
	else if( self->movedir & MOVE_RIGHT )
	{
		if( self->velocity[ 0 ] < 0 )
		{
			self->velocity[ 0 ] = 0;
		}

		self->velocity[ 0 ]++;
	}
	else
	{
		self->velocity[ 0 ] = 0;
	}

	if( self->movedir & MOVE_UP )
	{
		if( self->velocity[ 1 ] > 0 )
		{
			self->velocity[ 1 ] = 0;
		}

		self->velocity[ 1 ]--;
	}
	else if( self->movedir & MOVE_DOWN )
	{
		if( self->velocity[ 1 ] < 0 )
		{
			self->velocity[ 1 ] = 0;
		}

		self->velocity[ 1 ]++;
	}
	else
	{
		self->velocity[ 1 ] = 0;
	}
	
	/* make sure to not go to fast in any direction */
	if( self->velocity[ 0 ] > MAX_VELOCITY )
	{
		self->velocity[ 0 ] = MAX_VELOCITY;
	}
	else if( self->velocity[ 0 ] < -MAX_VELOCITY )
	{
		self->velocity[ 0 ] = -MAX_VELOCITY;
	}

	if( self->velocity[ 1 ] > MAX_VELOCITY )
	{
		self->velocity[ 1 ] = MAX_VELOCITY;
	}
	else if( self->velocity[ 1 ] < -MAX_VELOCITY )
	{
		self->velocity[ 1 ] = -MAX_VELOCITY;
	}
	
	VectorAdd( self->velocity, self->position, self->position );

	/* make sure we stay on the screen */
	if( self->position[ 0 ] < 0 )
	{
		self->position[ 0 ] = 0;
	}
	else if( (self->position[ 0 ] + self->width ) > screen->w )
	{
		self->position[ 0 ] = screen->w - self->width;
	}

	if( self->position[ 1 ] < 0 )
	{
		self->position[ 1 ] = 0;
	}
	else if( (self->position[ 1 ] + self->height ) > screen->h )
	{
		self->position[ 1 ] = screen->h - self->height;
	}
}
