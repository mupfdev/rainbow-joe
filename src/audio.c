/** @file audio.c
 * @ingroup   Audio
 * @defgroup  Audio
 * @brief     Handler for the playback of music, sound effects, etc.t
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

 #include "audio.h"

/**
 * @brief   Free audio mixer.
 * @param   mixer
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
 * @return  Mixer on success, NULL on error.
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

    if (-1 ==
        Mix_OpenAudio(
            mixer->samplingFrequency,
            mixer->audioFormat,
            mixer->numChannels,
            mixer->chunkSize))
    {
        fprintf(stderr, "%s\n", Mix_GetError());
        free(mixer);
        return NULL;
    }

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
    music->mus = Mix_LoadMUS(music->filename);

    if (NULL == music->mus) {
        fprintf(stderr, "%s\n", Mix_GetError());
        return -1;
    }

    if (-1 == Mix_FadeInMusic(music->mus, loops, ms))
    {
        fprintf(stderr, "%s\n", Mix_GetError());
        return -1;
    }

    return 0;
}

/**
 * @brief   Free music.
 * @param   music
 * @ingroup Audio
 */
void musicFree(Music *music)
{
    free(music);
}

/**
 * @brief   Initialise Music.
 * @return  Music on success, NULL on error.
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

    music->filename = filename;

    return music;
}

/**
 * @brief   Play music.
 * @param   music the music structure that should be played.
 * @param   loops number of times to play through the music, -1 plays the music
 *                forever.
 * @return  0 on success, -1 on error.
 * @ingroup Audio
 */
int8_t musicPlay(Music *music, int8_t loops)
{
    music->mus = Mix_LoadMUS(music->filename);

    if (NULL == music->mus) {
        fprintf(stderr, "%s\n", Mix_GetError());
        return -1;
    }

    if (-1 == Mix_PlayMusic(music->mus, loops)) {
        fprintf(stderr, "%s\n", Mix_GetError());
        return -1;
    }

    return 0;
}

/**
 * @brief Halt music.
 */
void musicHalt()
{
    Mix_HaltMusic();
}
