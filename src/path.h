/**
 * @file path.h
 *
 * @brief file containing functions and stucts to facilitate pathing
 */

#ifndef __PATH__
#define __PATH__


#include "vector.h"
#include "entity.h"


/**
 * @brief defines movement types that entities can use
 */
typedef enum
{
	MOVE_NO			= 0,
	MOVE_UP			= 1,
	MOVE_DOWN		= 2,
	MOVE_LEFT		= 4,
	MOVE_RIGHT		= 8,
	MOVE_RANDOM		= 16,
	MOVE_PATH		= 31
}Move_Type;


/**
 * @brief linked list for pathing information
 */
struct path_s
{
	vec2_t			pos;
	struct path_s	*next;
};


/**
 * @brief gets called when an entity needs to know where to go next in the path
 *
 * @param ent a pointer to the entity
 */
void NextPosition( struct entity_s *ent );

/**
 * @brief frees an entire path
 *
 * @param path a pointer to the path to be freed
 *
 * @return returns NULL;
 */
struct path_s *ClearPathing( struct path_s *path );

/**
 * @brief adds a node to the path
 *
 * @param ent a pointer to the entity whose pathing will be updated
 */
void AddPath( struct entity_s *ent, vec2_t point );


#endif
