#include "firingmode.h"
#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern Uint32 NOW;
extern SDL_Surface *screen;


int LoadFiringMode( FireMode *mode, char *filename )
{
	FILE *file = NULL;
	char buf[ 128 ];
	char path[ FILE_PATH_LEN ];
	int w, h;
	int subnum = 0;
	int fpl;
	FireMode *temp = NULL;

	if( mode )
	{
		FreeFireMode( mode );
	}

	file = fopen( filename, "r" );
	if( !file )
	{
		fprintf( stderr, "ERROR: LoadMode: could not open file: %s\n", filename );
		return 0;
	}

	while( fscanf( file, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), file );
		}
		else if( strncmp( buf, "name:", 128 ) == 0 )
		{
			fscanf( file, "%s", mode->name );
		}
		else if( strncmp( buf, "projectile:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
		}
		else if( strncmp( buf, "framesperline:", 128 ) == 0 )
		{
			fscanf( file, "%i", &fpl );
		}
		else if( strncmp( buf, "size:", 128 ) == 0 )
		{
			fscanf( file, "%i,%i", &w, &h );
			mode->proj = LoadSprite( path, w, h, fpl );
		}
		else if( strncmp( buf, "position:", 128 ) == 0 )
		{
			fscanf( file, "%f,%f", &mode->position[ 0 ], &mode->position[ 1 ] );
		}
		else if( strncmp( buf, "numProj:", 128 ) == 0 )
		{
			fscanf( file, "%i", &mode->numProj );
		}
		else if( strncmp( buf, "angle:", 128 ) == 0 )
		{
			fscanf( file, "%f", &mode->angle );
		}
		else if( strncmp( buf, "cone:", 128 ) == 0 )
		{
			fscanf( file, "%f", &mode->cone );
		}
		else if( strncmp( buf, "velocity:", 128 ) == 0 )
		{
			fscanf( file, "%f", &mode->velocity );
		}
		else if( strncmp( buf, "fuse:", 128 ) == 0 )
		{
			fscanf( file, "%i", &mode->fuse );
		}
		else if( strncmp( buf, "rate:", 128 ) == 0 )
		{
			fscanf( file, "%i", &mode->fireRate );
		}
		else if( strncmp( buf, "damage:", 128 ) == 0 )
		{
			fscanf( file, "%i", &mode->damage );
		}
		else if( strncmp( buf, "firemode:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );

			temp = ( FireMode * )malloc( sizeof( FireMode ) * (subnum + 1 ) );
			memcpy( temp, mode->concurrent_mode, sizeof( FireMode ) * subnum );
			mode->concurrent_mode = temp;
			LoadFiringMode( &mode->concurrent_mode[ subnum ], path );
			subnum++;
		}
		else if( strncmp( buf, "actor:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
			mode->actor = NewActor( path );
		}
	}

	fclose( file );

	mode->velocities = ( vec2_t * )malloc( sizeof( vec2_t ) * mode->numProj );
	if( !mode->velocities )
	{
		fprintf( stderr, "ERROR: LoadFiringMode: can't allocate memory for velocities\n" );
		return 0;
	}

	FindVelocities( mode );

	mode->concurrent_num = subnum;
	mode->loaded = 1;

	fprintf( stdout, "LoadFiringMode: %s firing mode loaded\n", mode->name );
	return 1;
}


void FindVelocities( FireMode *mode )
{
	int i;
	double vx, vy;
	float angle;
	float curangle;
	float decriment;

	angle = mode->angle - 90;
	angle = angle - ( mode->cone / 2 );
	angle *= TORAD;

	decriment = mode->cone / ( mode->numProj - 1 );
	decriment *= TORAD;

	for( i = 0; i < mode->numProj; i++ )
	{
		curangle = angle + ( decriment * i );

		vx = ( cos( curangle ) ) * mode->velocity;
		vy = ( sin( curangle ) ) * mode->velocity;

		mode->velocities[ i ][ 0 ] = vx;
		mode->velocities[ i ][ 1 ] = vy;
	}
}


void Fire( Entity *ent, FireMode *mode )
{
	vec2_t pos;
	int i;

	pos[ 0 ] = ( ent->w / 2 ) + ent->position[ 0 ] + mode->position[ 0 ];
	pos[ 1 ] = ent->position[ 1 ] - mode->position[ 1 ];

	for( i = 0; i < mode->numProj; i++ )
	{
		InitProjectile( ent, ent->group, mode->proj, pos, mode->velocities[ i ], mode->fuse, mode->damage ); 
	}

	for( i = 0; i < mode->concurrent_num; i++ )
	{
		Fire( ent, &mode->concurrent_mode[ i ] );
	}
}


void FreeFireMode( FireMode *mode )
{
	mode->loaded = 0;

	if( mode->proj )
	{
		FreeSprite( mode->proj );
	}

	if( mode->concurrent_mode )
	{
		FreeFireMode( mode->concurrent_mode );
	}
}
