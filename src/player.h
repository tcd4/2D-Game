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
#define MODE_SWITCH_CD 500


/**
 * @brief initializes the player
 *
 * @return a pointer to the player entity
 */
Entity *InitPlayer();

/**
 * @brief loads the specfied player
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
