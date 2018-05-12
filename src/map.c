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
 * @brief   Check whether a tile is from a specific type or not.
 * @param   map  the map.
 * @param   type name of the tile type to look for.
 * @param   xPos coordinate along the x-axis.
 * @param   yPos coordinate along the y-axis.
 * @return  1 if the tile is of the specific type, 0 if not.
 * @ingroup Map
 */
uint8_t mapCoordIsType(Map *map, const char *type, double xPos, double yPos)
{
    xPos = xPos / map->map->tile_width + 1;
    yPos = yPos / map->map->tile_height;

    // Prevent segfaults by setting boundaries.
    if ((xPos < 0) ||
        (yPos < 0) ||
        (xPos > map->map->width) ||
        (yPos > map->map->height))
            return 0;

    tmx_layer *layers = map->map->ly_head;
    while(layers)
    {
        uint16_t gid = layers->content.gids[((int32_t)yPos * map->map->width) + (int32_t)xPos] & TMX_FLIP_BITS_REMOVAL;
        if (NULL != map->map->tiles[gid])
            if (NULL != map->map->tiles[gid]->type)
                if (0 == strcmp(type, map->map->tiles[gid]->type))
                    return 1;
        layers = layers->next;
    }

    return 0;
}

/**
 * @brief   Free map.
 * @param   map the map that should be freed.
 * @ingroup Map
 */
void mapFree(Map *map)
{
    tmx_map_free(map->map);
}

/**
 * @brief   Initialise map.
 * @param   filename the TMX map file to load.
 * @return  The initialised map on success, NULL on error.
 * @ingroup Map
 */
Map *mapInit(const char *filename)
{
    static Map *map;
    map = malloc(sizeof(struct map_t));
    if (NULL == map)
    {
        fprintf(stderr, "mapInit(): error allocating memory.\n");
        return NULL;
    }

    map->map = tmx_load(filename);
    if (NULL == map->map) {
        fprintf(stderr, "%s\n", tmx_strerr());
        return NULL;
    }

    map->height    = map->map->height * map->map->tile_height;
    map->width     = map->map->width  * map->map->tile_width;
    map->worldPosX = 0;
    map->worldPosY = 0;

    for (uint8_t i = 0; i < MAX_TEXTURES_PER_MAP; i++)
        map->texture[i] = NULL;

    return map;
}

/**
 * @brief   Render Map on screen.
 * @param   renderer SDL's rendering context.  See @ref struct Video.
 * @param   map      the map that should be rendered.
 * @param   name     substring of the layer name(s) that should be rendered.
 * @param   bg       boolean value to determine if the map's background colour
 *                   should be rendered or not.  If set to 0, the background
 *                   stays transparent.
 * @param   index    determine the texture index.  The total amount of textures
 *                   per map is defined by MAP_TEXTURES_PER_MAP.
 * @param   posX     coordinate, where the map should be rendered along the
 *                   x-axis of the set rendering context.
 * @param   posY     coordinate, where the map should be rendered along the
 *                   y-axis of the set rendering context.
 * @return  0 on success, -1 on error.
 * @ingroup Map
 */
int8_t mapRender(
    SDL_Renderer *renderer,
    Map          *map,
    const char   *name,
    uint8_t      bg,
    uint8_t      index,
    double       posX,
    double       posY)
{
    // Render texture if already generated.
    if (map->texture[index])
    {
        SDL_Rect dst = { posX, posY, map->map->width * map->map->tile_width, map->map->height * map->map->tile_height };
        if (-1 == SDL_RenderCopyEx(renderer, map->texture[index], NULL, &dst, 0, NULL, SDL_FLIP_NONE))
        {
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }
        return 0;
    }

    // Generate texture.
    map->texture[index] = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        map->map->width  * map->map->tile_width,
        map->map->height * map->map->tile_height);

    if (NULL == map->texture[index])
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    SDL_Texture *tileset = IMG_LoadTexture(renderer, "res/tilesets/tileset.png");
    if (NULL == tileset)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (0 != SDL_SetRenderTarget(renderer, map->texture[index]))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (bg)
        SDL_SetRenderDrawColor(
            renderer,
            (map->map->backgroundcolor >> 16) & 0xFF,
            (map->map->backgroundcolor >>  8) & 0xFF,
            (map->map->backgroundcolor)       & 0xFF,
            255);

    tmx_layer *layers = map->map->ly_head;
    while(layers)
    {
        uint32_t    gid;
        SDL_Rect    dst;
        SDL_Rect    src;
        tmx_tileset *ts;

        if ( (layers->visible) && (NULL != strstr(layers->name, name)) )
        {
            for (uint32_t ih = 0; ih < map->map->height; ih++)
                for (uint32_t iw = 0; iw < map->map->width; iw++)
                {
                    gid = layers->content.gids[(ih * map->map->width) + iw] & TMX_FLIP_BITS_REMOVAL;
                    if (NULL != map->map->tiles[gid])
                    {
                        ts    = map->map->tiles[gid]->tileset;
                        src.x = map->map->tiles[gid]->ul_x;
                        src.y = map->map->tiles[gid]->ul_y;
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

    // Switch back to default render target.
    if (0 != SDL_SetRenderTarget(renderer, NULL))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (0 != SDL_SetTextureBlendMode(map->texture[index], SDL_BLENDMODE_BLEND))
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    return 0;
}
