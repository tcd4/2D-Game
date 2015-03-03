/**
 * @file projectile.h
 *
 * @brief file containing functions to control projectile's
 * character
 */

#ifndef __PROJECTILE__
#define __PROJECTILE__

#include "entity.h"

/**
 * @brief loads the character's projectile
 *
 * @param self a pointer to the projectile's owner
 * @param filename the file name and path to the projectile definition file
 */
void LoadProjectile( Entity *owner, char *filename );

/**
 * @brief initializes a projectile
 *
 * @param owner a pointer to the owner of the projectile
 * @param opponent a pointer to the opponent of the projectile
 * @param sprite a pointer to the projectile's sprite
 * @param pos the starting position of the projectile
 * @param movedir the move direction of the projectile
 * @param v the velocity of the projectile
 */
void InitProjectile( Entity *owner, Entity *opponent, Sprite *sprite, vec2_t pos, vec2_t v, Uint32 fuse);

#endif
