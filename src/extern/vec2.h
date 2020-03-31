#ifndef __VEC2__
#define __VEC2__

#include <math.h>
#include <float.h>
#include <stdbool.h>

typedef struct 
{
    float x, y;
} vec2;

static inline vec2 vec2_angle(float angle)
{
    return (vec2) {cosf(angle), sinf(angle)};
}

static inline vec2 vec2_add(vec2 a, vec2 b)
{
    return (vec2) {a.x + b.x, a.y + b.y};
}

static inline vec2 vec2_sub(vec2 a, vec2 b)
{
    return (vec2) {a.x - b.x, a.y - b.y};
}

static inline vec2 vec2_scale(vec2 a, float f)
{
    return (vec2) {a.x * f, a.y * f};
}

static inline vec2 vec2_rotate(vec2 point, vec2 rotation)
{
    vec2 result;
    result.x = point.x * rotation.x - point.y * rotation.y;
    result.y = point.y * rotation.x + point.x * rotation.y;
    return result;
}

static inline vec2 vec2_skew(vec2 v)
{
    return (vec2) {-v.y, v.x};
}

static inline vec2 vec2_mul(vec2 a, vec2 b)
{
    return (vec2){a.x * b.x, a.y * b.y};
}

static inline float vec2_dot(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

static inline float vec2_sq_length(vec2 v)
{
    return vec2_dot(v, v);
}

static inline float vec2_length(vec2 v)
{
    return sqrtf(vec2_sq_length(v));
}

static inline float vec2_normalize(vec2* v)
{
    float norm = vec2_length(*v);
    if (norm<FLT_EPSILON)
        return 0.f;

    *v = vec2_scale(*v, 1.f / norm);
    return norm;
}

static inline float vec2_sq_distance(vec2 a, vec2 b)
{
    return vec2_sq_length(vec2_sub(b, a));
}

static inline float vec2_distance(vec2 a, vec2 b)
{
    return vec2_length(vec2_sub(b, a));
}

static inline vec2 vec2_reflect(vec2 v, vec2 normal)
{        
    return vec2_sub(v, vec2_scale(normal, vec2_dot(v, normal) * 2.f));
}

static inline vec2 vec2_min(vec2 v, vec2 op)
{
    return (vec2) {.x = fmin(v.x, op.x), .y = fmin(v.y, op.y) };
}

static inline vec2 vec2_max(vec2 v, vec2 op)
{
    return (vec2) {.x = fmax(v.x, op.x), .y = fmax(v.y, op.y) };
}

static inline vec2 vec2_clamp(vec2 v, vec2 lower_bound, vec2 higher_bound)
{
    return vec2_min(vec2_max(v, lower_bound), higher_bound);
}

static inline bool vec2_equal(vec2 a, vec2 b)
{
    return (a.x == b.x && a.y == b.y);
}

static inline bool vec2_similar(vec2 a, vec2 b, float epsilon)
{
    return  (fabs(a.x - b.x) < epsilon) && (fabs(a.y - b.y) < epsilon);
}

static inline vec2 vec2_quadratic_bezier(vec2 p0, vec2 p1, vec2 p2, float t)
{
    float one_minus_t = 1.f - t;
    return vec2_add(vec2_add(vec2_scale(p0, one_minus_t * one_minus_t), vec2_scale(p1, 2.f * one_minus_t * t)), vec2_scale(p2, t * t));
}

static inline vec2 vec2_abs(vec2 a)
{
    return (vec2) {.x = fabs(a.x), .y = fabs(a.y)};
}

#endif 
