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
    SDL_Renderer *renderer;
    SDL_Window   *window;
    int32_t      width;
    int32_t      height;
} Video;

Video *videoInit(const char *title, uint32_t width, uint32_t height, uint32_t flags);
void videoTerminate(Video *video);

#endif
