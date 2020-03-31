#ifndef __RASTERIZATION_H__
#define __RASTERIZATION_H__

#include "extern/vec2.h"

typedef float (*sq_distance_func_ptr)(vec2 sample_position, void* params);

// MSAA 16x rasterization : check distance distance against primitives for all 16 samples
// 
// top_left of the pixel (uv)
// offset_step is 1/5 of pixel (uv)
// sq_distance_func function that computes the squared distance to sample position
// squared_distance if the function returns below that the sample is considered opaque
float compute_pixel_opacity(vec2 top_left, vec2 offset_step, sq_distance_func_ptr sq_distance_func, 
                            void* params, float squared_distance);

//-----------------------------------------------------------------------------
typedef struct 
{
    vec2 disc_center;
} disc_rasterization_params;

typedef struct 
{
    vec2 a;
    vec2 b;
} line_rasterization_params;

//-----------------------------------------------------------------------------
float disc_sq_distance(vec2 sample_position, void* params);
float line_sq_distance(vec2 sample_position, void* params);

#endif // __RASTERIZATION_H__
