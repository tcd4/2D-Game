#include "audio.h"
#include <stdio.h>
#include <string.h>


static Sound * __soundList = NULL;


void InitSoundList()
{
	__soundList = ( Sound * )malloc( sizeof( Sound ) * MAX_SOUND_CLIPS );

	if( __soundList == NULL )
	{
		fprintf( stderr, "InitSoundList: ERROR: cannot allocate memory for __soundList\n" );
		exit( -1 );
	}

	memset( __soundList, 0, sizeof( Sound ) * MAX_SOUND_CLIPS );
}


void ClearSoundList()
{
	int i;

	for( i = 0; i < MAX_SOUND_CLIPS; i++ )
	{
		SilenceSound( &__soundList[ i ] );
	}
}


void SilenceSound( Sound *sound )
{
	if( !sound )
	{
		return;
	}

	memset( sound, 0, sizeof( Sound ) );
	sound = NULL;
}


void InitAudio()
{
	InitSoundList();
}
