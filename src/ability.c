#include <string.h>
#include <stdio.h>
#include "ability.h"
#include "projectile.h"


extern SDL_Surface *screen;
extern Uint32 NOW;
extern Uint32 cooldown;


int LoadAbility( Ability *ability, char *filename, Entity *owner )
{
	FILE *file = NULL;
	char buf[ 128 ];
	char path[ FILE_PATH_LEN ];
	int w, h;
	int subnum = 0;
	int fpl;
	Ability *temp = NULL;

	if( ability )
	{
		FreeAbility( ability );
	}

	ability->owner = owner;

	file = fopen( filename, "r" );
	if( !file )
	{
		fprintf( stderr, "ERROR: LoadAbility: could not open file: %s\n", filename );
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
			fscanf( file, "%s", ability->name );
		}
		else if( strncmp( buf, "anim:", 128 ) == 0 )
		{
			fscanf( file, "%s", ability->anim );
		}
		else if( strncmp( buf, "size:", 128 ) == 0 )
		{
			fscanf( file, "%i,%i", &w, &h );
		}
		else if( strncmp( buf, "framesperline:", 128 ) == 0 )
		{
			fscanf( file, "%i", &fpl );
		}
		else if( strncmp( buf, "projectile:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );
			ability->proj = LoadSprite( path, w, h, fpl );
		}
		else if( strncmp( buf, "pattern:", 128 ) == 0 )
		{
			fscanf( file, "%s", ability->pattern );
		}
		else if( strncmp( buf, "relative:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->relative );
		}
		else if( strncmp( buf, "velocity:", 128 ) == 0 )
		{
			fscanf( file, "%f", &ability->velocity );
		} 
		else if( strncmp( buf, "duration:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->duration );
		}
		else if( strncmp( buf, "position:", 128 ) == 0 )
		{
			fscanf( file, "%f,%f", &ability->position[ 0 ], &ability->position[ 1 ] );
		}
		else if( strncmp( buf, "numProj:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->numProj );
		}
		else if( strncmp( buf, "fuse:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->fuse );
		}
		else if( strncmp( buf, "rate:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->fireRate );
		}
		else if( strncmp( buf, "cooldown:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->cooldown );
		}
		else if( strncmp( buf, "start_time:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->startTime );
			ability->startTime += NOW;
		}
		else if( strncmp( buf, "ability:", 128 ) == 0 )
		{
			fscanf( file, "%s", path );

			temp = ( Ability * )malloc( sizeof( Ability ) * (subnum + 1 ) );
			memcpy( temp, ability->concurrent_ability, sizeof( Ability ) * subnum );
			ability->concurrent_ability = temp;
			LoadAbility( &ability->concurrent_ability[ subnum ], path, owner );
			subnum++;
		}
	}

	fclose( file );

	if( !ability->relative )
	{
		ability->position[ 0 ] = screen->w * ( ability->position[ 0 ] / 100.00 );
		ability->position[ 1 ] = screen->h * ( ability->position[ 1 ] / 100.00 );
	}

	ability->velocities = ( vec2_t * )malloc( sizeof( vec2_t ) * ability->numProj );
	if( !ability->velocities )
	{
		fprintf( stderr, "ERROR: LoadPoint: can't allocate memory for velocities\n" );
		return 0;
	}

	if( strncmp( ability->pattern, "point", 128 ) == 0 )
	{
		LoadPoint( ability, filename );
	}
	else if( strncmp( ability->pattern, "circle", 128 ) == 0 )
	{
		LoadCircle( ability, filename );
	}
	else if( strncmp( ability->pattern, "custom", 128 ) == 0 )
	{
		LoadCustom( ability, filename );
	}

	ability->concurrent_num = subnum;
	/*
	FILE *abilityfile = NULL;
	char buf[ 128 ];
	char pat[ 128 ];
	vec2_t postemp;
	vec2_t *plisttemp;
	
	ability->pos = NULL;

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
			fscanf( abilityfile, "%f,%f", &postemp[ 0 ], &postemp[ 1 ] );

			ability->numpos++;

			plisttemp = ( vec2_t * )malloc( sizeof( vec2_t ) * ability->numpos );
			memcpy( plisttemp, ability->pos, sizeof( vec2_t ) * ( ability->numpos - 1 ) );
			ability->pos = plisttemp;

			Vec2Copy( postemp, ability->pos[ ability->numpos - 1 ] );
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
		else if( strncmp( buf, "mtype:", 128 ) == 0 )
		{
			fscanf( abilityfile, "%i", &ability->movement );
		}
	}

	fclose( abilityfile );

	offset[ 0 ] = owner->w / 2;
	offset[ 1 ] = owner->h / 2;

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

	if( ability->movement == NULL )
	{
		ability->movement = MOVE_NO;
	}

	if( ability->path )
	{
		ability->movement = MOVE_PATH;
	}
	else
	{
		ability->path = NULL;
	}

	ability->endTime = 0;*/

	ability->loaded = 1;
	ability->inuse = 0;

	fprintf( stdout, "LoadAbility: %s ability %s loaded\n", ability->pattern, ability->name );
	return 1;
}


int LoadPoint( Ability *ability, char *filename )
{
	FILE *file = NULL;
	char buf[ 128 ];
	int i;
	float vx, vy;
	float angle;
	float curangle;
	float decriment;

	file = fopen( filename, "r" );

	while( fscanf( file, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), file );
		}
		else if( strncmp( buf, "angle:", 128 ) == 0 )
		{
			fscanf( file, "%f", &ability->angle );
		}
		else if( strncmp( buf, "cone:", 128 ) == 0 )
		{
			fscanf( file, "%f", &ability->cone );
		} 
	}

	fclose( file );

	if( !ability->relative )
	{
		angle = ability->angle + 90;
		angle += ( ability->cone / 2 );
		angle *= TORAD;

		decriment = ability->cone / ( ability->numProj - 1 );
		decriment *= TORAD;

		for( i = 0; i < ability->numProj; i++ )
		{
			curangle = angle - ( decriment * i );
			
			vx = ( cos( curangle ) ) * ability->velocity;
			vy = ( sin( curangle ) ) * ability->velocity;

			ability->velocities[ i ][ 0 ] = vx;
			ability->velocities[ i ][ 1 ] = vy;
		}
	}

	return 1;
}


int LoadCircle( Ability *ability, char *filename )
{
	return 1;
}


int LoadCustom( Ability *ability, char *filename )
{
	return 1;
}


void FreeAbility( Ability *ability )
{
	ability->inuse = 0;
	ability->loaded = 0;

	if( ability->proj )
	{
		FreeSprite( ability->proj );
	}

	if( ability->concurrent_ability )
	{
		FreeAbility( ability->concurrent_ability );
	}
	
	/*
	if( ability->delimiter )
	{
		free( ability->delimiter );
	}*/
}


void StartAbility( Ability *ability )
{
	int i;

	if( !ability->loaded )
	{
		return;
	}

	if( ability->inuse )
	{
		return;
	}

	ability->inuse = 1;

	if( ability->duration )
	{
		ability->endTime = ability->duration + NOW;
	}

	if( ability->anim )
	{
		//change animations
	}

	cooldown = NOW + ability->cooldown + ability->duration;

	for( i = 0; i < ability->concurrent_num; i++ )
	{
		ability->concurrent_ability[ i ].inuse = 1;
	}

	UseAbility( ability );
}


void UseAbility( Ability *ability )
{
	vec2_t firepos;
	int i;

	for( i = 0; i < ability->concurrent_num; i++ )
	{
		UseAbility( &ability->concurrent_ability[ i ] );
	}

	if( !ability->inuse || ability->nextFire > NOW )
	{
		return;
	}

	if( ability->relative )
	{
		Vec2Add( ability->position, ability->owner->origin, firepos );
	}
	else
	{
		Vec2Copy( ability->position, firepos );
	}
	
	if( strncmp( ability->pattern, "point", 128 ) == 0 )
	{
		FirePointAbility( ability, firepos );
	}
	else if( strncmp( ability->pattern, "circle", 128 ) == 0 )
	{
		FireCircleAbility( ability, firepos );
	}
	else if( strncmp( ability->pattern, "custom", 128 ) == 0 )
	{
		FireCustomAbility( ability, firepos );
	}

	if( ability->endTime && ability->endTime <= NOW )
	{
		EndAbility( ability );
	}
	else
	{
		ability->nextFire = NOW + ability->fireRate;
	}
}


void EndAbility( Ability *ability )
{
	int i;
	ability->inuse = 0;
	//change animation
	ability->startTime = 0;
	ability->endTime = 0;
	ability->nextFire = 0;

	for( i = 0; i < ability->concurrent_num; i++ )
	{
		ability->concurrent_ability[ i ].inuse = 0;
	}
}


void FirePointAbility( Ability *ability, vec2_t firepos )
{
	int i;
	float vx, vy;
	float angle;
	float curangle;
	float decriment;

	if( ability->relative )
	{
		angle = ability->angle + 90;
		angle += ( ability->cone / 2 );
		angle *= TORAD;

		decriment = ability->cone / ( ability->numProj - 1 );
		decriment *= TORAD;

		for( i = 0; i < ability->numProj; i++ )
		{
			curangle = angle - ( decriment * i );

			vx = ( cos( curangle ) ) * ability->velocity;
			vy = ( sin( curangle ) ) * ability->velocity;

			ability->velocities[ i ][ 0 ] = vx;
			ability->velocities[ i ][ 1 ] = vy;
		}
	}

	for( i = 0; i < ability->numProj; i++ )
	{	
		InitProjectile( ability->owner, ability->owner->group, ability->proj, firepos,
			ability->velocities[ i ], ability->fuse, 0 );
	}
}


void FireCircleAbility( Ability *ability, vec2_t firepos )
{
}


void FireCustomAbility( Ability *ability, vec2_t firepos )
{
}
