/** @file aabb.h
 * @ingroup AABB
 */

#ifndef AABB_h
#define AABB_h

#include <stdint.h>

/**
 * @ingroup AABB
 */
typedef struct AABB_t {
    double b;
    double l;
    double r;
    double t;
} AABB;

uint8_t doIntersect(AABB boxA, AABB boxB);

#endif
