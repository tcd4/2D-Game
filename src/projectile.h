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
 * @brief initializes a projectile
 *
 * @param owner the owner of the projectile
 * @param group the group the projectile belongs to
 * @param sprite the projectile's sprite
 * @param pos the starting position of the projectile
 * @param v the velocity of the projectile
 * @param fuse the duration the projectile will last
 * @param damage the damage the projectile will do
 */
void InitProjectile( Entity *owner, int group, Sprite *sprite, vec2_t pos, vec2_t v, Uint32 fuse, int damage );

#endif
