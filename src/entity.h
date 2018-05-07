/** @file entity.h
 * @ingroup Entity
 */

#ifndef ENTITY_h
#define ENTITY_h

#include "aabb.h"

// Flags.
#define DIRECTION         0
#define IN_MID_AIR        1
#define IN_MOTION         2
#define IS_DEAD           3
#define IS_JUMPING        4

// Frames.
#define FALL      6
#define FALL_MAX  7
#define JUMP      4
#define JUMP_MAX  5
#define RUN      14
#define RUN_MAX  17
#define WALK      0
#define WALK_MAX  3

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * @ingroup Entity
 */
typedef struct entity_t
{
    double   acceleration;
    double   deceleration;
    uint16_t flags;
    double   fps;
    uint8_t  frameEnd;
    uint8_t  frameStart;
    uint8_t  frameYoffset;
    uint8_t  height;
    double   jumpGravityFactor;
    double   jumpTimeMax;
    double   velocityMax;
    uint8_t  width;
    uint32_t worldHeight;
    double   worldGravitation;
    double   worldMeterInPixel;
    double   worldPosX;
    double   worldPosY;
    uint32_t worldWidth;

    /* These variables are used internally to store volatile values and usually
     * do not have to be changed manually. */
    AABB        bb;
    double      distanceFall;
    uint8_t     frame;
    double      frameTime;
    double      jumpTime;
    double      velocityJump;
    SDL_Texture *sprite;
    double      velocity;
    double      velocityFall;

} Entity;

void   entityFrame(Entity *entity, double dTime);
void   entityFree(Entity *entity);
Entity *entityInit();
int8_t entityLoadSprite(Entity *entity, SDL_Renderer *renderer, const char *filename);
int8_t entityRender(SDL_Renderer *renderer, Entity *entity, double posX, double posY);

#endif
