/**
 * @file sprites.h
 *
 * @brief file containing a basic sprite structure and functions
 */

#ifndef __SPRITES__
#define __SPRITES__


#include <SDL.h>
#include "graphics.h"


#define SPRITE_NAME_LEN 80


typedef struct Sprite_T
{
	SDL_Surface		*image;						/**< pointer to the actual image in memory */
	char			filename[ SPRITE_NAME_LEN ];	/**< the name of the file, keep them short */
	int				w, h;						/**< the width and height of the frames of the sprites, not the file itself */
	int				framesperline;				/**< default is 16 */
	int				color1, color2, color3;		/**< if the file was palette swapped these are the colors that were changed */
	int				used;						/**< used by the maintanenc functions to keep track of how many times a single sprite is being used */
}Sprite;


Sprite *Msprite;	/**< pointer to the mouse sprite */


/**
 * @brief and now bringing it all together, we swap the pure colors in the sprite out
 * and put the new colors in.  This maintains any of the artist's shading and
 * detail, but still lets us have that old school palette swapping.
 *
 * @param sprite the sprite to have its color be swapped
 * @param color1 the first color to be swapped
 * @param color2 the second color to be swapped
 * @param color3 the third color to be swapped
 */
void SwapSprite( SDL_Surface *sprite, int color1, int color2, int color3 );

/**
 * @brief called when the program is first started.
 * It just sets everything to zero and sets all pointers to NULL.
 * It should never be called again.
*/
void InitSpriteList();

/**
 * @brief When we are done with a sprite, lets give the resources back to the system...
 * so we can get them again later.
 *
 * @param sprite a pointer to the sprite to be freed
 */
void FreeSprite( Sprite *img );

/**
 * @brief create a sprite from a file, the most common use for it.
 * 
 * @param filename the filename of the sprite sheet
 * @param sizex the width of the sprite on the screen
 * @param sizey the height of the sprite on the screen
 *
 * @return a pointer to the loaded sprite
 */
Sprite *LoadSprite( char *filename, int sizex, int sizey );	

/**
 * @brief the palette swapping version of LoadSprite.  
 * It will check the file loaded to see if there is any
 * pure colors for swapping them.
 * 
 * @param filename the filename of the sprite sheet
 * @param sizex the width of the sprite on the screen
 * @param sizey the height of the sprite on the screen
 * @param c1 the first color to swap
 * @param c2 the second color to swap
 * @param c3 the third color to swap
 *
 * @return a pointer to the loaded sprite
 */
Sprite *LoadSwappedSprite( char *filename, int sizex, int sizey, int c1, int c2, int c3 );

/**
 * @brief draws a sprite on the screen
 *
 * @param sprite a pointer to the sprite to be drawn
 * @param surface the screen buffer to that we will draw to
 * @param sx the x cooridinate to draw the sprite at
 * @param sy the y cooridinate to draw the sprite at
 * @param frame the frame of the sprite we need to draw
 */
void DrawSprite( Sprite *sprite, SDL_Surface *surface, int sx, int sy, int frame );

/**
 * @brief frees up all used sprites
 */
void CloseSprites();	


#endif
