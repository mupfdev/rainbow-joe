/** @file video.h
 * @ingroup Video
 */

#ifndef VIDEO_h
#define VIDEO_h

#include <SDL2/SDL.h>

/**
 * @ingroup Video
 */
typedef struct video_t
{
    SDL_Renderer *renderer;
    SDL_Window   *window;
    int32_t      windowHeight;
    int32_t      windowWidth;
    int32_t      viewportHeight;
    int32_t      viewportWidth;
    double       zoomLevel;
} Video;

Video *videoInit(const char *title, int32_t width, int32_t height, uint32_t flags, double zoomLevel);
int8_t videoSetZoomLevel(Video *video, double zoomLevel);
void   videoTerminate(Video *video);

#endif
