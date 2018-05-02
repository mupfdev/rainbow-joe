/** @file aabb.c
 * @ingroup   AABB
 * @defgroup  AABB
 * @brief     Simple axis-aligned bounding boxes manager.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "aabb.h"

uint8_t doIntersect(AABB boxA, AABB boxB)
{
    double dAx = boxB.l - boxA.r;
    double dAy = boxB.t - boxA.b;
    double dBx = boxA.l - boxB.r;
    double dBy = boxA.t - boxB.b;

    if (dAx > 0.0 || dAy > 0.0) return 0;
    if (dBx > 0.0 || dBy > 0.0) return 0;

    return 1;
}
