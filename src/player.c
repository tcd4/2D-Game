#include "player.h"


void InitPlayer()
{
	Entity *self = newEnt();

	self->classname = "player";
	self->height = 40;
	self->width = 44;
	self->sprite = LoadSprite( "images/character1.png", self->width, self->height );
	self->visible = 1;
	self->frame = 0;
	self->position[ 0 ] = 220 - ( self->width / 2 );
	self->position[ 1 ] = 480 - self->height;
	self->numFrames = 8;
}
