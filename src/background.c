/** @file background.c
 * @ingroup   Background
 * @defgroup  Background
 * @brief     A background handler to render parallax scrolling backgrounds.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "background.h"

/**
 * @brief   
 * @param   background
 * @ingroup Background
 */
void backgroundFree(Background *background)
{
    free(background);
}

Background *backgroundInit(const char *filename)
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

    return background;
}

/**
 * @brief 
 * @param   renderer
 * @param   background
 * @param   mapWidth
 * @param   posX
 * @param   posY
 * @return  
 * @ingroup Background
 */
int8_t backgroundRender(
    SDL_Renderer *renderer,
    Background   *background,
    int32_t      mapWidth,
    double       posX,
    double       posY)
{
    // Render background if already generated.
    if (background->background)
    {
        SDL_Rect dst = { posX, posY, background->width, background->height };
        if (-1 == SDL_RenderCopyEx(renderer, background->background, NULL, &dst, 0, NULL, SDL_FLIP_NONE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }

        dst.x = posX + background->width;
        if (-1 == SDL_RenderCopyEx(renderer, background->background, NULL, &dst, 0, NULL, SDL_FLIP_NONE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }

        dst.x = posX - background->width;
        if (-1 == SDL_RenderCopyEx(renderer, background->background, NULL, &dst, 0, NULL, SDL_FLIP_NONE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }

        return 0;
    }

    // Generate background.
    SDL_Texture *image = IMG_LoadTexture(renderer, background->filename);
    if (NULL == image)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    int32_t imageHeight;
    int32_t imageWidth;
    if (0 != SDL_QueryTexture(image, NULL, NULL, &imageWidth, &imageHeight))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    uint8_t wFactor = 0;
    if (wFactor > mapWidth)
        wFactor = 1;
    else
        wFactor = ceil((double)mapWidth / (double)imageWidth);

    background->height = imageHeight;
    background->width  = imageWidth * wFactor;

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
    for (uint8_t i = 0; i < wFactor; i++)
    {
        dst.y  = 0;
        dst.w  = imageWidth;
        dst.h  = imageHeight;
        SDL_RenderCopy(renderer, image, NULL, &dst);
        dst.x += imageWidth;
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
