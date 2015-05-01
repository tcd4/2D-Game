/**
 * @file level.h
 *
 * @brief file containing a basic level structure and functions
 */

#ifndef __LEVEL__
#define __LEVEL__


#include "sprites.h"
#include "types.h"


#define MAX_LEVELS 5


typedef struct level_s
{
	int			loaded;						/**< determines if a level is loaded */
	char		filename[ FILE_NAME_LEN ];	/**< image path for the level */
	char		name[ TYPE_NAME_LEN ];		/**< the name of the level */

	int			active;						/**< determines if the level is being used */
	int			visible;					/**< determines if the level can be seen */

	Sprite		*background;				/**< the level's background */
	Sprite		*layers;					/**< the level's layers */
	int			layerCount;					/**< the number of layers the level has */

	vec2_t		position;					/**< the position of the level in the world */
	int			w, h;						/**< the size of the level */
	vec4_t		bounds;						/**< absolute bounds for entities in this level */

	void		( *Update )( struct level_s *self );	/**< pointer to the level's update function */
}Level;


/**
 * @brief loads a game level into memory and sets it up for drawing
 *
 * @param filename file name and path of the level def
 */
Level *LoadLevel( char *filename );

/**
 * @brief loads the level's background
 *
 * @param filename file name of the background image
 * @param level the level to load the background for
 */
void LoadBackground( char *filename, Level *level );

/**
 * @brief loads the level's layer
 *
 * @param filename file name of the layer image
 * @param level the level to load the layer for
 */
void LoadLayer( char *filename, Level *level );

/**
 * @brief draws all loaded levels
 */
void DrawLevels();

/**
 * @brief draws a level
 *
 * @param level the level to draw
 */
void DrawLevel( Level *level );

/**
 * @brief frees a level
 *
 * @param level the level to free
 */
void FreeLevel( Level *level );

/**
 * @brief closes the level system and frees all loaded levels
 */
void CloseLevelSystem();

/**
 * @brief initializes a level
 */
Level *InitLevel();

/**
 * @brief sets up the level system to a clean state
 */
void InitLevelSystem();


#endif
