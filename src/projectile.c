#include <string.h>
#include <stdio.h>
#include "projectile.h"


extern Uint32 NOW;
extern SDL_Surface *screen;


void ProjectileTouch( Entity *self, Entity *other );
void ProjectileMove( Entity *self );


void InitProjectile( Entity *owner, int group, Sprite *sprite, vec2_t pos, vec2_t v, Uint32 fuse, int damage )
{
	Entity *self = NULL;

	self = NewEnt();
	if( !self )
	{
		fprintf( stderr, "ERROR: InitProjectile: could not load projectile\n" );
		return;
	}

	strncpy( self->name, "projectile", TYPE_NAME_LEN );
	self->owner = owner;
	self->sprite = sprite;
	
	Vec2Copy( pos, self->position );
	Vec2Copy( v, self->velocity );

	self->w = self->sprite->w;
	self->h = self->sprite->h;

	self->damage = damage;

	self->Touch = ProjectileTouch;
	self->Move = ProjectileMove;

	self->visible = 1;
	self->self = self;

	if( fuse )
	{
		self->Think = FreeEnt;
		self->thinkrate = fuse;
	}
}


void ProjectileMove( Entity *self )
{
	if( OutOfBounds( self ) )
	{
		FreeEnt( self );
	}
}


void ProjectileTouch( Entity *self, Entity *other )
{
}
