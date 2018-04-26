/** @file main.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "main.h"

int main()
{
    Video *video = videoInit("Rainbow Joe", 800, 600, 0);
    if (NULL == video) return EXIT_FAILURE;

    atexit(SDL_Quit);

    tmx_map *map = mapLoad("res/maps/level-01.tmx");
    if (NULL == map) return EXIT_FAILURE;

    SDL_Texture *mapLevel = mapRender(video->renderer, map, 0, "Level");
    if (NULL == map) return EXIT_FAILURE;

    SDL_Texture *mapBG = mapRender(video->renderer, map, 1, "Background");
    if (NULL == map) return EXIT_FAILURE;

    int32_t  cameraPosX = 0;
    int32_t  cameraPosY = 0 - (map->height * map->tile_height - video->height);
    SDL_Rect camera;
    camera.w = map->width  * map->tile_width;
    camera.h = map->height * map->tile_height;
    camera.x = cameraPosX;
    camera.y = cameraPosY;

    if (-1 == SDL_RenderCopy(video->renderer, mapBG, NULL, &camera))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (-1 == SDL_RenderCopy(video->renderer, mapLevel, NULL, &camera))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    SDL_RenderPresent(video->renderer);
    SDL_RenderClear(video->renderer);

    while (1)
    {
        const uint8_t *keyState;
        SDL_PumpEvents();
        if (SDL_PeepEvents(0, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0)
            goto quit;

        keyState = SDL_GetKeyboardState(NULL);

        if (keyState[SDL_SCANCODE_Q]) goto quit;
    }

    quit:
    SDL_DestroyTexture(mapBG);
    SDL_DestroyTexture(mapLevel);
    mapFree(map);
    videoTerminate(video);
    return EXIT_SUCCESS;
}
