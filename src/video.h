/** @file video.h
 * @ingroup Video
 */

#ifndef VIDEO_h
#define VIDEO_h

#include <SDL2/SDL.h>

/**
 * @ingroup Video
 */
typedef struct video_t {
    int32_t      height;
    SDL_Renderer *renderer;
    int32_t      width;
    SDL_Window   *window;
} Video;

Video *videoInit(const char *title, uint32_t width, uint32_t height, uint32_t flags);
void  videoTerminate(Video *video);

#endif
