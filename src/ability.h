/**
 * @file ability.h
 *
 * @brief file containing functions to control a boss'
 * abilities
 */

#ifndef __ABILITY__
#define __ABILITY__


#include "entity.h"


typedef struct delimiter_s
{
	struct ability_s	*owner;					/**< the ability that uses the delimiter */
	int					inuse;					/**< determines if the delimiter is being used */
	char				type[ TYPE_NAME_LEN ];	/**< the type of delimiter */
	int					uses;					/**< the number of uses for the delimiter */
	int					totalUses;				/**< the total number of uses for the delimiter */

	int					numProj;				/**< the change in number of projectiles */
	float				velocity;				/**< the change in velocity for projectiles */

	Uint32				duration;				/**< the change in duration */
	Uint32				fireRate;				/**< the change in fire rate*/
	Uint32				fuse;					/**< the change in fuse time */
	Uint32				cooldown;				/**< the change in cooldown */
		
	float				cone;					/**< the change in the cone of fire */
	float				radius;					/**< the change in the radius */
	float				angle;					/**< the change in the angle */
}Delimiter;


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
	Uint32				lock;						/**< determines if the ability prevents other abilities from being used */
	int					numProj;					/**< the number of projectiles fired */
	Uint32				fuse;						/**< the duration projectiles last for */
	

	char				pattern[ TYPE_NAME_LEN ];	/**< the pattern of the projectiles */

	/* point variables */
	float				angle;						/**< the angle projectiles are fired at */
	float				cone;						/**< the cone of fire for multiple angles */

	/* circle variables */
	float				radius;						/**< the radius of the circle */
	vec2_t				*base;						/**< the base positions */
	vec2_t				*positions;					/**< the positions the projectiles start at */

	/* custom variables */
	float				*angles;					/**< the angles to fire at */
	int					currentFire;				/**< the projectile to fire at the moment */
	
	Delimiter			*delimiter;					/**< the ability's delimiter */
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
 * @param filename the filename and path of the def file
 */
int LoadCustom( Ability *ability, char *filename );


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


/**
 * @brief checks to see if the delimiter should be used
 *
 * @param ability the ability whose delimiter we need to check
 */
void CheckDelimiter( Ability *ability );

/**
 * @brief adds the delimiter to the ability and reloads the ability
 *
 * @param ability the ability to add to
 */
void AddDelimiter( Ability *ability );


#endif
