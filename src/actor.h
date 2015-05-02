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
	int			inuse;						/**< determines if the actor is being used */
	int			done;						/**< determines if the actor is finish being player */
	char		action[ TYPE_NAME_LEN ];	/**< the action the actor does */
	int			numFrames;					/**< the number of frames the actor uses */
	int			frame;						/**< the current frame the actor is on */
	int			frameRate;					/**< how often the animation updates */
	int			animStart;					/**< the frame to start on */
	int			animEnd;					/**< the frame to end on */
	int			direction;					/**< the direction the animation is being player */
	int			type;						/**< the Anim_Type the actor is */
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

/**
 * @brief resets an actor
 */
void ResetActor( Actor *actor );


#endif
