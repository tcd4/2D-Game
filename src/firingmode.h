/**
 * @file firingmode.h
 *
 * @brief file containing a variables and functions for player firing modes
 */

#ifndef __FIRINGMODE__
#define __FIRINGMODE__


#include "entity.h"


typedef struct mode_s
{
	int		numProj;		/**< number of projectiles that are fired */
	Uint32	fireRate;		/**< how often the player can fire */
	Uint32	nextFire;		/**< when the player can fire next */
	vec2_t	*velocities;	/**< velocities of the projectiles */
	vec2_t	origin;			/**< spot to fire the projectiles */
	int		angle;			/**< cone of fire */
	Uint32	fuse;			/**< time until projectiles expire */
	int		damage;			/**< amount of damage each projectile does */
}Mode;

/**
 * @brief loads the firing mode into memory
 *
 * @param mode a pointer to the firing mode
 * @param filename the file name and path to the mode def file
 */
void LoadMode( Mode *mode, char *filename );

/**
 * @brief calculates the velocity each projectile is fired at
 *
 * @param mode a pointer to the firing mode
 * @param v the velocity of a bullet going straight up
 */
void FindVelocities( Mode *mode, int v );

/**
 * @brief fires the projectiles in the proper pattern
 *
 * @param ent a pointer to the player entity
 */
void Fire( Entity *ent, Mode *mode );

#endif
