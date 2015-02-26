/**
 * @file player.h
 *
 * @brief file containing functions to control the player's
 * character
 */

#ifndef __PLAYER__
#define __PLAYER__


#include "entity.h"


#define MAX_VELOCITY 5


typedef enum
{
	MOVE_NO			= 0,
	MOVE_UP			= 1,
	MOVE_DOWN		= 2,
	MOVE_LEFT		= 4,
	MOVE_RIGHT		= 8,
	MOVE_TUG_OF_WAR = 15
}Direction;


/**
 * @brief initializes the player
 */
void InitPlayer();

/**
 * @brief loads the specfied player from character.txt
 *
 * @param self a pointer to the player entity
 * @param filename the file name and path to the player defintion file
 */
void LoadPlayer( Entity *self, char *filename);

/**
 * @brief checks the input to adjust the player
 *
 * @param self a pointer to the player entity
 */
void CheckInput( Entity *self );


#endif
