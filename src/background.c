/** @file background.c
 * @ingroup   Background
 * @defgroup  Background
 * @brief     A background handler to render parallax scrolling backgrounds.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL2/SDL_image.h>
#include "background.h"

/**
 * @brief   Initialise background structure.  See @ref struct Background.
 * @param   filename   the image file to load.
 * @param   mapWidth   the width of the map.  See @ref struct Map.
 * @return  Background on success, NULL on error.
 * @ingroup Background
 */
Background *backgroundInit(SDL_Renderer *renderer, const char *filename, int32_t mapWidth)
{
    static Background *background;
    background = malloc(sizeof(struct background_t));
    if (NULL == background)
    {
        fprintf(stderr, "backgroundInit(): error allocating memory.\n");
        return NULL;
    }

    background->background = NULL;
    background->filename   = filename;
    background->height     = 0;
    background->width      = 0;
    background->worldPosX  = 0;
    background->worldPosY  = 0;

    background->image = IMG_LoadTexture(renderer, background->filename);
    if (NULL == background->image)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(background);
        return NULL;
    }

    if (0 != SDL_QueryTexture(background->image, NULL, NULL, &background->imageWidth, &background->imageHeight))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(background);
        return NULL;
    }

    background->wFactor = 0;
    if (background->wFactor > mapWidth)
    {
        background->wFactor = 1;
    }
    else
    {
        background->wFactor = ceil((double)mapWidth / (double)background->imageWidth);
    }

    background->height = background->imageHeight;
    background->width  = background->imageWidth * background->wFactor;

    return background;
}

/**
 * @brief   Render background.
 * @param   renderer   SDL's rendering context.  See @ref struct Video.
 * @param   background the background structure.  See @ref struct Background.
 * @param   cameraPosX camera position along the x-axis.
 * @param   cameraPosY camera position along the y-axis.
 * @return  0 on success, -1 on error.
 * @ingroup Background
 */
int8_t backgroundRender(
    SDL_Renderer *renderer,
    Background   *background,
    double       cameraPosX,
    double       cameraPosY)
{
    // Render background if already generated.
    if (background->background)
    {
        double renderPosX = background->worldPosX - cameraPosX;
        double renderPosY = background->worldPosY - cameraPosY;

        SDL_Rect dst =
        {
            renderPosX,
            renderPosY,
            background->width,
            background->height
        };
        if (-1 == SDL_RenderCopyEx(renderer, background->background, NULL, &dst, 0, NULL, SDL_FLIP_NONE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }

        dst.x = renderPosX + background->width;
        if (-1 == SDL_RenderCopyEx(renderer, background->background, NULL, &dst, 0, NULL, SDL_FLIP_NONE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }

        dst.x = renderPosX - background->width;
        if (-1 == SDL_RenderCopyEx(renderer, background->background, NULL, &dst, 0, NULL, SDL_FLIP_NONE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }

        return 0;
    }

    // Generate background.
    background->background = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        background->width,
        background->height);

    if (NULL == background->background)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (0 != SDL_SetRenderTarget(renderer, background->background))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    SDL_Rect dst;
    dst.x = 0;
    for (uint8_t i = 0; i < background->wFactor; i++)
    {
        dst.y  = 0;
        dst.w  = background->imageWidth;
        dst.h  = background->imageHeight;
        SDL_RenderCopy(renderer, background->image, NULL, &dst);
        dst.x += background->imageWidth;
    }

    // Switch back to default render target.
    if (0 != SDL_SetRenderTarget(renderer, NULL))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (0 != SDL_SetTextureBlendMode(background->background, SDL_BLENDMODE_BLEND))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    return 0;
}
