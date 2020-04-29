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
static inline vec2 inverse_transform(vec2 point, float rotation_angle, vec2 translation)
{
    vec2 rotation = vec2_angle(rotation_angle);
    return vec2_rotate(vec2_sub(point, translation), rotation);
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

//-----------------------------------------------------------------------------
static inline float sd_uneven_capsule( vec2 p, float r1, float r2, float h )
{
    p.x = fabsf(p.x);
    float b = (r1-r2)/h;
    float a = sqrtf(1.f-b*b);
    float k = vec2_dot(p,(vec2){-b,a});
    if( k < 0.f ) return vec2_length(p) - r1;
    if( k > a*h ) return vec2_length(vec2_sub(p, (vec2){0.0,h})) - r2;
    return vec2_dot(p, (vec2){a,b}) - r1;
}

//-----------------------------------------------------------------------------
static inline float sign_float(float value)
{
    if (value < 0.f)
        return -1.f;
    if (value > 0.f)
        return 1.f;
    return 0.f;
}

//-----------------------------------------------------------------------------
static inline float sd_triangle(vec2 p, vec2 p0, vec2 p1, vec2 p2)
{
    vec2 e0 = vec2_sub(p1, p0);
    vec2 e1 = vec2_sub(p2, p1);
    vec2 e2 = vec2_sub(p0, p2);
    vec2 v0 = vec2_sub(p, p0);
    vec2 v1 = vec2_sub(p, p1);
    vec2 v2 = vec2_sub(p, p2);
    vec2 pq0 = vec2_sub(v0, vec2_scale(e0, clamp_float(vec2_dot(v0,e0)/vec2_dot(e0,e0), 0.f, 1.f)));
    vec2 pq1 = vec2_sub(v1, vec2_scale(e1, clamp_float(vec2_dot(v1,e1)/vec2_dot(e1,e1), 0.f, 1.f)));
    vec2 pq2 = vec2_sub(v2, vec2_scale(e2, clamp_float(vec2_dot(v2,e2)/vec2_dot(e2,e2), 0.f, 1.f)));
    float s = sign_float(e0.x*e2.y - e0.y*e2.x);
    vec2 d = vec2_min(vec2_min((vec2){vec2_dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)},
                               (vec2){vec2_dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x)}),
                               (vec2){vec2_dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)});
    return -sqrtf(d.x)*sign_float(d.y);
}

#endif
