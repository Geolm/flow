#include "rasterization.h"
#include "extern/collision.h"
#include <math.h>

//-----------------------------------------------------------------------------
static inline float clamp(float value, float min_value, float max_value)
{
    return fmin(max_value, fmax(value, min_value));
}

//-----------------------------------------------------------------------------
// MSAA 16x rasterization : check distance distance against disc for all 16 samples
// 
// top_left of the pixel (uv)
// offset_step is 1/5 of pixel (uv)
// squared_distance if the function returns below that the sample is considered opaque
float compute_disc_opacity(vec2 top_left, vec2 offset_step, vec2 center, float squared_distance)
{
    float opacity = 0.f;
    vec2 offset = offset_step;
    for(int y=0; y<4; ++y)  
    {
        offset.x = offset_step.x;
        for(int x=0; x<4; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            
            if (vec2_sq_distance(sample_position, center) < squared_distance)
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
inline static float line_sq_distance(vec2 sample_position, vec2 a, vec2 b)
{
    vec2 pa = vec2_sub(sample_position, a);
    vec2 ba = vec2_sub(b, a);
    float h = clamp(vec2_dot(pa,ba)/vec2_dot(ba,ba), 0.f, 1.f);
    
    return vec2_sq_length(vec2_sub(pa, vec2_scale(ba, h)));
}

//-----------------------------------------------------------------------------
// kind of copy/paste from the previous function, template? ;)
float compute_line_opacity(vec2 top_left, vec2 offset_step, vec2 a, vec2 b, float squared_width)
{
    float opacity = 0.f;
    vec2 offset = offset_step;
    for(int y=0; y<4; ++y)  
    {
        offset.x = offset_step.x;
        for(int x=0; x<4; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            
            if (line_sq_distance(sample_position, a, b) < squared_width)
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
static inline int int_clamp(int value, int value_min, int value_max)
{
    if (value > value_max)
        value = value_max;
    if (value < value_min)
        value = value_min;

    return value;
}

//-----------------------------------------------------------------------------
void rasterize_line(image_buffers *image, vec2 p0, vec2 p1, float width, int bucket_row_start, int bucket_row_end)
{
    // compute bounding box of the line
    aabb line_aabb = {vec2_min(p0, p1), vec2_max(p0, p1)};

    int column_start = int_clamp((int)floorf((line_aabb.min.x - width) * image->size.x), 0, image->width);
    int column_end = int_clamp((int)ceilf((line_aabb.max.x + width) * image->size.x), 0, image->width);
    int row_start = int_clamp((int)floorf((line_aabb.min.y - width) * image->size.y), bucket_row_start, bucket_row_end);
    int row_end = int_clamp((int)floorf((line_aabb.max.y + width) * image->size.y), bucket_row_start, bucket_row_end);
    
    for(int y=row_start; y<row_end; ++y)
    {
        for(int x=column_start; x<column_end; ++x)
        {

        }
    }
}