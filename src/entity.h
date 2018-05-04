/** @file entity.h
 * @ingroup Entity
 */

#ifndef ENTITY_h
#define ENTITY_h

#include "aabb.h"

// Flags.
#define DIRECTION          0
#define IS_DEAD            1
#define IN_MID_AIR         2
#define IN_MOTION          3
#define THREAD_IS_RUNNING  4

// Frames.
#define WALK      0
#define WALK_MAX  3
#define RUN      14
#define RUN_MAX  17

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * @ingroup Entity
 */
typedef struct entity_t
{
    double   acceleration;
    double   deceleration;
    double   dTime;
    uint16_t flags;
    uint8_t  frame;
    double   frameDelay;
    double   frameDelayMax;
    uint8_t  frameEnd;
    uint8_t  frameStart;
    uint8_t  frameYoffset;
    uint32_t gameLoopCount;
    uint8_t  height;
    double   velocity;
    double   velocityMax;
    uint8_t  width;
    uint32_t worldHeight;
    double   worldGravitation;
    double   worldPosX;
    double   worldPosY;
    uint32_t worldWidth;

    /* These variables are used internally to store volatile values and usually
     * do not have to be changed manually. */
    AABB        bb;
    uint32_t    gameLoopCountPrev;
    SDL_Texture *sprite;
    SDL_Thread  *thread;
} Entity;

void   entityFree(Entity *entity);
Entity *entityInit(const char *name);
int8_t entityLoadSprite(Entity *entity, SDL_Renderer *renderer, const char *filename);
int8_t entityRender(SDL_Renderer *renderer, Entity *entity, double posX, double posY);

#endif
