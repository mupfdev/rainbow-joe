/** @file main.h
 * @ingroup Map
 */

#ifndef MAP_h
#define MAP_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

/**
 * @ingroup Map
 */
typedef struct map_t
{
    tmx_map     *map;
    SDL_Texture *texture;
    int32_t     worldPosX;
    int32_t     worldPosY;
} Map;

Map    *mapInit(const char *filename, int32_t windowHeight);
int8_t mapRender(SDL_Renderer *renderer, Map *map, const char *name, uint8_t bg, int32_t posX, int32_t posY);
void   mapFree(Map *map);

#endif
