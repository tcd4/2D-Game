#include "vector.h"


int VectorCompare( vec2_t a, vec2_t b )
{
	if( ( a[ 0 ] != b[ 0 ] ) || ( a[ 1 ] != b[ 1 ] ) )
	{
		return 0;
	}

	return 1;
}
