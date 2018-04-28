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

typedef struct music_t {
    char      *filename;
    Mix_Music *mus;
} Music;

Mixer  *mixerInit();
Music  *musicInit();
int8_t musicPlay(Music *music);
int8_t musicFadeIn(Music *music, uint16_t ms);
void   mixerFree(Mixer *mixer);
void   musicFree(Music *music);

#endif
