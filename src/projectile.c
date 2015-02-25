#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern Uint32 NOW;


void ProjectileThink( Entity *self );
void ProjectileTouch( Entity *self, Entity *other );


void LoadProjectile( Entity *owner, char *filename )
{
	FILE *projfile = NULL;
	char buf[ 128 ];
	char projimagepath[ 128 ];
	int w, h;
	Sprite *temp;

	projfile = fopen( filename, "r" );
	if( projfile == NULL )
	{
		fprintf( stderr, "LoadProjectile: FATAL: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( projfile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), projfile );
		}
		else if( strncmp( buf, "sprite:", 128 ) == 0 )
		{
			fscanf( projfile, "%s", projimagepath );
		}
		else if( strncmp( buf, "width:", 128 ) == 0 )
		{
			fscanf( projfile, "%i", &w );
		}
		else if( strncmp( buf, "height:", 128 ) == 0 )
		{
			fscanf( projfile, "%i", &h );
		}
	}

	fclose( projfile );

	temp = LoadSprite( projimagepath, w, h );
	if( !temp )
	{
		fprintf( stderr, "LoadProjectile: FATAL: could not open sprite file: %s\n", projimagepath );
		exit( -1 );
	}

	owner->projectile = temp;
}
