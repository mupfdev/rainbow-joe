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
 * @param   title the title of the window, in UTF-8 encoding.
 * @param   width the width of the window, in screen coordinates.
 * @param   height the height of the window, in screen coordinates.
 * @param   flags 0, or one or more SDL_WindowFlags OR'd together.
 * @return  A Video structure or NULL on failure.  See @ref struct Video.
 * @ingroup Video
 */
Video *videoInit(const char *title, uint32_t width, uint32_t height, uint32_t flags)
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

    video->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        flags);

    video->width  = width;
    video->height = height;

    if (NULL == video->window)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(video);
        return NULL;
    }

    video->renderer = SDL_CreateRenderer(
        video->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    if (NULL == video->renderer)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(video);
        return NULL;
    }

    return video;
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