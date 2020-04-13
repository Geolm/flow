#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "vec2.h"
#include "stdbool.h"

//-----------------------------------------------------------------------------
typedef struct
{
    vec2 p0;
    vec2 p1;
} segment;

typedef struct
{
    vec2 min;
    vec2 max;
} aabb;

//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

bool intersection_segment_line(segment seg, segment line, vec2* intersection_point);
bool intersection_segment_segment(segment seg0, segment seg1, vec2* intersection_point);
bool intersection_aabb_circle(aabb box, vec2 center, float radius);
bool intersection_aabb_circle_info(aabb  box, vec2 center, float radius, vec2* nearest_point, float* penetration);
bool intersect_aabb_segment(aabb box, segment seg);
bool intersection_circle_circle(vec2 center0, float radius0, vec2 center1, float radius1);    

bool test_point_triangle(vec2 p0, vec2 p1, vec2 p2, vec2 point);

#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------
static inline bool test_point_circle(vec2 center, float radius, vec2 point)
{
    return (vec2_sq_distance(center, point) < radius*radius);
}

//-----------------------------------------------------------------------------
static inline bool test_point_aabb(aabb box, vec2 point)
{
    return !(point.x < box.min.x || point.y < box.min.y || point.x > box.max.x || point.y > box.max.y);
}

//-----------------------------------------------------------------------------
static inline bool intersection_aabb_aabb(aabb box0, aabb box1)
{
    return !(box1.max.x < box0.min.x || box0.max.x < box1.min.x || box1.max.y < box0.min.y || box0.max.y < box1.min.y);
}

#endif // __COLLISION_H__
