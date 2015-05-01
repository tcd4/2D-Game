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
	
			if( self->numActors + 1 >= MAX_ACTORS )
			{
				self->numActors++;
				//self->actors[ self->numActors ] = NewActor();
			}
		}
		else if( strncmp( buf, "projectile:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
			//LoadProjectile( self, path );
		}
		else if( strncmp( buf, "mode:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
			//LoadFiringMode( self, path );
		}	
	}

	fclose( file );


	/*
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
	self->w = w;
	self->h = h;
	*/
	fprintf( stdout, "LoadPlayer: player loaded\n" );
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
	self->thinkrate = 0;
	self->nextthink = 0;

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
{/*
	CheckInput( self );

	if( modeList[ __curMode ].nextFire <= NOW )
	{
		Fire( self, &modeList[ __curMode ] );

		modeList[ __curMode ].nextFire = NOW + modeList[ __curMode ].fireRate;
	}*/
}


void CheckInput( Entity *self )
{/*
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
{/*
	if( !self->movetype )
	{
		return;
	}

	if( self->movetype & MOVE_LEFT )
	{
		
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
	*/
}


void PlayerFree( Entity *self )
{
	if( self->sprite )
	{
		FreeSprite( self->sprite );
	}

	if( self->projectile )
	{
		FreeSprite( self->projectile );
	}
}


void PlayerDraw( Entity *self )
{
	if( !self->sprite )
	{
		return;
	}
	fprintf( stdout, "\nx = %f: y = %f\n", self->position[ 0 ], self->position[ 1 ] );

	DrawSprite( self->sprite, screen, self->position[ 0 ], self->position[ 1 ], 0 );
}
