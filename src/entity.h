/** @file entity.h
 * @ingroup Entity
 */

#ifndef ENTITY_h
#define ENTITY_h

// Flags.
#define DIRECTION          0
#define IN_MOTION          1
#define IN_MID_AIR         2
#define THREAD_IS_RUNNING  3

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
    double      acceleration;
    double      deceleration;
    double      dTime;
    uint16_t    flags;
    uint8_t     fps;
    uint8_t     frame;
    double      frameDelay;
    uint8_t     frameEnd;
    uint8_t     frameStart;
    uint8_t     frameYoffset;
    uint16_t    gid;
    double      gravity;
    SDL_Texture *sprite;
    const char  *spriteImage;
    SDL_Thread  *thread;
    double      velocity;
    double      velocityMax;
    double      velocityFall;
    double      velocityFallMax;
    double      worldPosX;
    double      worldPosY;
} Entity;

void   entityFree(Entity *entity);
Entity *entityInit(const char *name);
int8_t entityLoadSprite(Entity *entity, SDL_Renderer *renderer, const char *filename);
int8_t entityRender(SDL_Renderer *renderer, Entity *entity, double posX, double posY);

#endif
