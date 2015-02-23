/**
 * @file mouse.h
 *
 * @brief file containing a basic structure and functions
 * for displaying the mouse
 */

#ifndef _MOUSE_
#define _MOUSE_


#include <SDL.h>
#include "graphics.h"
#include "sprites.h"


struct
{
	Uint32 state;	/**< the current state of the mouse */
	Uint32 shown;	/**< determines whether or not the mouse is visible */
	Uint32 frame;	/**< the current frame the mouse is on */
	Uint16  x, y;	/**< the x and y coordinates of the mouse on the screen */
}Mouse;


/**
 * @brief initializes the mouse
 */
void InitMouse();

/**
 * @brief draws to the screen immediately before the blit, after all
 * it wouldn't be a very good mouse if it got covered up by the
 * game content
 */
void DrawMouse();


#endif
