#include "rasterization.h"

//-----------------------------------------------------------------------------
static inline float clamp(float value, float min_value, float max_value)
{
    return fmin(max_value, fmax(value, min_value));
}

//-----------------------------------------------------------------------------
float compute_pixel_opacity(vec2 top_left, vec2 offset_step, sq_distance_func_ptr sq_distance_func, void* params, float squared_distance)
{
    float opacity = 0.f;
    vec2 offset = offset_step;
    for(int y=0; y<4; ++y)  
    {
        offset.x = offset_step.x;
        for(int x=0; x<4; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            
            if (sq_distance_func(sample_position, params) < squared_distance)
            {
                opacity += 0.0625f; // 1.f/16.f
            }            
            offset.x += offset_step.x;
        }
        offset.y += offset_step.y;
    }
    return opacity;
}

//-----------------------------------------------------------------------------
float disc_sq_distance(vec2 sample_position, void* params)
{
    disc_rasterization_params* disc_params = (disc_rasterization_params*) params;
    return vec2_sq_distance(disc_params->disc_center, sample_position);
}

//-----------------------------------------------------------------------------
float line_sq_distance(vec2 sample_position, void* params)
{
    line_rasterization_params* line_params = (line_rasterization_params*) params;

    vec2 pa = vec2_sub(sample_position, line_params->a);
    vec2 ba = vec2_sub(line_params->b, line_params->a);
    float h = clamp(vec2_dot(pa,ba)/vec2_dot(ba,ba), 0.f, 1.f);
    
    return vec2_sq_length(vec2_sub(pa, vec2_scale(ba, h)));
}
