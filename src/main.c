/** @file main.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "main.h"

int32_t main(int32_t argc, char *argv[])
{
    int execStatus  = EXIT_SUCCESS;

    const char *configFilename;
    if (argc > 1) configFilename = argv[1];
    else configFilename = "default.ini";

    Config config  = configInit(configFilename);
    Video  *video  = NULL;
    Map    *map    = NULL;
    Mixer  *mixer  = NULL;
    Music  *music  = NULL;
    Icon   *fcMode = NULL;

    Background *bg[NUM_BACKGROUNDS];
    for (uint32_t i = 0; i < NUM_BACKGROUNDS; i++)
        bg[i] = NULL;

    Entity *entity[NUM_ENTITIES];
    for (uint32_t i = 0; i < NUM_ENTITIES; i++)
        entity[i] = NULL;

    SFX *sfx[NUM_SFX];
    for (uint32_t i = 0; i < NUM_SFX; i++)
        sfx[i] = NULL;

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

    map = mapInit("res/maps/01.tmx");
    if (NULL == map)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    /* Note: The error handling isn't missing here.  There is simply no need to
     * quit the program if the music can't be played by some reason. */
    mixer = mixerInit();
    music = musicInit("res/music/enchanted-tiki-86.ogg");
    musicFadeIn(music, -1, 8000);

    fcMode = iconInit(video->renderer, "res/icons/telescope.png");
    if (NULL == fcMode)
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }

    bg[0] = backgroundInit(video->renderer, "res/backgrounds/sky.png", map->width);
    if (NULL == bg[0])
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    bg[0]->worldPosY = map->height - bg[0]->height;

    bg[1] = backgroundInit(video->renderer, "res/backgrounds/clouds.png", map->width);
    if (NULL == bg[1])
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    bg[1]->worldPosY = map->height - bg[1]->height;

    bg[2] = backgroundInit(video->renderer, "res/backgrounds/sea.png", map->width);
    if (NULL == bg[2])
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    bg[2]->worldPosY = map->height - bg[2]->height;

    bg[3] = backgroundInit(video->renderer, "res/backgrounds/far-grounds.png", map->width);
    if (NULL == bg[3])
    {
        execStatus = EXIT_FAILURE;
        goto quit;
    }
    bg[3]->worldPosY = map->height - bg[3]->height;

    for (uint32_t i = 0; i < NUM_ENTITIES; i++)
    {
        entity[i] = entityInit();
        if (NULL == entity[i])
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }
        if (-1 == entityLoadSprite(entity[i], video->renderer, "res/sprites/characters.png"))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }
        entity[i]->worldWidth  = map->width;
        entity[i]->worldHeight = map->height;
    }

    // Set up individual entities.
    // Player.
    entity[PLAYER_ENTITY]->frameYoffset =  64;
    entity[PLAYER_ENTITY]->respawnPosX  =  32;
    entity[PLAYER_ENTITY]->respawnPosY  = 608;
    entity[PLAYER_ENTITY]->worldPosX    =  32;
    entity[PLAYER_ENTITY]->worldPosY    = 608;
    // NPCs.
    entity[1]->respawnPosX  =  112;
    entity[1]->respawnPosY  =  400;
    entity[1]->worldPosX    =  112;
    entity[1]->worldPosY    =  400;
    entity[2]->respawnPosX  =  256;
    entity[2]->respawnPosY  =   80;
    entity[2]->worldPosX    =  256;
    entity[2]->worldPosY    =   80;
    entity[3]->frameYoffset =   64;
    entity[3]->respawnPosX  =  496;
    entity[3]->respawnPosY  =  160;
    entity[3]->worldPosX    =  496;
    entity[3]->worldPosY    =  160;
    entity[4]->frameYoffset =    0;
    entity[4]->respawnPosX  = 3696;
    entity[4]->respawnPosY  =   80;
    entity[4]->worldPosX    = 3696;
    entity[4]->worldPosY    =   80;

    sfx[SFX_DEAD] = sfxInit("res/sfx/dead.wav");
    sfx[SFX_JUMP] = sfxInit("res/sfx/jump.wav");

    double cameraPosX = 0;
    double cameraPosY = map->height - video->windowHeight;
    double timeA      = SDL_GetTicks();
    double delay      = 0;
    while (1)
    {
        double timeB  = SDL_GetTicks();
        double dTime  = (timeB - timeA) / 1000;
        timeA         = timeB;

        // Limit FPS.
        if (config.video.limitFPS)
            SDL_Delay(1000 / config.video.fps - dTime);

        for (uint32_t i = 0; i < NUM_ENTITIES; i++)
        {
            entityFrame(entity[i], dTime);
            if ((entity[i]->flags >> IS_DEAD) & 1)
                if (PLAYER_ENTITY != i)
                    entityRespawn(entity[i]);
        }

        if ((entity[PLAYER_ENTITY]->flags >> IS_DEAD) & 1)
        {
            if (0 == delay)
                if (mixer) sfxPlay(sfx[SFX_DEAD], CH_DEAD, 0);
            delay += dTime;

            if (delay > 2)
            {
                entity[PLAYER_ENTITY]->flags &= ~(1 << IS_DEAD);
                for (uint32_t i = 0; i < NUM_ENTITIES; i++)
                    entityRespawn(entity[i]);
                delay =  0;
            }
        }

        // Handle keyboard input.
        const uint8_t *keyState;
        SDL_PumpEvents();
        if (SDL_PeepEvents(0, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0)
            goto quit;
        keyState = SDL_GetKeyboardState(NULL);
        // Reset IN_MOTION flag (in case no key is pressed).
        entity[PLAYER_ENTITY]->flags &= ~(1 << IN_MOTION);

        if (keyState[SDL_SCANCODE_LSHIFT])
        {
            // Allow running only when not in mid-air.
            if (0 == ((entity[PLAYER_ENTITY]->flags >> IN_MID_AIR) & 1))
            {
                entity[PLAYER_ENTITY]->velocityMax  = 250;
                entity[PLAYER_ENTITY]->frameStart   = RUN;
                entity[PLAYER_ENTITY]->frameEnd     = RUN_MAX;
            }
        }
        else
        {
            // Don't allow to slow down in mid-air.
            if (0 == ((entity[PLAYER_ENTITY]->flags >> IN_MID_AIR) & 1))
            {
                entity[PLAYER_ENTITY]->velocityMax  = 100;
                entity[PLAYER_ENTITY]->frameStart   = WALK;
                entity[PLAYER_ENTITY]->frameEnd     = WALK_MAX;
            }
        }
        if (keyState[SDL_SCANCODE_Q]) goto quit;
        if (keyState[SDL_SCANCODE_A])
        {
            if (0 == ((entity[PLAYER_ENTITY]->flags >> DIRECTION) & 1))
                entity[PLAYER_ENTITY]->velocity = -entity[PLAYER_ENTITY]->velocity;
            entity[PLAYER_ENTITY]->flags |= 1 << IN_MOTION;
            entity[PLAYER_ENTITY]->flags |= 1 << DIRECTION;
        }
        if (keyState[SDL_SCANCODE_D])
        {
            if ((entity[PLAYER_ENTITY]->flags >> DIRECTION) & 1)
                entity[PLAYER_ENTITY]->velocity = -entity[PLAYER_ENTITY]->velocity;

            entity[PLAYER_ENTITY]->flags |= 1   << IN_MOTION;
            entity[PLAYER_ENTITY]->flags &= ~(1 << DIRECTION);
        }

        if (0 == ((entity[PLAYER_ENTITY]->flags >> IN_MID_AIR) & 1))
            if (keyState[SDL_SCANCODE_SPACE])
            {
                if (mixer) sfxPlay(sfx[SFX_JUMP], CH_JUMP, 0);
                entity[PLAYER_ENTITY]->flags        |= 1 << IS_JUMPING;
                entity[PLAYER_ENTITY]->velocityJump  = entity[PLAYER_ENTITY]->velocity;
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
            cameraPosX = entity[PLAYER_ENTITY]->worldPosX - video->windowWidth  / (video->zoomLevel * 2) + (entity[PLAYER_ENTITY]->width  / 2);
            cameraPosY = entity[PLAYER_ENTITY]->worldPosY - video->windowHeight / (video->zoomLevel * 2) + (entity[PLAYER_ENTITY]->height / 2);
        }

        // Set up collision detection.
        for (uint32_t i = 0; i < NUM_ENTITIES; i++)
        {
            if (mapCoordIsType(map, "floor", entity[i]->worldPosX, entity[i]->worldPosY + entity[i]->height))
                entity[i]->flags &= ~(1 << IN_MID_AIR);
            else
                entity[i]->flags |= 1 << IN_MID_AIR;
        }

        // Set NPC behavior.
        for (uint32_t i = 1; i < NUM_ENTITIES; i++)
        {
            if ((doIntersect(entity[PLAYER_ENTITY]->bb, entity[i]->bb)))
            {
                if (entity[PLAYER_ENTITY]->worldPosX > entity[i]->worldPosX)
                    entity[i]->flags |= 1 << DIRECTION;            
                else
                    entity[i]->flags &= ~(1 << DIRECTION);

                entity[i]->flags |= 1 << IN_MOTION;            
            }
        }

        // Set camera boundaries to map size.
        int32_t cameraMaxX = (map->width)  - (video->windowWidth  / video->zoomLevel);
        int32_t cameraMaxY = (map->height) - (video->windowHeight / video->zoomLevel);
        if (cameraPosX < 0) cameraPosX = 0;
        if (cameraPosY < 0) cameraPosY = 0;
        if (cameraPosX > cameraMaxX) cameraPosX = cameraMaxX;
        if (cameraPosY > cameraMaxY) cameraPosY = cameraMaxY;

        // Render scene.
        if (-1 == backgroundRender(video->renderer, bg[0], cameraPosX, cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == backgroundRender(video->renderer, bg[1], cameraPosX * 0.05, cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == backgroundRender(video->renderer, bg[2], cameraPosX * 0.15, cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == backgroundRender(video->renderer, bg[3], cameraPosX * 0.1, cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "Background", 1, 0, cameraPosX, cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        if (-1 == mapRender(video->renderer, map, "World", 1, 1, cameraPosX, cameraPosY))
        {
            execStatus = EXIT_FAILURE;
            goto quit;
        }

        for (uint32_t i = 0; i < NUM_ENTITIES; i++)
            if (-1 == entityRender(video->renderer, entity[i], cameraPosX, cameraPosY))
            {
                execStatus = EXIT_FAILURE;
                goto quit;
            }

        if (-1 == mapRender(video->renderer, map, "Overlay", 1, 2, cameraPosX, cameraPosY))
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
    for (uint32_t i = 0; i < NUM_SFX; i++)
        sfxFree(sfx[i]);

    for (uint32_t i = 0; i < NUM_ENTITIES; i++)
        entityFree(entity[i]);

    for (uint32_t i = 0; i < NUM_BACKGROUNDS; i++)
        backgroundFree(bg[i]);

    iconFree(fcMode);
    musicFree(music);
    mixerFree(mixer);
    mapFree(map);
    videoTerminate(video);

    return execStatus;
}
