/**
 * @file ability.h
 *
 * @brief file containing functions to control a boss'
 * abilities
 */

#ifndef __ABILITY__
#define __ABILITY__


#include "entity.h"
#include "boss.h"


typedef struct ability_s
{
	Entity		*owner;		/**< pointer to the owner of the ability */
	int			inuse;		/**< determines if the ability is being used */
	int			lock;		/**< determines if other abilities can be used during an ability */

	char		*pattern;	/**< the pattern of the projectiles */
	int			numProj;	/**< the number of projectiles fired */
	int			cone;		/**< the angle the projectiles are fired within */
	int			velocity;	/**< the velocity of the projectiles */
	int			radius;		/**< the radius of the pattern */
	vec2_t		pos;		/**< the position projectiles are fired from relative to the owner */

	Uint32		duration;	/**< the duration of the ability */
	Uint32		startTime;	/**< when the ability begins */
	Uint32		endTime;	/**< when the ability ends */
	Uint32		firerate;	/**< how often the ability is used during the duration */
	Uint32		nextfire;	/**< the next time the ability will be used during the duration */
	Uint32		fuse;		/**< the duration projectiles last for */
	Uint32		cooldown;	/**< the time it takes for the owner to use another ability */

	char		*delimiter;	/**< declares what determines the next stage of the abilty */
	
	Move_Type	movement;	/**< where the owner will move during the ability */
}Ability;


/**
 * @brief loads the ability into memory
 *
 * @param ability a pointer to the ability
 * @param filename the file name and path to the ability def file
 * @param owner a pointer to the owner of the ability
 */
void LoadAbility( Ability *ability, char *filename, Entity *owner );

/**
 * @brief use an ability
 *
 * @param ability a pointer to the ability
 */
void UseAbility( Ability *ability );

#endif
