#ifndef __RASTERIZATION_H__
#define __RASTERIZATION_H__

#include "extern/vec2.h"

// MSAA 16x rasterization of a disc
static inline float compute_pixel_opacity(vec2 top_left, float offset_step, vec2 disc_center, float squared_radius)
{
    float opacity = 0.f;
    vec offset = {offset_step, offset_step};
    for(int y=0; y<4; ++y)
    {
        offset.x = offset_step;
        for(int x=0; x<4; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            
            if (vec2_sqdistance(sample_position, disc_center) < squared_radius)
            {
                opacity += 0.0625f; // 1.f/16.f
            }            
            offset.x += offset_step;
        }
        offset.y += offset_step;
    }
    return opacity;
}

#endif // __RASTERIZATION_H__
