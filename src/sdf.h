#ifndef __SDF_H__
#define __SDF_H__

#include "extern/vec2.h"
#include "extern/clamp.h"

// based on https://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm


typedef float (*fetch_sdf)(vec2 p);

//-----------------------------------------------------------------------------
static inline vec2 compute_sdf_normal(fetch_sdf func, vec2 p, float epsilon)
{
    float ddx = func(vec2_add(p, (vec2) {epsilon, 0.f})) - func(vec2_add(p, (vec2) {-epsilon, 0.f}));
    float ddy = func(vec2_add(p, (vec2) {0.f, epsilon})) - func(vec2_add(p, (vec2) {0.f, -epsilon}));
    vec2 normal = {ddx,  ddy};
    vec2_normalize(&normal);
    return normal;
}

//-----------------------------------------------------------------------------
static inline float compute_sdf_angle(fetch_sdf func, vec2 p, float epsilon)
{
    vec2 normal = compute_sdf_normal(func, p, epsilon);
    vec2 tangent = vec2_skew(normal);
    return atan2f(tangent.y, tangent.x);
}

//-----------------------------------------------------------------------------
static inline float smooth_min_cubic( float a, float b, float k )
{
    // polynomial smooth min (k = 0.1);
    float h = fmaxf( k-fabsf(a-b), 0.0f)/k;
    return fminf(a, b) - h*h*h*k*(1.0f/6.0f);
}

//-----------------------------------------------------------------------------
static inline float sd_circle(vec2 p, vec2 c, float r)
{
    return vec2_distance(p, c) - r;
}

//-----------------------------------------------------------------------------
static inline float sd_segment(vec2 p, vec2 a, vec2 b)
{
    vec2 pa = vec2_sub(p, a);
    vec2 ba = vec2_sub(b, a);
    float h = clamp_float(vec2_dot(pa,ba)/vec2_dot(ba,ba), 0.f, 1.f);
    
    return vec2_sq_length(vec2_sub(pa, vec2_scale(ba, h)));
}


#endif
