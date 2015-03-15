#include "boss.h"
#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern SDL_Surface *screen;
extern Uint32 NOW;
static int __moveSpeed;


void LoadBoss( Entity *self, char *filename )
{
	FILE *bossfile = NULL;
	char buf[ 128 ];
	char bossimagepath[ 128 ];
	char projdefpath[ 128 ];
	int w, h;
	int col;
	int delay;
	int hp;
	Sprite *stemp;

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
			fscanf( bossfile, "%i", &h );
		}
		else if( strncmp( buf, "width:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &w );
		}
		else if( strncmp( buf, "columns:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &col );
		}
		else if( strncmp( buf, "delay:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &delay );
		}
		else if( strncmp( buf, "health:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &hp );
		}
		else if( strncmp( buf, "proj:", 128 ) == 0 )
		{
			fscanf( bossfile, "%s", projdefpath );
		}
		else if( strncmp( buf, "speed:", 128 ) == 0 )
		{
			fscanf( bossfile, "%i", &__moveSpeed );
		}
	}

	fclose( bossfile );

	stemp = LoadSprite( bossimagepath, w, h );
	if( !stemp )
	{
		fprintf( stderr, "LoadBoss: FATAL: could not open sprite file: %s\n", bossimagepath );
		exit( -1 );
	}

	LoadProjectile( self, projdefpath );

	self->sprite = stemp;
	self->width = w;
	self->height = h;
	self->numFrames = col;
	self->frameDelay = delay;
	self->health = hp;
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

	self->deadflag = 0;
	
	self->damage = 0;

	return self;
}