#include <string.h>
#include <stdio.h>
#include "ability.h"
#include "projectile.h"


extern SDL_Surface *screen;
extern Uint32 NOW;
extern Uint32 cooldown;
extern Uint32 lock;


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
		else if( strncmp( buf, "angle:", 128 ) == 0 )
		{
			fscanf( file, "%f", &ability->angle );
		}
		else if( strncmp( buf, "cone:", 128 ) == 0 )
		{
			fscanf( file, "%f", &ability->cone );
		} 
		else if( strncmp( buf, "radius:", 128 ) == 0 )
		{
			fscanf( file, "%f", &ability->radius );
		}
		else if( strncmp( buf, "lock:", 128 ) == 0 )
		{
			fscanf( file, "%i", &ability->lock );
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
		LoadPoint( ability );
	}
	else if( strncmp( ability->pattern, "circle", 128 ) == 0 )
	{
		LoadCircle( ability );
	}
	else if( strncmp( ability->pattern, "custom", 128 ) == 0 )
	{
		LoadCustom( ability, filename );
	}

	ability->concurrent_num = subnum;
	ability->loaded = 1;
	ability->inuse = 0;

	fprintf( stdout, "LoadAbility: %s ability %s loaded\n", ability->pattern, ability->name );
	return 1;
}


int LoadPoint( Ability *ability )
{
	int i;
	float vx, vy;
	float angle;
	float curangle;
	float decriment;

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


int LoadCircle( Ability *ability )
{
	int i;
	float angle;
	float sine, cosine;
	float increment;

	ability->positions = ( vec2_t * )malloc( sizeof( vec2_t ) * ability->numProj );
	if( !ability->positions )
	{
		fprintf( stderr, "ERROR: LoadPoint: can't allocate memory for positions\n" );
		return 0;
	}
	
	ability->base = ( vec2_t * )malloc( sizeof( vec2_t ) * ability->numProj );
	if( !ability->positions )
	{
		fprintf( stderr, "ERROR: LoadPoint: can't allocate memory for base positions\n" );
		return 0;
	}

	increment = TAU / ability->numProj;

	for( i = 0; i < ability->numProj; i++ )
	{
		angle = increment * i;

		sine = sin( angle );
		cosine = cos( angle );
		ability->base[ i ][ 0 ] = cosine * ability->radius;
		ability->base[ i ][ 1 ] = sine * ability->radius;

		ability->velocities[ i ][ 0 ] = cosine * ability->velocity;
		ability->velocities[ i ][ 1 ] = sine * ability->velocity;
	}

	if( !ability->relative )
	{
		for( i = 0; i < ability->numProj; i++ )
		{
			Vec2Add( ability->position, ability->base[ i ], ability->positions[ i ] );
		}
	}

	return 1;
}


int LoadCustom( Ability *ability, char *filename )
{
	int i = 0;
	FILE *file = NULL;
	char buf[ 128 ];
	float angle;
	float x, y;

	ability->positions = ( vec2_t * )malloc( sizeof( vec2_t ) * ability->numProj );
	if( !ability->positions )
	{
		fprintf( stderr, "ERROR: LoadPoint: can't allocate memory for positions\n" );
		return 0;
	}

	ability->base = ( vec2_t * )malloc( sizeof( vec2_t ) * ability->numProj );
	if( !ability->positions )
	{
		fprintf( stderr, "ERROR: LoadPoint: can't allocate memory for base positions\n" );
		return 0;
	}

	ability->angles = ( float * )malloc( sizeof( float ) * ability->numProj );
	if( !ability->angles )
	{
		fprintf( stderr, "ERROR: LoadCustom: can't allocate memory for angles\n" );
		return 0;
	}

	file = fopen( filename, "r" );

	while( fscanf( file, "%s", buf ) != EOF )
	{
		if( buf[ 0 ] == '#' )
		{
			fgets( buf, sizeof( buf ), file );
		}
		else if( strncmp( buf, "p:", 128 ) == 0 )
		{
			fscanf( file, "%f,%f", &x, &y );
			ability->base[ i ][ 0 ] = x;
			ability->base[ i ][ 1 ] = y;
		}
		else if( strncmp( buf, "a:", 128 ) == 0 )
		{
			fscanf( file, "%f", &angle );
			ability->angles[ i ] = angle;
			i++;
		}
	}

	fclose( file );

	for( i = 0; i < ability->numProj; i++ )
	{
		angle = ( ability->angles[ i ] + 90 ) * TORAD;
		ability->velocities[ i ][ 0 ] = cos( angle ) * ability->velocity;
		ability->velocities[ i ][ 1 ] = sin( angle ) * ability->velocity;

		if( !ability->relative )
		{
			ability->base[ i ][ 0 ] = screen->w * ( ability->base[ i ][ 0 ] / 100.00 );
			ability->base[ i ][ 1 ] = screen->h * ( ability->base[ i ][ 1 ] / 100.00 );
		}
	}

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

	if( strncmp( ability->anim, "", 128 ) != 0 )
	{
		ChangeState( ability->owner, ability->anim );
	}

	for( i = 0; i < ability->concurrent_num; i++ )
	{
		ability->concurrent_ability[ i ].inuse = 1;
	}

	if( ability->lock )
	{
		lock = 1;
	}

	ability->currentFire = 0;

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

	if( ability->currentFire >= ability->numProj )
	{
		EndAbility( ability );
	}
}


void EndAbility( Ability *ability )
{
	int i;

	ability->inuse = 0;
	ability->startTime = 0;
	ability->endTime = 0;
	ability->nextFire = 0;

	if( ability->lock )
	{
		lock = 0;
	}

	cooldown = NOW + ability->cooldown;

	if( strncmp( ability->anim, "", 128 ) != 0 )
	{
		ChangeState( ability->owner, "idle" );
	}

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
	int i;

	for( i = 0; i < ability->numProj; i++ )
	{	
		if( ability->relative )
		{
			Vec2Add( firepos, ability->base[ i ], ability->positions[ i ] );
		}

		InitProjectile( ability->owner, ability->owner->group, ability->proj, ability->positions[ i ],
			ability->velocities[ i ], ability->fuse, 0 );
	}
}


void FireCustomAbility( Ability *ability, vec2_t firepos )
{
	int i;

	if( ability->fireRate )
	{
		if( ability->relative )
		{
			Vec2Add( firepos, ability->base[ ability->currentFire ], ability->positions[ ability->currentFire ] );
		
			InitProjectile( ability->owner, ability->owner->group, ability->proj, 
				ability->positions[ ability->currentFire ], ability->velocities[ ability->currentFire ], ability->fuse, 0 );

		}
		else
		{
			InitProjectile( ability->owner, ability->owner->group, ability->proj, 
				ability->base[ ability->currentFire ], ability->velocities[ ability->currentFire ], ability->fuse, 0 );
		}
		ability->currentFire++;
	}
	else
	{
		for( i = 0; i < ability->numProj; i++ )
		{
			if( ability->relative )
			{
				Vec2Add( firepos, ability->base[ i ], ability->positions[ i ] );
		
				InitProjectile( ability->owner, ability->owner->group, ability->proj, 
					ability->positions[ i ], ability->velocities[ i ], ability->fuse, 0 );

			}
			else
			{
				InitProjectile( ability->owner, ability->owner->group, ability->proj, 
					ability->base[ i ], ability->velocities[ i ], ability->fuse, 0 );
			}
		}

		ability->currentFire = ability->numProj + 1;
	}
}
