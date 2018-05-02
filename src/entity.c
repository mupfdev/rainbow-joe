/** @file entity.c
 * @ingroup   Entity
 * @defgroup  Entity
 * @brief     Handler to take care of game entities such as the player, enemies,
 *            etc.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "entity.h"

/**
 * @brief
 * @param   ent
 * @return  Always 0.
 * @ingroup Entity
 */
static int32_t entityThread(void *ent)
{
    Entity *entity = (Entity *)ent;

    while ((entity->flags >> THREAD_IS_RUNNING) & 1)
    {
        // Update bounding box.
        entity->bb.b = entity->worldPosY + entity->height;
        entity->bb.l = entity->worldPosX;
        entity->bb.r = entity->worldPosX + entity->width;
        entity->bb.t = entity->worldPosY;

        if ((entity->flags >> IN_MID_AIR) & 1)
            entity->velocityFall += entity->gravity;

        if ((entity->flags >> IN_MOTION) & 1)
        {
            entity->velocity   += entity->acceleration;
            // Slowing down frame animation.
            entity->frameDelay += entity->dTime;
            if (entity->frameDelay > 5000)
            {
                entity->frame++;
                entity->frameDelay = 0;
            }
        }
        else
        {
            entity->velocity -= entity->deceleration;
        }

        // Set velocity limits.
        if (entity->velocity > entity->velocityMax)
            entity->velocity = entity->velocityMax;
        if (entity->velocity < 0)
        {
            entity->velocity = 0;
            // Reset frame animation when standing still.
            entity->frame = entity->frameStart;
        }

        if (entity->velocityFall > entity->velocityFallMax)
            entity->velocityFall = entity->velocityFallMax;
        if (entity->velocityFall < 0) entity->velocityFall = 0;

        // Loop frame animation.
        if (entity->frameEnd <= entity->frame)
            entity->frame = entity->frameStart;
    }

    return 0;
}

/**
 * @brief
 * @param entity
 * @ingroup Entity
 */
void entityFree(Entity *entity)
{
    entity->flags &= ~(1 << THREAD_IS_RUNNING);
    SDL_WaitThread(entity->thread, NULL);
    free(entity);
}

/**
 * @brief
 * @param   name
 * @return
 * @ingroup Entity
 */
Entity *entityInit(const char *name)
{
    static Entity *entity;
    entity = malloc(sizeof(struct entity_t));
    if (NULL == entity)
    {
        fprintf(stderr, "entityInit(): error allocating memory.\n");
        return NULL;
    }

    // Default values.
    entity->height          =  32;
    entity->width           =  32;
    entity->bb.b           =   0;
    entity->bb.l           = entity->height;
    entity->bb.r           = entity->width;
    entity->bb.t           =   0;
    entity->acceleration    =   0.2;
    entity->deceleration    =   0.000004;
    entity->flags           =   0;
    entity->fps             =  24;
    entity->frame           =   0;
    entity->frameDelay      =   0;
    entity->frameEnd        = WALK_MAX;
    entity->frameStart      = WALK;
    entity->gravity         =   0.6;
    entity->sprite          = NULL;
    entity->velocity        =   0.0;
    entity->velocityMax     = 100.0;
    entity->velocityFall    =   0.0;
    entity->velocityFallMax = 320.0;
    entity->worldPosX       =   0.0;
    entity->worldPosY       =   0.0;

    entity->flags |= 1 << THREAD_IS_RUNNING;
    entity->thread = SDL_CreateThread(entityThread, name, entity);
    if (NULL == entity->thread)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        free(entity);
        return NULL;
    }

    return entity;
}

/**
 * @brief
 * @param   entity
 * @param   renderer
 * @param   filename
 * @return  0 on success, -1 on error.
 * @ingroup Entity
 */
int8_t entityLoadSprite(Entity *entity, SDL_Renderer *renderer, const char *filename)
{
    if (NULL != entity->sprite)
        SDL_DestroyTexture(entity->sprite);

    entity->sprite = IMG_LoadTexture(renderer, filename);
    if (NULL == entity->sprite)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

/**
 * @brief
 * @param   renderer
 * @param   entity
 * @param   posX
 * @param   posY
 * @return
 * @ingroup Entity
 */
int8_t entityRender(SDL_Renderer *renderer, Entity *entity, double posX, double posY)
{
    if (NULL == entity->sprite)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    SDL_Rect dst = { posX, posY, entity->width, entity->height };
    SDL_Rect src = { entity->frame * entity->width, entity->frameYoffset, entity->width, entity->height };

    SDL_RendererFlip flip;

    if ((entity->flags >> DIRECTION) & 1)
        flip = SDL_FLIP_HORIZONTAL;
    else
        flip = SDL_FLIP_NONE;

    if (-1 == SDL_RenderCopyEx(renderer, entity->sprite, &src, &dst, 0, NULL, flip))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    return 0;
}
