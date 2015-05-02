/**
 * @file player.h
 *
 * @brief file containing functions to control the player's
 * character
 */

#ifndef __PLAYER__
#define __PLAYER__


#include "entity.h"


#define MAX_FIRE_MODES 3


/**
 * @brief initializes the player
 *
 * @return a pointer to the player entity
 */
Entity *InitPlayer( char *filename );

/**
 * @brief loads the specfied player
 *
 * @param self a pointer to the player entity
 * @param filename the file name and path to the player defintion file
 *
 * @return 1 if the player loaded and 0 if they did not
 */
int LoadPlayer( Entity *self, char *filename);

/**
 * @brief checks the input to adjust the player
 *
 * @param self a pointer to the player entity
 */
void CheckInput( Entity *self );

/**
 * @brief initializes the firing mode list
 */
void InitFireModeList();

/**
 * @brief clears the firing mode list and frees the firing modes
 */
void ClearFireModeList();


#endif
