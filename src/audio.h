/** @file audio.h
 * @ingroup Audio
 */

#ifndef AUDIO_h
#define AUDIO_h

#define NUM_SFX   5
#define SFX_DEAD  0
#define SFX_JUMP  1
#define CH_DEAD   1
#define CH_JUMP   2

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
    Mix_Music *music;
} Music;

/**
 * @ingroup Audio
 */
typedef struct sfx_t {
    Mix_Chunk *sfx;
} SFX;

void   mixerFree(Mixer *mixer);
Mixer  *mixerInit();
int8_t musicFadeIn(Music *music, int8_t loops, uint16_t ms);
void   musicFree(Music *music);
Music *musicInit(const char *filename);
int8_t musicPlay(Music *music, int8_t loops);
void   musicHalt();
void   sfxFree(SFX *sfx);
SFX   *sfxInit(const char *filename);
int8_t sfxPlay(SFX *sfx, int8_t channel, int8_t loops);

#endif
