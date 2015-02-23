/**
 * @file entity.h
 *
 * @brief file containing a basic entity structure and functions
 */

#ifndef __ENTITY__
#define __ENTITY__


#include "sprites.h"
#include "vector.h"


#define MAX_ENTITIES	4096


typedef struct entity_s
{
	int				inuse;						/**< determines if the entities is in use */
	/*
	player variable
	*/

	char			*classname;		/**< name of the entity type */

	struct entity_s	*owner;			/**< pointer to the entity that owns this one */
	struct entity_s *opponent;		/**< pointer to the opponent entity */

	Sprite			*sprite;		/**< pointer to the entity's sprite */
	int				frame;			/**< frame the entity's sprite is on */
	int				numFrames;		/**< number of frames in a row of a sprite sheet */
	int				width, height;	/**< width and height of the sprite */

	SDL_Rect		bbox;			/**< bounding box for the entity */

	vec2_t			position;		/**< position of the entity */
	vec2_t			velocity;		/**< velocity of the entity */

	int				visible;		/**< determines if the entity can be seen or not */
	int				deadflag;		/**< determines if the entity has died */

	
	Uint32			thinkrate;		/**< determines how often the entity thinks */
	Uint32			nextthink;		/**< determines when the entity will think next */

	void			( *Think )( struct entity_s *self );							/**< pointer to the entity's think function */
	void			( *Touch )( struct entity_s *self, struct entity_s *other );	/**< pointer to the entity's collision function */
	void			( *Die )( struct entity_s *self );								/**< pointer to the entity's death function */
	void			( *World_Touch )( struct entity_s *self, vec2_t contact_point );/**< function to call when this entity touches the world */ 
}Entity;


/**
 * @brief initializes all the entities to nothing
 */
void	InitEntList();

/**
 * @brief initializes new entities
 *
 * @return a pointer to the new entity or NULL if the entList is full
 */
Entity	*newEnt();

/**
 * @brief draws all the used entities to the screen buffer
 */
void	DrawEntList();

/**
 * @brief draws an entity to the screen
 *
 * @param ent a pointer to the entity to be drawn
 */
void	DrawEnt( Entity *ent );

/**
 * @brief destroys all the used entities
 */
void	FreeEntList();

/**
 * @brief destroys all ents except 1
 *
 * @param ent the entity not to be destroyed
 */
void	FreeEntsExcept( Entity *ent );

/**
 * @brief destroys an entity
 *
 * @param ent a pointer to the entity that must be destroyed
 */
void	FreeEnt( Entity *ent );

/**
 * @brief allows entities to have a mind of their own
 */
void	ThinkEntities();


#endif
