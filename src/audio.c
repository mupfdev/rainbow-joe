/** @file audio.c
 * @ingroup   Audio
 * @defgroup  Audio
 * @brief     Handler for the playback of music, sound effects, etc..
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "audio.h"

/**
 * @brief   Free audio mixer.
 * @param   mixer the mixer structure.  See @ref struct Mixer.
 * @ingroup Audio
 */
void mixerFree(Mixer *mixer)
{
    free(mixer);
    Mix_CloseAudio();
    while(Mix_Init(0)) Mix_Quit();
}

/**
 * @brief   Initialise audio mixer.
 * @return  Mixer on success, NULL on error.  See @ref struct Mixer.
 * @ingroup Audio
 */
Mixer *mixerInit()
{
    static Mixer *mixer;
    mixer = malloc(sizeof(struct mixer_t));
    if (NULL == mixer)
    {
        fprintf(stderr, "mixerInit(): error allocating memory.\n");
        return NULL;
    }

    if (-1 == SDL_Init(SDL_INIT_AUDIO))
    {
        fprintf(stderr, "Couldn't initialise SDL: %s\n", SDL_GetError());
        free(mixer);
        return NULL;
    }

    mixer->audioFormat       = MIX_DEFAULT_FORMAT;
    mixer->chunkSize         = 4096;
    mixer->numChannels       = 2;
    mixer->samplingFrequency = 44100;

    if (-1 == Mix_OpenAudio(
        mixer->samplingFrequency,
        mixer->audioFormat,
        mixer->numChannels,
        mixer->chunkSize))
    {
        fprintf(stderr, "%s\n", Mix_GetError());
        free(mixer);
        return NULL;
    }
    Mix_AllocateChannels(16);

    return mixer;
}

/**
 * @brief   Same as musicPlay but with fade-in effect.
 * @param   music the music structure that should be played.
 * @param   loops number of times to play through the music, -1 plays the music
 *                forever.
 * @param   ms    time to fade-in music.
 * @return  0 on success, -1 on error.
 * @ingroup Audio
 */
int8_t musicFadeIn(Music *music, int8_t loops, uint16_t ms)
{
    if (-1 == Mix_FadeInMusic(music->music, loops, ms))
    {
        fprintf(stderr, "%s\n", Mix_GetError());
        return -1;
    }

    return 0;
}

/**
 * @brief   Initialise Music.
 * @return  Music on success, NULL on error.  See @ref struct Music.
 * @ingroup Audio
 */
Music *musicInit(const char *filename)
{
    static Music *music;
    music = malloc(sizeof(struct music_t));
    if (NULL == music)
    {
        fprintf(stderr, "musicInit(): error allocating memory.\n");
        return NULL;
    }

    music->music = Mix_LoadMUS(filename);

    if (NULL == music->music)
    {
        fprintf(stderr, "%s\n", Mix_GetError());
        free(music);
        return NULL;
    }

    return music;
}

/**
 * @brief   Play music.
 * @param   music the music structure that should be played.
 * @param   loops number of times to play through the music, -1 plays the
 *                music forever.
 * @return  0 on success, -1 on error.
 * @ingroup Audio
 */
int8_t musicPlay(Music *music, int8_t loops)
{
    if (-1 == Mix_PlayMusic(music->music, loops))
    {
        fprintf(stderr, "%s\n", Mix_GetError());
        return -1;
    }

    return 0;
}

/**
 * @brief   Toggle music playback.
 * @ingroup Audio
 */
void musicToggle()
{
    if (Mix_PausedMusic())
    {
        Mix_ResumeMusic();
    }
    else
    {
        Mix_PauseMusic();
    }
}

/**
 * @brief   Initialise sound effect.
 * @param   sfx the sound sfx structure.  See @ref struct SFX.
 * @return  SFX on success, NULL on error.
 * @ingroup Audio
 */
SFX *sfxInit(const char *filename)
{
    static SFX *sfx;
    sfx = malloc(sizeof(struct sfx_t));
    if (NULL == sfx)
    {
        fprintf(stderr, "sfxInit(): error allocating memory.\n");
        return NULL;
    }

    sfx->sfx = Mix_LoadWAV(filename);

    if (NULL == sfx->sfx)
    {
        fprintf(stderr, "%s\n", Mix_GetError());
        free(sfx);
        return NULL;
    }

    return sfx;
}

/**
 * @brief   Play sound effect.
 * @param   sfx     the sfx structure.
 * @param   channel the mixer channel to use.
 * @param   loops   number of times to play the sound effect, -1 plays the
 *                  effect forever.
 * @return
 * @ingroup Audio
 */
int8_t sfxPlay(SFX *sfx, int8_t channel, int8_t loops)
{
    if (0 == Mix_Playing(channel))
    {
        if (-1 == Mix_PlayChannel(channel, sfx->sfx, loops))
        {
            fprintf(stderr, "%s\n", Mix_GetError());
            return -1;
        }
    }

    return 0;
}
