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
static Uint32 __canModeSwitch = 0;
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
	int dmg;
	Sprite *stemp;
	Mode *mtemp;

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
		else if( strncmp( buf, "mode:", 128 ) == 0 )
		{
			fscanf( charfile, "%s", modedefpath );
			
			__numModes++;

			mtemp = (Mode *)malloc( sizeof( Mode ) * __numModes );
			memcpy( mtemp, modeList, sizeof( Mode ) * ( __numModes - 1 ) );
			modeList = mtemp;

			LoadMode( &modeList[ __numModes - 1 ], modedefpath );
		}
		else if( strncmp( buf, "damage:", 128 ) == 0 )
		{
			fscanf( charfile, "%i", &dmg );
		}
	}

	fclose( charfile );

	stemp = LoadSprite( charimagepath, w, h );
	if( !stemp )
	{
		fprintf( stderr, "LoadPlayer: FATAL: could not open sprite file: %s\n", charimagepath );
		exit( -1 );
	}

	LoadProjectile( self, projdefpath );

	self->sprite = stemp;
	self->width = w;
	self->height = h;
	self->numFrames = col;
	self->frameDelay = delay;
	self->damage = dmg;
}


Entity *InitPlayer()
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

	LoadPlayer( self, "def/Characters/Marisa/Marisa.txt" );
	self->frame = 0;
	self->drawNextFrame = NOW + self->frameDelay;
	self->visible = 1;

	x = ( screen->w / 2 ) - ( self->width / 2 );
	y = screen->h - self->height;
	self->position[ 0 ] = x;
	self->position[ 1 ] = y;
	self->movetype = MOVE_NO;
	Vec2Clear( self->velocity );

	self->deadflag = 0;
	
	self->damage = 0;

	self->thinkrate = 50;
	self->nextthink = NOW + self->thinkrate;
	
	self->Think = PlayerThink;
	self->Touch = PlayerTouch;
	self->Die = PlayerDie;
	self->Move = PlayerMove;
	
	__canModeSwitch = NOW;

	return self;
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
	self->movetype = 0;

	if( keys[ SDLK_w ] )
	{
		self->movetype |= MOVE_UP;
	}
	else if( keys[ SDLK_s ] )
	{
		self->movetype |= MOVE_DOWN;
	}

	if( keys[ SDLK_a ] )
	{
		self->movetype |= MOVE_LEFT;
	}
	else if( keys[ SDLK_d ] )
	{
		self->movetype |= MOVE_RIGHT;
	}

	if( keys[ SDLK_SPACE ] )
	{
		if( __canModeSwitch <= NOW )
		{
			__canModeSwitch = NOW + MODE_SWITCH_CD;

			__curMode = ( __curMode + 1 ) % __numModes;		
		}
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
	if( !self->movetype )
	{
		return;
	}

	if( self->movetype & MOVE_LEFT )
	{
		/* instantly go the opposite direction instead of slowing down first */
		if( self->velocity[ 0 ] > 0 )
		{
			self->velocity[ 0 ] = 0;
		}

		self->velocity[ 0 ]--;
	}
	else if( self->movetype & MOVE_RIGHT )
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

	if( self->movetype & MOVE_UP )
	{
		if( self->velocity[ 1 ] > 0 )
		{
			self->velocity[ 1 ] = 0;
		}

		self->velocity[ 1 ]--;
	}
	else if( self->movetype & MOVE_DOWN )
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
	
	Vec2Add( self->velocity, self->position, self->position );

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
