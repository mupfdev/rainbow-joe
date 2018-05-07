/** @file hud.c
 * @ingroup   HUD
 * @defgroup  HUD
 * @brief     Handler to manage the head-up display.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "hud.h"

/**
 * @brief   
 * @param   icon
 * @ingroup HUD
 */
void iconFree(Icon *icon)
{
    free(icon);
}

/**
 * @brief   
 * @param   filename
 * @return  
 * @ingroup HUD
 */
Icon *iconInit(SDL_Renderer *renderer, const char *filename)
{
    static Icon *icon;
    icon = malloc(sizeof(struct icon_t));
    if (NULL == icon)
    {
        fprintf(stderr, "iconInit(): error allocating memory.\n");
        return NULL;
    }

    icon->icon   = NULL;
    icon->height = 32;
    icon->width  = 32;

    icon->icon = IMG_LoadTexture(renderer, filename);
    if (NULL == icon->icon)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(icon);
        return NULL;
    }

    return icon;
}

/**
 * @brief   
 * @param   renderer
 * @param   icon
 * @param   posX
 * @param   posY
 * @return  
 * @ingroup HUD
 */
int8_t iconRender(SDL_Renderer *renderer, Icon *icon, double posX, double posY)
{
    if (NULL == icon->icon)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    SDL_Rect dst = { posX, posY, icon->width, icon->height };
    SDL_Rect src = { 0,    0,    icon->width, icon->height };

    if (-1 == SDL_RenderCopy(renderer, icon->icon, &src, &dst))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    return 0;
}
