/** @file audio.h
 * @ingroup Audio
 */

#ifndef AUDIO_h
#define AUDIO_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/**
 * @ingroup Audio
 */
typedef struct mixer_t {
    uint16_t audioFormat;
    uint16_t chunkSize;
    uint8_t  numChannels;
    uint16_t samplingFrequency;
} Mixer;

/**
 * @ingroup Audio
 */
typedef struct music_t {
    const char *filename;
    Mix_Music  *mus;
} Music;

void   mixerFree(Mixer *mixer);
Mixer  *mixerInit();
int8_t musicFadeIn(Music *music, int8_t loops, uint16_t ms);
void   musicFree(Music *music);
Music *musicInit(const char *filename);
int8_t musicPlay(Music *music, int8_t loops);
void   musicHalt();

#endif
