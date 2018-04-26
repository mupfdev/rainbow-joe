/** @file main.h
 * @ingroup Map
 */

#ifndef MAP_h
#define MAP_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

tmx_map *mapLoad(const char *filename);
SDL_Texture *mapRender(SDL_Renderer *renderer, tmx_map *map, uint8_t bg, const char *name);
void mapFree(tmx_map *map);

#endif
