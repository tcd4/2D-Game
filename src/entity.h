/**
 * @file entity.h
 *
 * @brief file containing a basic entity structure and functions
 */

#ifndef __ENTITY__
#define __ENTITY__


#include "sprites.h"
#include "types.h"
#include "path.h"
#include "actor.h"


#define MAX_ENTITIES	4096
#define MAX_ACTORS		10


typedef struct entity_s
{
	int				inuse;					/**< determines if the entities is in use */

	char			name[ TYPE_NAME_LEN ];	/**< name of the entity type */

	struct entity_s *self;					/**< pointer to itself */
	struct entity_s	*owner;					/**< pointer to the entity that owns this one */

	Sprite			*sprite;				/**< pointer to the entity's sprite */
	Actor			*actors[ MAX_ACTORS ];	/**< actors for the entity */
	int				state;					/**< what the entity is currently doing */
	int				numActors;				/**< the number of actors an entity has */
	int				w, h;					/**< width and height of the sprite */

	int				group;					/**< determines the group the entity is in */
	int				canCollide;				/**< determines if the entity can collide */
	vec4_t			bbox;					/**< bounding box for the entity */
	vec2_t			offset;					/**< bounding box offset */				
	vec2_t			velocity;				/**< velocity of the entity */
	vec2_t			position;				/**< position of the entity */
	vec2_t			origin;					/**< center of the entity */
	int				trapped;				/**< determines if the entity can leave the screen */
	
	Uint32			movetype;				/**< move direction of the entity */

	int				health;					/**< how much health an entity has */
	int				damage;					/**< how much damage an entity does */
	
	int				visible;				/**< determines if the entity can be seen or not */
	int				deadflag;				/**< determines if the entity has died */

	Uint32			thinkrate;				/**< determines how often the entity thinks */

	void			( *Draw )( struct entity_s *self );								/**< pointer to the entity's draw function */
	void			( *Free )( struct entity_s *self );								/**< pointer to the entity's free function */
	void			( *Think )( struct entity_s *self );							/**< pointer to the entity's think function */
	void			( *Touch )( struct entity_s *self, struct entity_s *other );	/**< pointer to the entity's collision function */
	void			( *Die )( struct entity_s *self );								/**< pointer to the entity's death function */
	void			( *Move )( struct entity_s *self );								/**< pointer to the entity's move function */
}Entity;


/**
 * @brief initializes all the entities to nothing
 */
void InitEntitySystem();

/**
 * @brief initializes new entities
 *
 * @return a pointer to the new entity or NULL if the entList is full
 */
Entity *NewEnt();

/**
 * @brief draws all the used entities to the screen buffer
 */
void DrawEntList();

/**
 * @brief draws an entity to the screen
 *
 * @param ent a pointer to the entity to be drawn
 */
void DrawEnt( Entity *ent );

/**
 * @brief destroys all the used entities
 */
void FreeEntList();

/**
 * @brief destroys all ents except 1
 *
 * @param ent the entity not to be destroyed
 */
void FreeEntsExcept( Entity *ent );

/**
 * @brief destroys an entity
 *
 * @param ent a pointer to the entity that must be destroyed
 */
void FreeEnt( Entity *ent );

/**
 * @brief allows entities to have a mind of their own
 */
void UpdateEnts();

/**
 * @brief changes which state an entity is in
 *
 * @param ent the entity that is changing states
 */
void ChangeState( Entity *ent );

/**
 * @brief moves the entities in the world
 */
void MoveEnts();

/**
 * @brief moves a single entity in the world
 *
 * @param ent the entity that is moving
 */
void MoveEnt( Entity *ent );

/**
 * @brief checks if an entity is leaving the stage
 *
 * @param ent the entity to check for
 */
int OutOfBounds( Entity *ent );

/**
 * @brief changes the animation state of the entity
 *
 * @param ent the entity to change
 * @param state the state to change to
 */
void ChangeState( Entity *ent, char *state );

#endif
