/** @file map.c
 * @ingroup   Map
 * @defgroup  Map
 * @brief     Handles TMX (Tile Map XML) map files that can be edited using the
 *            Tiled Map Editor.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "map.h"

/**
 * @brief   Load TMX map.
 * @param   filename the TMX map file to load.
 * @return  A TMX map on success, NULL on error.
 * @ingroup Map
 */
tmx_map *mapLoad(const char *filename)
{
    tmx_map *map = tmx_load(filename);

    if (NULL == map) {
        fprintf(stderr, "%s\n", tmx_strerr());
        return NULL;
    }

    return map;
}

/**
 * @brief   Render map on SDL_Texture.  Should only be called once per map.
 * @param   renderer SDL's rendering context.  See @ref struct Video.
 * @param   map      the TMX map that should be rendered.
 * @param   bg       boolean value to determine if the map's background colour
 *                   should be rendered or not.  Otherwise the background stays
 *                   transparent.
 * @param   tileset  SDL texture used as tileset.
 * @param   name     substring of the layer name(s) that should be rendered.
 * @return  A SDL texture holding the rendered map, NULL on error.
 * @ingroup Map
 */
SDL_Texture *mapRender(SDL_Renderer *renderer, tmx_map *map, uint8_t bg, const char *name)
{
    SDL_Texture *dest = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        map->width  * map->tile_width,
        map->height * map->tile_height);

    if (NULL == dest)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture *tileset = IMG_LoadTexture(renderer, "res/tilesets/sheet.png");
    if (NULL == tileset)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }

    if (0 != SDL_SetRenderTarget(renderer, dest))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }

    if (bg)
    {
        SDL_SetRenderDrawColor(
            renderer,
            (map->backgroundcolor >> 16) & 0xFF,
            (map->backgroundcolor >>  8) & 0xFF,
            (map->backgroundcolor)       & 0xFF,
            255);
        SDL_RenderClear(renderer);
    }

    tmx_layer *layers = map->ly_head;
    while(layers)
    {
        uint32_t    gid;
        SDL_Rect    dst;
        SDL_Rect    src;
        tmx_tileset *ts;

        if ( (layers->visible) && (NULL != strstr(layers->name, name)) )
        {
            for (uint32_t ih = 0; ih < map->height; ih++)
                for (uint32_t iw = 0; iw < map->width; iw++)
                {
                    gid = layers->content.gids[(ih * map->width) + iw] & TMX_FLIP_BITS_REMOVAL;
                    if (NULL != map->tiles[gid])
                    {
                        ts       = map->tiles[gid]->tileset;
                        src.x = map->tiles[gid]->ul_x;
                        src.y = map->tiles[gid]->ul_y;
                        src.w = dst.w = ts->tile_width;
                        src.h = dst.h = ts->tile_height;
                        dst.x = iw * ts->tile_width;
                        dst.y = ih * ts->tile_height;
                        SDL_RenderCopy(renderer, tileset, &src, &dst);
                    }
                }
        }
        layers = layers->next;
    }

    SDL_DestroyTexture(tileset);

    // Switch back to default render target.
    if (0 != SDL_SetRenderTarget(renderer, NULL))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }

    if (0 != SDL_SetTextureBlendMode(dest, SDL_BLENDMODE_BLEND))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }

    return dest;
}

/**
 * @brief   Terminate TMX map.
 * @param   map the TMX map that should be freed.
 * @ingroup Map
 */
void mapFree(tmx_map *map)
{
    tmx_map_free(map);
}
