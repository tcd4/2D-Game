/**
 * @file firingmode.h
 *
 * @brief file containing a variables and functions for player firing modes
 */

#ifndef __FIRINGMODE__
#define __FIRINGMODE__


#include "entity.h"


#define MAX_POSITIONS 5


typedef struct firing_mode_s
{
	int						loaded;					/**< determines if the firing mode is loaded */
	char					name[ TYPE_NAME_LEN ];	/**< the name of the firing mode */
	Sprite					*proj;					/**< the proj sprite to use */
	vec2_t					position;				/**< position to fire the projectiles from offset from the center of the player */
	int						numProj;				/**< number of projectiles that are fired */
	float					angle;					/**< angle of fire for a single angle */
	float					cone;					/**< cone of fire for multiple angles */
	float					velocity;				/**< velocity of the projectiles */
	vec2_t					*velocities;			/**< the vector velocities of each projectile */
	Uint32					fireRate;				/**< how often the player can fire */
	Uint32					fuse;					/**< time until projectiles expire */
	int						damage;					/**< amount of damage each projectile does */
	Actor					*actor;					/**< animation data for a projectile */
	int						concurrent_num;			/**< number of sub-modes */
	struct firing_mode_s	*concurrent_mode;		/**< gives firing modes sub-modes */	
}FireMode;


/**
 * @brief loads the firing mode into memory
 *
 * @param mode the firing mode to load
 * @param filename the file name and path to the mode def file
 *
 * @return returns 1 if the mode was loaded and 0 if not
 */
int LoadFiringMode( FireMode *mode, char *filename );

/**
 * @brief calculates the velocity each projectile is fired at
 *
 * @param mode the firing mode to use
 */
void FindVelocities( FireMode *mode );

/**
 * @brief fires the projectiles in the proper pattern
 *
 * @param ent a pointer to the player entity
 */
void Fire( Entity *ent, FireMode *mode );

/**
 * @brief frees a firing mode
 *
 * @param mode the firing mode to free
 */
void FreeFireMode( FireMode *mode );

#endif
