/**
 * @file actor.h
 *
 * @brief file containing actor system for animation control
 */

#ifndef __ACTOR__
#define __ACTOR__


#include "types.h"


/**
 * @brief defines the animation types
 */
typedef enum
{
	ANIM_NO		= 0,
	ANIM_PASS	= 1,
	ANIM_LOOP	= 2,
	ANIM_OSCI	= 3
}Anim_Type;


/**
 * @brief defines the Actor struct for animation handling
 */
typedef struct
{
	int			inuse;
	char		action[ TYPE_NAME_LEN ];
	int			numFrames;
	int			frame;
	int			frameRate;
	int			animStart;
	int			animEnd;
	int			direction;
	int			type;
}Actor;

/**
 * @brief creates a new actor
 *
 * @param filename the file name and path of the actor
 *
 * @return a pointer to the newly created actor
 */
Actor *NewActor( char *filename );

/**
 * @brief updates the actor
 *
 * @param actor the actor to use
 *
 * @return the current frame of the animation
 */
int UseActor( Actor *actor );


#endif
