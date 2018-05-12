/** @file background.h
 * @ingroup Background
 */

#ifndef BACKGROUND_h
#define BACKGROUND_h

#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * @ingroup Background
 */
typedef struct background_t
{
    SDL_Texture *background;
    const char  *filename;
    uint32_t    height;
    uint32_t    width;
    double      worldPosX;
    double      worldPosY;
} Background;

void       backgroundFree(Background *background);
Background *backgroundInit(const char *filename);
int8_t     backgroundRender(SDL_Renderer *renderer, Background *background, int32_t windowWidth, double posX, double posY);

#endif
