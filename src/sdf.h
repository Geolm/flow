#ifndef __SDF_H__
#define __SDF_H__

#include "extern/vec2.h"
#include "extern/clamp.h"

// based on https://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm

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