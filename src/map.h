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

void     mapFree(Map *map);
uint16_t mapGetGID(uint16_t width, int32_t posX, int32_t posY);
Map      *mapInit(const char *filename);
int8_t   mapRender(SDL_Renderer *renderer, Map *map, const char *name, uint8_t bg, uint8_t index, int32_t posX, int32_t posY);

#endif
