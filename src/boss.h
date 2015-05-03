/**
 * @file boss.h
 *
 * @brief file containing functions to control the bosses
 */

#ifndef __BOSS__
#define __BOSS__


#include "entity.h"


#define MAX_ABILITIES 10
/**
 * @brief initializes the boss
 *
 * @param filename the file name and path to the boss definition file
 *
 * @return a pointer to the boss entity
 */
Entity *InitBoss( char *filename );

/**
 * @brief initializes the ability list
 */
void InitAbilityList();

/**
 * @brief frees all abilities in the ability list
 */
void ClearAbilityList();

/**
 * @brief loads the specified boss
 *
 * @return 1 if the boss loaded and 0 for anything else
 */
int LoadBoss( Entity *self, char *filename );

/**
 * @brief calculates the velocity of the boss
 * 
 * @param self a pointer to the boss entity
 */
void CalculateVelocity( Entity *self );

/**
 * @brief uses abilities that are in use and checks to see if they're done
 *
 * @param self a pointer to the boss entity
 */
void CheckAbilities( Entity *self );

/**
 * @brief causes the boss to use a random ability from available abilities
 *
 * @param self a pointer to the boss entity
 */
void UseRandomAbility( Entity *self );

/**
 * @brief stops a specific ability
 *
 * @param self a pointer to the boss entity
 * @param index the index of the ability in the abilityList
 */
void StopAbility( Entity *self, int index );

/**
 * @brief stops all abilities that can be stopped
 */
void EndAbilities();

/**
 * @brief stops all abilities that can be stopped
 */
void EndAllAbilities();

/**
 * @brief unlocks the lock on ability use
 */
void Unlock();

#endif
