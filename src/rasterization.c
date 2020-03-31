#include "rasterization.h"

float compute_pixel_opacity(vec2 top_left, vec2 offset_step, vec2 disc_center, float squared_radius)
{
    float opacity = 0.f;
    vec2 offset = offset_step;
    for(int y=0; y<4; ++y)
    {
        offset.x = offset_step.x;
        for(int x=0; x<4; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            
            if (vec2_sq_distance(sample_position, disc_center) < squared_radius)
            {
                opacity += 0.0625f; // 1.f/16.f
            }            
            offset.x += offset_step.x;
        }
        offset.y += offset_step.y;
    }
    return opacity;
}