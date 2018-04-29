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

    Map *map = mapInit("res/maps/forest.tmx");
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
    player->frameYoffset =  32;
    player->worldPosX    =  64;
    player->worldPosY    = 624;

    /* Note: The error handling isn't missing here.  There is simply no need to
     * quit the program if the music can't be played by some reason. */
    /*Mixer *mixer    = mixerInit();
    Music *music    = musicInit();
    music->filename = "res/music/creepy.ogg";
    if (mixer) musicFadeIn(music, 5000);*/

    uint16_t flags      = 0;
    int32_t  cameraPosX = 0;
    int32_t  cameraPosY = map->map->height * map->map->tile_height - video->height;

    while (1)
    {
        player->gid = mapGetGID(map->map->width, player->worldPosX, player->worldPosY);

        // Handle keyboard input.
        const uint8_t *keyState;
        SDL_PumpEvents();
        if (SDL_PeepEvents(0, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0)
            goto quit;

        // Reset flag (in case no key is pressed).
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
        if (0 != ((flags >> FREE_CAMERA) & 1))
        {
            if (keyState[SDL_SCANCODE_UP])    cameraPosY--;
            if (keyState[SDL_SCANCODE_DOWN])  cameraPosY++;
            if (keyState[SDL_SCANCODE_LEFT])  cameraPosX--;
            if (keyState[SDL_SCANCODE_RIGHT]) cameraPosX++;
        }
        else
        {
            cameraPosX = player->worldPosX - video->width  / 2;
            cameraPosY = player->worldPosY - video->height / 2 - 16;
        }
        // Set camera boundaries to map size.
        int32_t cameraMaxX = map->map->width  * map->map->tile_width  - video->width;
        int32_t cameraMaxY = map->map->height * map->map->tile_height - video->height;
        if (cameraPosX < 0) cameraPosX = 0;
        if (cameraPosY < 0) cameraPosY = 0;
        if (cameraPosX > cameraMaxX) cameraPosX = cameraMaxX;
        if (cameraPosY > cameraMaxY) cameraPosY = cameraMaxY;

        // Render scene.
        if (-1 == mapRender(video->renderer, map, "Background", 1, 0, map->worldPosX - cameraPosX, map->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "Level", 1, 1, map->worldPosX - cameraPosX, map->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == entityRender(video->renderer, player, player->worldPosX - cameraPosX, player->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "Overlay", 1, 2, map->worldPosX - cameraPosX, map->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        SDL_RenderPresent(video->renderer);
        SDL_RenderClear(video->renderer);
        SDL_Delay(5);
    }

    quit:
    /*musicFree(music);
    mixerFree(mixer);*/
    entityFree(player);
    mapFree(map);
    videoTerminate(video);
    return execStatus;
}
