#include "ability.h"
#include "projectile.h"
#include <string.h>
#include <stdio.h>


extern Uint32 NOW;


void PatternPoint( Ability *ability );
void PatternLine( Ability *ability );
void PatternCircle( Ability *ability );


void LoadAbility( Ability *ability, char *filename, Entity *owner )
{
	FILE *abilityfile = NULL;
	char buf[ 128 ];
	char pat[ 128 ];

	abilityfile = fopen( filename, "r" );
	if( abilityfile == NULL )
	{
		fprintf( stderr, "LoadAbility: Fatal: could not open file: %s\n", filename );
		exit( -1 );
	}

	while( fscanf( abilityfile, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), abilityfile );
		}
		else if( strncmp( buf, "pattern:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%s", pat );
		}
		else if( strncmp( buf, "numProj:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->numProj );
		}
		else if( strncmp( buf, "pos:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%f,%f", &ability->pos[ 0 ], &ability->pos[ 1 ] );
		}
		else if( strncmp( buf, "radius:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->radius );
		}
		else if( strncmp( buf, "v:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->velocity );
		}
		else if( strncmp( buf, "angle:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->cone );
		}
		else if( strncmp( buf, "rate:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->firerate );
		}
		else if( strncmp( buf, "dur:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->duration );
		}
		else if( strncmp( buf, "cd:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->cooldown );
		}
	}

	fclose( abilityfile );

	ability->owner = owner;
	ability->inuse = 0;
	ability->endTime = 0;

	ability->pattern = (char *)malloc( sizeof( pat ) );
	strcpy( ability->pattern, pat );

	if( !ability->startTime )
	{
		ability->startTime = 0;
	}

	if( !ability->duration )
	{
		ability->duration = 0;
	}
	
	if( !ability->lock )
	{
		ability->lock = 0;
	}
}


void UseAbility( Ability *ability )
{
	if( !ability->endTime )
	{
		ability->endTime = NOW + ability->duration;
	}
	else if( ability->endTime <= NOW )
	{
		ability->inuse = 0;
		ability->startTime = 0;
		ability->endTime = 0;
		ability->nextfire = 0;
	}

	if( ability->firerate )
	{
		if( ability->nextfire >= NOW )
		{
			return;
		}

		ability->nextfire = ability->firerate + NOW;
	}

	if( strncmp( ability->pattern, "point", sizeof( ability->pattern ) ) == 0 )
	{
		PatternPoint( ability );
	}
	else if( strncmp( ability->pattern, "line", sizeof( ability->pattern ) ) == 0 )
	{
		PatternLine( ability );
	}
	else if( strncmp( ability->pattern, "circle", sizeof( ability->pattern ) ) == 0 )
	{
		PatternCircle( ability );
	}
}


void PatternPoint( Ability *ability )
{
	int i;
	int minang, maxang;
	double vx, vy;
	double ang;
	vec2_t startpos;
	vec2_t v;

	VectorAdd( ability->owner->position, ability->pos, startpos );

	maxang = 90 + ability->cone / 2;
	minang = maxang - ability->cone;
	
	for( i = 0; i < ability->numProj; i++ )
	{
		ang = ( double )( rand() % ability->cone );
		ang += minang;
		ang = ang * ( PI / 180 );

		vx = ( cos( ang ) ) * ability->velocity;
		vy = ( sin( ang ) ) * ability->velocity;

		v[ 0 ] = vx;
		v[ 1 ] = vy;

		InitProjectile( ability->owner, ability->owner->opponent, ability->owner->projectile, startpos, v, ability->fuse, 0 );
	}
}


void PatternLine( Ability *ability )
{
}


void PatternCircle( Ability *ability )
{
	vec2_t startpos;

	VectorClear( startpos );
	VectorAdd( startpos, ability->pos, startpos );
}
