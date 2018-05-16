/** @file hud.h
 * @ingroup HUD
 */

#ifndef HUD_h
#define HUD_h

#include <SDL2/SDL.h>
#include <stdint.h>

/**
 * @def     iconFree()
 *          Free icon structure.
 * @ingroup HUD
 */
#define iconFree(icon) free(icon)

/**
 * @ingroup HUD
 */
typedef struct icon_t {
    uint8_t     height;
    SDL_Texture *icon;
    double      renderPosX;
    double      renderPosY;
    uint8_t     width;
} Icon;

Icon   *iconInit(SDL_Renderer *renderer, const char *filename);
int8_t iconRender(SDL_Renderer *renderer, Icon *icon, double cameraPosX, double cameraPosY);

#endif
