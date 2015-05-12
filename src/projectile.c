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

	self->w = self->sprite->w;
	self->h = self->sprite->h;

	Vec2Copy( pos, self->position );
	Vec2Copy( v, self->velocity );

	self->damage = damage;

	self->Touch = ProjectileTouch;
	self->Move = ProjectileMove;

	self->canCollide = 1;
	self->group = group;
	self->visible = 1;
	self->self = self;

	if( fuse )
	{
		self->Think = FreeEnt;
		self->nextThink = NOW + fuse;
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
	FreeEnt( self );
}
