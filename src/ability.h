/**
 * @file ability.h
 *
 * @brief file containing functions to control a boss'
 * abilities
 */

#ifndef __ABILITY__
#define __ABILITY__


#include "entity.h"
#include "delimiter.h"


typedef struct ability_s
{
	Entity				*owner;						/**< pointer to the owner of the ability */
	char				name[ TYPE_NAME_LEN ];		/**< the name of the ability */
	int					inuse;						/**< determines if the ability is being used */
	int					loaded;						/**< determines if the ability is loaded */

	char				anim[ TYPE_NAME_LEN ];		/**< the animation played during the ability */
	Sprite				*proj;						/**< the projectile the ability uses */

	int					concurrent_num;				/**< the number of concurrent abilities */
	struct ability_s	*concurrent_ability;		/**< abilities that are used simultaneously */

	int					relative;					/**< determines if the ability position is relative to the boss' position or the screen */
	vec2_t				position;						/**< the position of the ability's origin */
	float				velocity;					/**< the velocity of projectiles */
	vec2_t				*velocities;				/**< the vector velocities of each projectile */

	Uint32				duration;					/**< the duration of the ability */
	Uint32				cooldown;					/**< the time it takes for the owner to be able to use another ability */
	Uint32				fireRate;					/**< how often the ability is used during the duration */
	Uint32				nextFire;					/**< the next time the ability will be used during the duration */
	Uint32				startTime;					/**< when the ability begins */
	Uint32				endTime;					/**< when the ability ends */

	int					numProj;					/**< the number of projectiles fired */
	Uint32				fuse;						/**< the duration projectiles last for */
	

	char				pattern[ TYPE_NAME_LEN ];	/**< the pattern of the projectiles */

	/* point variables */
	float				angle;						/**< the angle projectiles are fired at */
	float				cone;						/**< the cone of fire for multiple angles */

	/* circle variables */
	float				radius;						/**< the radius of the circle */
	vec2_t				*base;						/**< the base positions the circle uses */
	vec2_t				*positions;					/**< the positions the projectiles start at around the circle */

	
	
	

	
	
	
	
	
	
	

	char			*delimiter;	/**< declares what determines the next stage of the abilty */
	
	Move_Type		movement;	/**< where the owner will move during the ability */
	struct path_s	*path;		/**< the path the owner will take while using the ability */
}Ability;


/**
 * @brief loads the ability into memory
 *
 * @param ability a pointer to the ability
 * @param filename the file name and path to the ability def file
 * @param owner a pointer to the owner of the ability
 *
 * @return 1 if ability loaded and 0 if else
 */
int LoadAbility( Ability *ability, char *filename, Entity *owner );

/**
 * @brief loads a point pattern ability
 *
 * @param ability the ability to load
 */
int LoadPoint( Ability *ability );

/**
 * @brief loads a circle pattern ability
 *
 * @param ability the ability to load
 */
int LoadCircle( Ability *ability );

/**
 * @brief loads a custom pattern ability
 *
 * @param ability the ability to load
 */
int LoadCustom( Ability *ability );


/**
 * @brief frees an ability
 *
 * @param ability the ability to free
 */
void FreeAbility( Ability *ability );


/**
 * @brief starts an ability
 *
 * @param ability the ability to start
 */
void StartAbility( Ability *ability );

/**
 * @brief uses an ability
 *
 * @param ability the ability to use
 */
void UseAbility( Ability *ability );

/**
 * @brief ends and resets an ability
 *
 * @param ability the ability to end
 */
void EndAbility( Ability *ability );


/**
 * @brief fires an ability that has a point pattern
 *
 * @param ability the ability to fire
 * @param firepos the firing position
 */
void FirePointAbility( Ability *ability, vec2_t firepos );

/**
 * @brief fires an ability that has a circle pattern
 *
 * @param ability the ability to fire
 * @param firepos the firing position
 */
void FireCircleAbility( Ability *ability, vec2_t firepos );

/**
 * @brief fires an ability that has a custom pattern
 *
 * @param ability the ability to fire
 * @param firepos the firing position
 */
void FireCustomAbility( Ability *ability, vec2_t firepos );


#endif
