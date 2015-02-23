/**
 * @file graphics.h
 *
 * @brief file containing a basic graphics functions
 */

#ifndef __GRAPHICS__
#define __GRAPHICS__
/*
 *			graphics.h
 *			Originaly written by Donald Kehoe for the NJIT Introduction to game development class
 *			This file contains data structures, constants and function prototypes needed to initialize SDL and
 *			to handle sprites and graphics.
 */


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


/*uint32*/
#define Red_         0xDD0000
#define Green_       0x00DD00
#define Blue_        0x0000DD
#define Yellow_      0xDDDD00
#define Orange_      0xEE5522
#define Violet_      0xDD00DD
#define Brown_       0x663311
#define Grey_        0x888888
#define DarkRed_     0x880000
#define DarkGreen_   0x008800
#define DarkBlue_    0x000088
#define DarkYellow_  0x888800
#define DarkOrange_  0x774411
#define DarkViolet_  0x660066
#define DarkBrown_   0x442205
#define DarkGrey_    0x444444
#define LightRed_    0xFF2222
#define LightGreen_  0x33FF33
#define LightBlue_   0x3333FF
#define LightYellow_ 0xFFFF11
#define LightOrange_ 0xFFDD11
#define LightViolet_ 0xFF55FF
#define LightBrown_  0x886622
#define LightGrey_   0xBBBBBB
#define Black_       0x000000
#define White_       0xFEFEFE
#define Tan_         0xFFDD22
#define Gold_        0xBB9900
#define Silver_      0xAAAABB
#define YellowGreen_ 0x44EE01
#define Cyan_        0x00FFFF
#define Magenta_     0xFF00FF


enum colors { Red = 1, Green = 2, Blue = 3, Yellow = 4, Orange = 5, Violet = 6, Brown = 7, Grey = 8,
				DarkRed = 9, DarkGreen = 10, DarkBlue = 11, DarkYellow = 12, DarkOrange = 13,
				DarkViolet = 14, DarkBrown = 15, DarkGrey = 16, LightRed =17, LightGreen = 18,
				LightBlue = 19, LightYellow = 20, LightOrange = 21, LightViolet = 22, LightBrown = 23,
				LightGrey = 24, Black = 25, White = 26, Tan = 27, Gold = 28, Silver = 29, YellowGreen = 30,
				Cyan = 31, Magenta = 32 };


typedef struct	/*this didn't need to be its own type, its only used once*/
{
    int xres, yres, depth;
}ScreenData;


/*
 * @brief initializes the screen
 */
void Init_Graphics();

/*
 * @brief draws a pixel on the screen
 *
 * @param screen a pointer to the screen
 * @param R the red value
 * @param G the green value
 * @param B the blue value
 * @param x the x coordinate to place the pixel
 * @param y the y coordinate to place the pixel
 */
void DrawPixel( SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y );

/*
 * @brief turns the entire screen to a single color
 *
 * @param buf a pointer to the screen buffer
 * @param color the color to turn the screen
 */
void BlankScreen( SDL_Surface *buf, Uint32 color );

/*
 * @brief gets the hex value of a color
 *
 * @param color the color to get the value of
 */
Uint32 IndexColor( int color );

/*
 * @brief determines when the next frame should be drawn
 *
 * @param delay the amount of time to wait for the next frame to be drawn
 */
void FrameDelay( Uint32 delay );

/*
 * @brief resets the screen buffer
 */
void ResetBuffer();

/*
 * @brief creates the next game frame
 */
void NextFrame();


#endif
