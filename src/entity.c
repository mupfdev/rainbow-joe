/** @file entity.c
 * @ingroup   Entity
 * @defgroup  Entity
 * @brief     Handler to take care of game entities such as the player, enemies,
 *            etc.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "entity.h"

Entity *entityInit()
{
    static Entity *entity;
    entity = malloc(sizeof(struct entity_t));
    if (NULL == entity)
    {
        fprintf(stderr, "entityInit(): error allocating memory.\n");
        return NULL;
    }

    return entity;
}
