#include "firingmode.h"
#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern Uint32 NOW;


void LoadMode( Mode *mode, char *filename )
{
	FILE *modefile = NULL;
	char buf[ 128 ];
	int v;

	modefile = fopen( filename, "r" );
	if( modefile == NULL )
	{
		fprintf( stderr, "LoadMode: FATAL: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( modefile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), modefile );
		}
		else if( strncmp( buf, "numProj:", 128 ) == 0 )
		{
			fscanf( modefile, "%i", &mode->numProj );
		}
		else if( strncmp( buf, "rate:", 128 ) == 0 )
		{
			fscanf( modefile, "%i", &mode->fireRate );
		}
		else if( strncmp( buf, "v:", 128 ) == 0 )
		{
			fscanf( modefile, "%i", &v );
		}
		else if( strncmp( buf, "angle:", 128 ) == 0 )
		{
			fscanf( modefile, "%i", &mode->angle );
		}
		else if( strncmp( buf, "fuse:", 128 ) == 0 )
		{
			fscanf( modefile, "%i", &mode->fuse );
		}
		else if( strncmp( buf, "damage:", 128 ) == 0 )
		{
			fscanf( modefile, "%i", &mode->damage );
		}
	}

	fclose( modefile );

	mode->velocities = (vec2_t *)malloc( sizeof( vec2_t ) * mode->numProj );
	if( mode->velocities == NULL )
	{
		fprintf( stderr, "LoadMode: FATAL: cannot allocate velocities\n" );
		exit( -1 );
	}

	if( !mode->fuse )
	{
		mode->fuse = 0;
	}

	mode->velocities[ 0 ][ 1 ] = -v;
	mode->velocities[ 0 ][ 0 ] = 0;
	mode->nextFire = NOW + mode->fireRate;
	FindVelocities( mode, v );
}


void FindVelocities( Mode *mode, int v )
{
	int i;
	double vx, vy;
	double ang = ( mode->angle / 2 ) - 90;
	double angrad;
	double dec = mode->angle / ( mode->numProj - 1 );

	for( i = 0; i < mode->numProj; i++ )
	{
		Vec2Clear( mode->velocities[ i ] );
		angrad = ang * ( PI / 180 );

		vx = ( cos( angrad ) ) * v;
		vy = ( sin( angrad ) ) * v;

		mode->velocities[ i ][ 0 ] = vx;
		mode->velocities[ i ][ 1 ] = vy;

		ang -= dec;
	}
}


void Fire( Entity *ent, Mode *mode )
{
	int i;

	Vec2Copy( ent->position, mode->origin );
	mode->origin[ 0 ] += ( ent->w / 2 );

	for( i = 0; i < mode->numProj; i++ )
	{
		//InitProjectile( ent, ent->opponent, ent->projectile, mode->origin, mode->velocities[ i ], mode->fuse, mode->damage );
	}
}
