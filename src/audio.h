/**
 * @file audio.h
 *
 * @brief file containing a basic structure and functions
 * for controlling audio
 */

#ifndef __AUDIO__
#define __AUDIO__


#include <SDL_mixer.h>


#define SOUND_NAME_LEN 80
#define MAX_SOUND_CLIPS 128


enum s_groups { FX_NULL, FX_Player, FX_Boss, FX_Fire };


typedef struct sound_s
{
	Mix_Chunk	*sound;						/**< a pointer to the sound */
	char		filename[ SOUND_NAME_LEN ]; /**< file path of the sound file */
	int			inuse;						/**< determines if the sound is being used */
	int			volume;						/**< determines the volume the sound is played at */
}Sound; 

/**
 * @brief initialize audio system
 */
void InitAudio();

/**
 * @brief initialize the sound list
 */
void InitSoundList();

/**
 * @brief destroy all sound
 */
void ClearSoundList();

/**
 * @brief silence a sound
 *
 * @param sound a pointer to the soon to be silenced sound
 */
void SilenceSound( Sound *sound );

/**
 * @brief play a sound in a specific channel
 *
 * @param sound a pointer to the sound to be played
 * @param channel the channel to play the sound
 */
void PlaySound( Sound *sound, int channel );

/**
 * @brief load a sound into memory
 *
 * @param filename the path and name of the file
 */
Sound *LoadSound( char *filename );


#endif
