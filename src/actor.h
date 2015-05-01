/**
 * @file actor.h
 *
 * @brief file containing actor system for animation control
 */

#ifndef __ACTOR__
#define __ACTOR__


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
	char		*action;
	int			numFrames;
	int			frame;
	int			frameRate;
	int			animStart;
	int			animEnd;
	Anim_Type	type;
}Actor;

/**
 * @brief creates a new actor
 *
 * @return a pointer to the newly created actor
 */
Actor *NewActor();

/**
 * @brief frees an actor
 *
 * @param actor the actor to free
 */
void FreeActor( Actor * actor );


#endif
