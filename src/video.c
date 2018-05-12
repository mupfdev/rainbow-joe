/** @file video.c
 * @ingroup   Video
 * @defgroup  Video
 * @brief     Used to initialise and terminate SDL's video subsystem.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "video.h"

/**
 * @brief   Initialise SDL's video subsystem.
 * @param   title      the title of the window, in UTF-8 encoding.
 * @param   width      the width of the window, in screen coordinates.
 * @param   height     the height of the window, in screen coordinates.
 * @param   fullscreen the window's fullscreen state.
 * @param   zoomLevel  the zoom level used by the renderer.
 * @return  A Video structure or NULL on failure.  See @ref struct Video.
 * @ingroup Video
 */
Video *videoInit(const char *title, int32_t width, int32_t height, uint8_t fullscreen, double zoomLevel)
{
    static Video *video;
    video = malloc(sizeof(struct video_t));

    if (NULL == video)
    {
        fprintf(stderr, "videoInit(): error allocating memory.\n");
        return NULL;
    }

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(video);
        return NULL;
    }

    video->windowHeight = height;
    video->windowWidth  = width;
    video->zoomLevel       = zoomLevel;
    video->zoomLevelInital = zoomLevel;

    uint32_t flags;
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    else
        flags = 0;

    video->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        video->windowWidth,
        video->windowHeight,
        flags);

    if (NULL == video->window)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(video);
        return NULL;
    }

    if (fullscreen)
    {
        SDL_GetWindowSize(video->window, &video->windowWidth, &video->windowHeight);
        if (0 > SDL_ShowCursor(SDL_DISABLE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            free(video);
            return NULL;
        }
    }

    video->renderer = SDL_CreateRenderer(
        video->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    if (NULL == video->renderer)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(video);
        return NULL;
    }

    if (0 != SDL_RenderSetLogicalSize(video->renderer, video->windowWidth / zoomLevel, video->windowHeight / zoomLevel))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(video);
        return NULL;
    }

    return video;
}

/**
 * @brief   Set the renderer's zoom level.
 * @param   video     A Video structure.  See @ref struct Video.
 * @param   zoomLevel the zoom level
 * @ingroup Video
 * @return  0 on success, -1 on failure.
 */
int8_t videoSetZoomLevel(Video *video, double zoomLevel)
{
    if (zoomLevel < 1) zoomLevel = 1;

    if (0 != SDL_RenderSetLogicalSize(video->renderer, video->windowWidth / zoomLevel, video->windowHeight / zoomLevel))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    video->zoomLevel = zoomLevel;

    return 0;
}

/**
 * @brief   Terminate SDL's video subsystem.
 * @param   video a Video structure.  See @ref struct Video.
 * @ingroup Video
 */
void videoTerminate(Video *video)
{
    if ((NULL == video->window))
        fprintf(stderr, "%s\n", SDL_GetError());

    SDL_DestroyRenderer(video->renderer);
    SDL_DestroyWindow(video->window);
    free(video);
}
