/** @file main.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 * @todo      Add function to display parallax backgrounds.
 */

#include "main.h"

int32_t main(int32_t argc, char *argv[])
{
    int execStatus  = EXIT_SUCCESS;

    const char *configFilename;
    if (argc > 1) configFilename = argv[1];
    else configFilename = "default.ini";

    Config     config      = configInit(configFilename);
    Video      *video      = NULL;
    Background *sky        = NULL;
    Background *clouds     = NULL;
    Background *sea        = NULL;
    Background *farGrounds = NULL;
    Map        *map        = NULL;
    Icon       *fcMode     = NULL;
    Entity     *player     = NULL;
    Entity     *npc        = NULL;
    Mixer      *mixer      = NULL;
    Music      *music      = NULL;
    Music      *dead       = NULL;

    video = videoInit(
        "Rainbow Joe",
        config.video.width,
        config.video.height,
        config.video.fullscreen,
        2);

    if (NULL == video)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    atexit(SDL_Quit);

    sky = backgroundInit("res/backgrounds/sky.png");
    if (NULL == sky)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    clouds = backgroundInit("res/backgrounds/clouds.png");
    if (NULL == clouds)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    sea = backgroundInit("res/backgrounds/sea.png");
    if (NULL == sea)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    farGrounds = backgroundInit("res/backgrounds/far-grounds.png");
    if (NULL == farGrounds)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    map = mapInit("res/maps/01.tmx");
    if (NULL == map)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    fcMode = iconInit(video->renderer, "res/icons/telescope.png");
    if (NULL == fcMode)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    player = entityInit();
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
    player->frameYoffset =  64;
    player->worldPosX    =  32;
    player->worldPosY    = 608;
    player->worldWidth   = map->width;
    player->worldHeight  = map->height;

    npc = entityInit();
    if (NULL == npc)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    if (-1 == entityLoadSprite(npc, video->renderer, "res/sprites/characters.png"))
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    npc->frameYoffset  =  32;
    npc->worldPosX     = 112;
    npc->worldPosY     = 400;
    npc->worldWidth    = map->width;
    npc->worldHeight   = map->height;

    /* Note: The error handling isn't missing here.  There is simply no need to
     * quit the program if the music can't be played by some reason. */
    mixer = mixerInit();
    music = musicInit("res/music/enchanted-tiki-86.ogg");
    dead  = musicInit("res/sfx/05.ogg");
    if (config.audio.enabled)
        if (mixer) musicFadeIn(music, -1, 5000);

    double   cameraPosX = 0;
    double   cameraPosY = map->height - video->windowHeight;
    double   timeA      = SDL_GetTicks();
    double   delay      = 0;
    while (1)
    {
        double timeB  = SDL_GetTicks();
        double dTime  = (timeB - timeA) / 1000;
        timeA         = timeB;

        entityFrame(player, dTime);
        entityFrame(npc,    dTime);

        if ((player->flags >> IS_DEAD) & 1)
        {
            if (config.audio.enabled && (0 == delay))
                if (mixer) musicPlay(dead, 1);
            delay += dTime;

            if (delay > 2)
            {
                player->flags     &= ~(1 << IS_DEAD);
                player->worldPosX  =  32;
                player->worldPosY  = 608;
                npc->flags        &= ~(1 << DIRECTION);
                npc->flags        &= ~(1 << IN_MOTION);
                npc->worldPosX     = 112;
                npc->worldPosY     = 400;

                delay              =   0;
                if (config.audio.enabled)
                    if (mixer) musicFadeIn(music, -1, 5000);
            }
        }

        // Handle keyboard input.
        const uint8_t *keyState;
        SDL_PumpEvents();
        if (SDL_PeepEvents(0, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0)
            goto quit;
        keyState = SDL_GetKeyboardState(NULL);
        // Reset IN_MOTION flag (in case no key is pressed).
        player->flags &= ~(1 << IN_MOTION);

        if (keyState[SDL_SCANCODE_LSHIFT])
        {
            // Allow running only when not in mid-air.
            if (0 == ((player->flags >> IN_MID_AIR) & 1))
            {
                player->velocityMax  = 250;
                player->frameStart   = RUN;
                player->frameEnd     = RUN_MAX;
            }
        }
        else
        {
            // Don't allow to slow down in mid-air.
            if (0 == ((player->flags >> IN_MID_AIR) & 1))
            {
                player->velocityMax  = 100;
                player->frameStart   = WALK;
                player->frameEnd     = WALK_MAX;
            }
        }
        if (keyState[SDL_SCANCODE_Q]) goto quit;
        if (keyState[SDL_SCANCODE_A])
        {
            if (0 == ((player->flags >> DIRECTION) & 1))
                player->velocity = -player->velocity;
            player->flags |= 1 << IN_MOTION;
            player->flags |= 1 << DIRECTION;
        }
        if (keyState[SDL_SCANCODE_D])
        {
            if ((player->flags >> DIRECTION) & 1)
                player->velocity = -player->velocity;

            player->flags |= 1   << IN_MOTION;
            player->flags &= ~(1 << DIRECTION);
        }

        if (0 == ((player->flags >> IN_MID_AIR) & 1))
            if (keyState[SDL_SCANCODE_SPACE])
            {
                player->flags        |= 1 << IS_JUMPING;
                player->velocityJump  = player->velocity;
            }

        if (keyState[SDL_SCANCODE_1])
            videoSetZoomLevel(video, video->zoomLevelInital);
        if (keyState[SDL_SCANCODE_2])
            videoSetZoomLevel(video, video->zoomLevel - dTime);
        if (keyState[SDL_SCANCODE_3])
            videoSetZoomLevel(video, video->zoomLevel + dTime);

        if (keyState[SDL_SCANCODE_F])
        {
            if (keyState[SDL_SCANCODE_UP])    cameraPosY -= (250 * dTime);
            if (keyState[SDL_SCANCODE_DOWN])  cameraPosY += (250 * dTime);
            if (keyState[SDL_SCANCODE_LEFT])  cameraPosX -= (250 * dTime);
            if (keyState[SDL_SCANCODE_RIGHT]) cameraPosX += (250 * dTime);
        }
        else
        {
            cameraPosX = player->worldPosX - video->windowWidth  / (video->zoomLevel * 2) + (player->width  / 2);
            cameraPosY = player->worldPosY - video->windowHeight / (video->zoomLevel * 2) + (player->height / 2);
        }

        // Set up collision detection.
        if (mapCoordIsType(map, "floor", player->worldPosX, player->worldPosY + player->height))
            player->flags &= ~(1 << IN_MID_AIR);
        else
            player->flags |= 1 << IN_MID_AIR;

        if (mapCoordIsType(map, "floor", npc->worldPosX, npc->worldPosY + npc->height))
            npc->flags &= ~(1 << IN_MID_AIR);
        else
            npc->flags |= 1 << IN_MID_AIR;


        // Set NPC behavior.
        if ((doIntersect(player->bb, npc->bb)) && 0 == ((npc->flags >> IN_MOTION) & 1))
        {
            npc->flags ^= 1 << DIRECTION;
            npc->flags |= 1 << IN_MOTION;            
        }

        if ((npc->worldPosX > 1632) &&
            (npc->worldPosX < 1648) &&
            ((npc->flags >> DIRECTION) & 1))
            {
                npc->flags &= ~(1 << IN_MOTION);
            }

        if ((npc->worldPosX > 112) &&
            (npc->worldPosX < 128) &&
            0 == ((npc->flags >> DIRECTION) & 1))
            {
                npc->flags &= ~(1 << IN_MOTION);
            }

        // Set camera boundaries to map size.
        int32_t cameraMaxX = (map->width)  - (video->windowWidth  / video->zoomLevel);
        int32_t cameraMaxY = (map->height) - (video->windowHeight / video->zoomLevel);
        if (cameraPosX < 0) cameraPosX = 0;
        if (cameraPosY < 0) cameraPosY = 0;
        if (cameraPosX > cameraMaxX) cameraPosX = cameraMaxX;
        if (cameraPosY > cameraMaxY) cameraPosY = cameraMaxY;

        // Render scene.
        sky->worldPosX = 0;
        sky->worldPosY = map->height - sky->height;
        if (-1 == backgroundRender(video->renderer, sky, map->width, sky->worldPosX - cameraPosX, sky->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        clouds->worldPosX = -player->worldPosX * 0.075;
        clouds->worldPosY = map->height - clouds->height;
        if (-1 == backgroundRender(video->renderer, clouds, map->width, clouds->worldPosX - cameraPosX, clouds->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        sea->worldPosX = -player->worldPosX * 0.1;
        sea->worldPosY = map->height - sea->height;
        if (-1 == backgroundRender(video->renderer, sea, map->width, sea->worldPosX - cameraPosX, sea->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        farGrounds->worldPosX = -player->worldPosX * 0.05;
        farGrounds->worldPosY = map->height - farGrounds->height;
        if (-1 == backgroundRender(video->renderer, farGrounds, map->width, farGrounds->worldPosX - cameraPosX, farGrounds->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "Background", 1, 0, map->worldPosX - cameraPosX, map->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "World", 1, 1, map->worldPosX - cameraPosX, map->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == entityRender(video->renderer, player, player->worldPosX - cameraPosX, player->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == entityRender(video->renderer, npc, npc->worldPosX - cameraPosX, npc->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "Overlay", 1, 2, map->worldPosX - cameraPosX, map->worldPosY - cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (keyState[SDL_SCANCODE_F])
            if (-1 == iconRender(video->renderer, fcMode, video->windowWidth / video->zoomLevel - fcMode->width, 0))
            {
                execStatus = EXIT_FAILURE;
                goto quit;
            }

        SDL_RenderPresent(video->renderer);
        SDL_RenderClear(video->renderer);
    }

    quit:
    musicFree(dead);
    musicFree(music);
    mixerFree(mixer);
    entityFree(npc);
    entityFree(player);
    iconFree(fcMode);
    mapFree(map);
    backgroundFree(farGrounds);
    backgroundFree(sea);
    backgroundFree(clouds);
    backgroundFree(sky);
    videoTerminate(video);
    return execStatus;
}
