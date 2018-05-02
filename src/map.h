/** @file main.h
 * @ingroup Map
 */

#ifndef MAP_h
#define MAP_h

#define MAX_TEXTURES_PER_MAP 5

// Tile types.
#define TILE_FLOOR 1
#define TILE_WALL  2
#define TILE_LWALL 3
#define TILE_RWALL 4

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
    double      worldPosX;
    double      worldPosY;
} Map;

uint8_t mapCoordIsType(Map *map, const char *type, double xPos, double yPos);
void    mapFree(Map *map);
uint8_t mapGetTileType(Map *map, double xPos, double yPos);
Map     *mapInit(const char *filename);
int8_t  mapRender(SDL_Renderer *renderer, Map *map, const char *name, uint8_t bg, uint8_t index, double posX, double posY);

#endif
