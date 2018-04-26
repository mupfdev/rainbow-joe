/** @file entity.h
 * @ingroup Entity
 */

#ifndef ENTITY_h
#define ENTITY_h

// Flags.
#define IN_MOTION 0

#include <SDL2/SDL.h>

/**
 * @ingroup Entity
 */
typedef struct entity_t {
    uint16_t    flags;
    int32_t     posX;
    int32_t     posY;
    SDL_Texture *sprite;
} Entity;

Entity *entityInit();

#endif
