/**
 * @file level.h
 *
 * @brief file containing a basic level structure and functions
 */

#ifndef __LEVEL__
#define __LEVEL__


#include "sprites.h"


#define LEVEL_NAME_LEN 80


typedef struct
{
	int			loaded;						/**< determines if a level is loaded */
	char		filename[ LEVEL_NAME_LEN ];	/**< image path for the level */
	Sprite		*bgImage;					/**< pointer to the background sprite */
	SDL_Rect	bounds;						/**< absolute bounds for entities in this level */
}Level;

/**
 * @brief loads a game level into memory and sets it up for drawing
 *
 * @param filename 
 */
void LoadLevel( char *filename );

/**
 * @brief draws the current level
 */
void DrawLevel();

/**
 * @brief unloads the current level
 */
void CloseLevel();

/**
 * @brief sets up the level system to a clean state
 */
void InitLevelSystem();


#endif
