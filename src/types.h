/**
 * @file types.h
 *
 * @brief file containing needed data types and functions to manipulate them
 */

#ifndef __TYPES__
#define __TYPES__


#include <math.h>


/* basic constants */
#define FILE_PATH_LEN 128
#define FILE_NAME_LEN 50
#define TYPE_NAME_LEN 30
#define PI 3.14159265
#define TAU 6.283185308
#define TODEG 57.2957795
#define TORAD 0.0174532925


/* vectors */
typedef float vec_t;
typedef vec_t vec2_t[ 2 ];
typedef vec_t vec3_t[ 3 ];
typedef vec_t vec4_t[ 4 ];

/* vectors functions and macros */
#define Vec2Subtract( a, b, c )	( c[ 0 ] = a[ 0 ] - b[ 0 ], c[ 1 ] = a[ 1 ] - b[ 1 ] )
#define Vec2Add( a, b, c )		( c[ 0 ] = a[ 0 ] + b[ 0 ], c[ 1 ] = a[ 1 ] + b[ 1 ] )
#define Vec2Copy( a, b )		( b[ 0 ] = a[ 0 ], b[ 1 ] = a[ 1 ] )
#define Vec2Clear( a )			( a[ 0 ] = 0, a[ 1 ] = 0 )
#define Vec2Compare( a, b )		( ( a[ 0 ] == b[ 0 ] && a[ 1 ] == b[ 1 ] ) ? 1: 0 )
#define Vec2Zeroed( a )			( ( a[ 0 ] == 0 && a[ 1 ] == 0 ) ? 1: 0 )

#define Vec3Subtract( a, b, c )	( Vec2Subtract( a, b, c ), c[ 2 ] = a[ 2 ] - b[ 2 ] )
#define Vec3Add( a, b, c )		( Vec2Add( a, b, c ), c[ 2 ] = a[ 2 ] + b[ 2 ] )
#define Vec3Copy( a, b )		( Vec2Copy( a, b ), b[ 2 ] = a[ 2 ] )
#define Vec3Clear( a )			( Vec2Clear( a ), a[ 2 ] = 0 )
#define Vec3Compare( a, b )		( ( Vec2Compare( a, b ) && a[ 2 ] == b[ 2 ] ) ? 1: 0 )
#define Vec3Zeroed( a )			( ( a[ 0 ] == a[ 1 ] == a[ 2 ] == 0 ) ? 1: 0 )

#define Vec4Subtract( a, b, c )	( Vec3Subtract( a, b, c ), c[ 3 ] = a[ 3 ] - b[ 3 ] )
#define Vec4Add( a, b, c )		( Vec3Add( a, b, c ), c[ 3 ] = a[ 3 ] + b[ 3 ] )
#define Vec4Copy( a, b )		( Vec3Copy( a, b ), b[ 3 ] = a[ 3 ] )
#define Vec4Clear( a )			( Vec3Clear( a ), a[ 3 ] = 0 )
#define Vec4Compare( a, b )		( ( Vec3Compare( a, b ) && a[ 3 ] == b[ 3 ] ) ? 1: 0 )
#define Vec4Zeroed( a )			( ( a[ 0 ] == a[ 1 ] == a[ 2 ] == a[ 3 ] == 0 ) ? 1: 0 )


/* random functions */
#define random()	( ( rand() & 0x7fff ) / ( ( float )0x7fff ) )
#define crandom()	( 2.0 * ( random() - 0.5 ) )


#endif
