/** @file audio.h
 * @ingroup Audio
 */

#ifndef AUDIO_h
#define AUDIO_h

/**
 * @def     musicPause()
 *          Pause music playback.
 * @ingroup Audio
 */
#define musicPause()  Mix_PauseMusic()
/**
 * @def     musicResume()
 *          Resume music playback.
 * @ingroup Audio
 */
#define musicResume() Mix_ResumeMusic()

#define NUM_SFX      5
#define SFX_DEAD     0
#define SFX_IMPACT   1
#define SFX_JUMP     2
#define SFX_PAUSE    3
#define SFX_UNPAUSE  4
#define CH_DEAD      1
#define CH_IMPACT    2
#define CH_JUMP      3
#define CH_PAUSE     4
#define CH_UNPAUSE   5

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
void   musicHalt();
Music *musicInit(const char *filename);
int8_t musicPlay(Music *music, int8_t loops);
void   musicToggle();
void   sfxFree(SFX *sfx);
SFX   *sfxInit(const char *filename);
int8_t sfxPlay(SFX *sfx, int8_t channel, int8_t loops);

#endif
