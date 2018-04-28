/** @file main.h
 * @ingroup Map
 */

#ifndef MAP_h
#define MAP_h

#define MAX_TEXTURES_PER_MAP 5

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

/**
 * @ingroup Map
 */
typedef struct map_t
{
    tmx_map     *map;
    SDL_Texture *texture[MAX_TEXTURES_PER_MAP];
    int32_t     worldPosX;
    int32_t     worldPosY;
} Map;

Map    *mapInit(const char *filename, int32_t windowHeight);
int8_t mapRender(SDL_Renderer *renderer, Map *map, const char *name, uint8_t bg, uint8_t index, int32_t posX, int32_t posY);
void   mapFree(Map *map);

#endif
