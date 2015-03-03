/**
 * @file vector.h
 *
 * @brief file containing a basic tools for handling vectors
 */

#ifndef __VECTOR__
#define __VECTOR__


#include <math.h>


#define PI 3.14159265


/**
 * @purpose to provide a basic 2D vector structure
 */
typedef int vec_t;
typedef vec_t vec2_t[ 2 ];


#define VectorSubtract(a,b,c)	(c[0]=a[0]-b[0],c[1]=a[1]-b[1])
#define VectorAdd(a,b,c)		(c[0]=a[0]+b[0],c[1]=a[1]+b[1])
#define VectorCopy(a,b)			(b[0]=a[0],b[1]=a[1])
#define VectorClear(a)			(a[0]=a[1]=0)


#endif
