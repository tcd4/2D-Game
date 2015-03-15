/**
 * @file boss.h
 *
 * @brief file containing functions to control the bosses
 */

#ifndef __BOSS__
#define __BOSS__


#include "entity.h"


typedef enum
{
	MOVE_NONE		= 0,
	MOVE_RANDOM		= 1,
	MOVE_PATH		= 2
}Move_Type;


struct Path
{
	vec2_t pathto;
	struct Path *next;
};


/**
 * @brief initializes the boss
 *
 * @param filename the file name and path to the boss definition file
 *
 * @return a pointer to the boss entity
 */
Entity *InitBoss( char *filename );

/**
 * @brief loads the specified boss
 *
 * @return a pointer to the boss entity
 */
void LoadBoss( Entity *self, char *filename );

/**
 * @brief calculates the velocity of the boss
 * 
 * @param self a pointer to the boss entity
 */
void CalculateVelocity( Entity *self );

/**
 * @brief removes the starting node from the path list
 */
void RemovePath();
#endif
