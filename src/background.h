/** @file background.h
 * @ingroup Background
 */

#ifndef BACKGROUND_h
#define BACKGROUND_h

/**
 * @def     NUM_BACKGROUNDS
 *          The overall number of backgrounds used in the main program.
 * @ingroup Background
 */
#define NUM_BACKGROUNDS 4

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
    SDL_Texture *image;
    int32_t     imageWidth;
    int32_t     imageHeight;
    uint32_t    height;
    uint32_t    width;
    uint8_t     wFactor;
    double      worldPosX;
    double      worldPosY;
} Background;

void       backgroundFree(Background *background);
Background *backgroundInit(SDL_Renderer *renderer, const char *filename, int32_t mapWidth);
int8_t     backgroundRender(SDL_Renderer *renderer, Background *background, double cameraPosX, double cameraPosY);

#endif
