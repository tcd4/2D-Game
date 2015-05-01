#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern Uint32 NOW;
extern SDL_Surface *screen;


void ProjectileTouch( Entity *self, Entity *other );
void ProjectileMove( Entity *self );


void LoadProjectile( Entity *owner, char *filename )
{
	FILE *projfile = NULL;
	char buf[ 128 ];
	char projimagepath[ 128 ];
	int w, h;
	Sprite *temp;

	projfile = fopen( filename, "r" );
	if( projfile == NULL )
	{
		fprintf( stderr, "LoadProjectile: FATAL: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( projfile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), projfile );
		}
		else if( strncmp( buf, "sprite:", 128 ) == 0 )
		{
			fscanf( projfile, "%s", projimagepath );
		}
		else if( strncmp( buf, "width:", 128 ) == 0 )
		{
			fscanf( projfile, "%i", &w );
		}
		else if( strncmp( buf, "height:", 128 ) == 0 )
		{
			fscanf( projfile, "%i", &h );
		}
	}

	fclose( projfile );

	temp = LoadSprite( projimagepath, w, h );
	if( !temp )
	{
		fprintf( stderr, "LoadProjectile: FATAL: could not open sprite file: %s\n", projimagepath );
		exit( -1 );
	}

	owner->projectile = temp;
}


void InitProjectile( Entity *owner, Entity *opponent, Sprite *sprite, vec2_t pos, vec2_t v, Uint32 fuse, int damage )
{
	Entity *self = NULL;

	self = NewEnt();
	if( self == NULL )
	{
		fprintf( stderr, "InitProjectile: ERROR: reached ent cap\n" );
		return;
	}

//	self->name = "projectile";

	self->self = self;
	self->owner = owner;

	self->sprite = sprite;
	self->visible = 1;

	self->w = self->sprite->w;
	self->h = self->sprite->h;

	Vec2Copy( pos, self->position );
	Vec2Copy( v, self->velocity );

	self->Move = ProjectileMove;

	if( fuse )
	{
		self->nextthink = NOW + fuse;
	}
	else
	{
		self->nextthink = NOW + 5000;
	}

	self->Think = FreeEnt;
}


void ProjectileMove( Entity *self )
{
	Vec2Add( self->position, self->velocity, self->position );
}
