/**
 * @file player.h
 *
 * @brief file containing functions to control the player's
 * character
 */

#ifndef __PLAYER__
#define __PLAYER__


#include "entity.h"

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


#endif
