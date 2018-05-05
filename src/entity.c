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
 * @brief   Update entity.  Thie function has to be called every frame.
 * @param   entity
 * @param   dTime
 * @ingroup Entity
 */
void entityFrame(Entity *entity, double dTime)
{
    // Update bounding box.
    entity->bb.b = entity->worldPosY + entity->height;
    entity->bb.l = entity->worldPosX;
    entity->bb.r = entity->worldPosX + entity->width;
    entity->bb.t = entity->worldPosY;

    // Increase/decrease vertical velocity if player is in motion.
    if ((entity->flags >> IN_MOTION)  & 1)
        entity->velocity += entity->acceleration * dTime;
    else
        entity->velocity -= entity->deceleration * dTime;

    // Set vertical velocity limits.
    if (entity->velocity > entity->velocityMax)
        entity->velocity = entity->velocityMax;
    if (entity->velocity < 0)
    {
        entity->velocity = 0;
        // Reset frame animation when standing still.
        entity->frame    = entity->frameStart;
    }

    // Update frame.
    if ((entity->flags >> IN_MOTION)  & 1)
    {
        entity->frameTime += dTime;

        if (entity->frameTime > 1 / entity->fps)
        {
            entity->frame++;
            entity->frameTime = 0;
        }
    }

    // Loop frame animation.
    if (entity->frameEnd <= entity->frame)
        entity->frame = entity->frameStart;

    // Set vertical player position.
    if (entity->velocity > 0)
    {
        if ((entity->flags >> DIRECTION) & 1)
            entity->worldPosX -= (entity->velocity * dTime);
        else
            entity->worldPosX += (entity->velocity * dTime);
    }

    // Set horizontal player position.
    if ((entity->flags >> IS_JUMPING) & 1)
        entity->flags |= 1 << IN_MID_AIR;

    if ((entity->flags >> IN_MID_AIR) & 1)
    {
        double g = (entity->worldMeterInPixel * entity->worldGravitation);
        if ((entity->flags >> IS_JUMPING) & 1)
        {
            if (entity->velocityFall > entity->jump)
            {
                g += entity->velocityJump;
                g = -g * 4;
            }
            else
                entity->flags &= ~(1 << IS_JUMPING);
        }

        entity->distanceFall  = g * dTime * dTime;
        entity->velocityFall += entity->distanceFall;
        entity->worldPosY    += entity->velocityFall;
    }
    else
    {
        entity->flags &= ~(1 << IS_JUMPING);
        entity->velocityFall  = 0;
    }

    //
    if (entity->worldPosX < 0 - (entity->width / 2))
        entity->worldPosX = entity->worldWidth - (entity->width / 2);

    if (entity->worldPosX > entity->worldWidth - (entity->width / 2))
        entity->worldPosX = 0;

    if (entity->worldPosY > entity->worldHeight + entity->height)
        entity->flags |= 1 << IS_DEAD;
}

/**
 * @brief
 * @param entity
 * @ingroup Entity
 */
void entityFree(Entity *entity)
{
    free(entity);
}

/**
 * @brief
 * @param   name
 * @return
 * @ingroup Entity
 */
Entity *entityInit()
{
    static Entity *entity;
    entity = malloc(sizeof(struct entity_t));
    if (NULL == entity)
    {
        fprintf(stderr, "entityInit(): error allocating memory.\n");
        return NULL;
    }

    // Default values.
    entity->height            =   32;
    entity->width             =   32;
    entity->bb.b              =    0;
    entity->bb.l              = entity->height;
    entity->bb.r              = entity->width;
    entity->bb.t              =    0;
    entity->acceleration      =  400;
    entity->deceleration      =  200;
    entity->distanceFall      =    0;
    entity->flags             =    0;
    entity->fps               =   12;
    entity->frame             =    0;
    entity->frameEnd          = WALK_MAX;
    entity->frameStart        = WALK;
    entity->frameTime         =    0.0;
    entity->jump              =   -0.25;
    entity->sprite            = NULL;
    entity->velocity          =    0.0;
    entity->velocityFall      =    0.0;
    entity->velocityJump      =    0.0;
    entity->velocityMax       =  100.0;
    entity->worldHeight       =      0;
    entity->worldGravitation  =    9.81;
    entity->worldMeterInPixel =     32;
    entity->worldPosX         =    0.0;
    entity->worldPosY         =    0.0;
    entity->worldWidth        =      0;

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
