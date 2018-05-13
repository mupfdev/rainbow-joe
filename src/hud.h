/** @file hud.h
 * @ingroup HUD
 */

#ifndef HUD_h
#define HUD_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

void   iconFree(Icon *icon);
Icon   *iconInit(SDL_Renderer *renderer, const char *filename);
int8_t iconRender(SDL_Renderer *renderer, Icon *icon, double cameraPosX, double cameraPosY);

#endif
