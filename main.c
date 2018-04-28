/** @file main.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "main.h"

int main()
{
    int execStatus = EXIT_SUCCESS;

    Video *video = videoInit("Rainbow Joe", 800, 600, 0);
    if (NULL == video)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    atexit(SDL_Quit);

    Map *map = mapInit("res/maps/forest.tmx", video->height);
    if (NULL == map)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    Entity *player = entityInit("player");
    if (NULL == player)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    if (-1 == entityLoadSprite(player, video->renderer, "res/sprites/characters.png"))
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    player->frameYoffset = 32;
    player->worldPosY    = 312;

    uint16_t flags      = 0;
    int32_t  cameraPosX = 0;
    int32_t  cameraPosY = 0;

    while (1)
    {
        // Handle keyboard input.
        const uint8_t *keyState;
        SDL_PumpEvents();
        if (SDL_PeepEvents(0, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0)
            goto quit;

        player->flags &= ~(1 << IN_MOTION);

        keyState = SDL_GetKeyboardState(NULL);

        if (keyState[SDL_SCANCODE_Q]) goto quit;
        if (keyState[SDL_SCANCODE_A])
        {
            player->flags |= 1 << IN_MOTION;
            player->flags |= 1 << DIRECTION;
            player->worldPosX--;
        }
        if (keyState[SDL_SCANCODE_D])
        {
            player->flags |= 1   << IN_MOTION;
            player->flags &= ~(1 << DIRECTION);
            player->worldPosX++;
        }

        if (keyState[SDL_SCANCODE_F]) flags ^= 1 << FREE_CAMERA;
        if (! ((flags >> FREE_CAMERA) & 1))
        {
            if (keyState[SDL_SCANCODE_UP])    cameraPosY--;
            if (keyState[SDL_SCANCODE_DOWN])  cameraPosY++;
            if (keyState[SDL_SCANCODE_LEFT])  cameraPosX--;
            if (keyState[SDL_SCANCODE_RIGHT]) cameraPosX++;
        }
        else
        {
            cameraPosX = player->worldPosX - video->width  / 2;
            cameraPosY = player->worldPosY - video->height / 2 - 32;
        }

        // Render scene.
        if (-1 == mapRender(video->renderer, map, "Background", 1, cameraPosX, cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == entityRender(video->renderer, player, player->worldPosX - cameraPosX, player->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "Level", 0, map->worldPosX - cameraPosX, map->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        SDL_RenderPresent(video->renderer);
        SDL_RenderClear(video->renderer);
    }

    quit:
    entityFree(player);
    mapFree(map);
    videoTerminate(video);
    return execStatus;
}
