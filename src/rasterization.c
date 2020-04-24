#include "rasterization.h"
#include "extern/collision.h"
#include "extern/color.h"
#include "extern/clamp.h"

//-----------------------------------------------------------------------------
void clear_image(image_buffers *image, uint32_t color)
{
    for(int i=0; i<image->pixels_count; ++i)
        image->color_buffer[i] = color;
}

//-----------------------------------------------------------------------------
// MSAA 16x rasterization : check distance distance against disc for all 16 samples
// 
// top_left of the pixel (uv)
// offset_step is 1/5 of pixel (uv)
// squared_distance if the function returns below that the sample is considered opaque
// returns samples_count between 0 and 16
static int get_disc_samples_count(vec2 top_left, vec2 offset_step, int max_samples, vec2 center, float squared_distance)
{
    int samples_count = 0;
    vec2 offset = offset_step;
    for(int y=0; y<max_samples; ++y)  
    {
        offset.x = offset_step.x;
        for(int x=0; x<max_samples; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            if (vec2_sq_distance(sample_position, center) < squared_distance)
                samples_count++;
            
            offset.x += offset_step.x;
        }
        offset.y += offset_step.y;
    }
    return samples_count;
}

//-----------------------------------------------------------------------------
inline static float line_sq_distance(vec2 sample_position, vec2 a, vec2 b)
{
    vec2 pa = vec2_sub(sample_position, a);
    vec2 ba = vec2_sub(b, a);
    float h = clamp_float(vec2_dot(pa,ba)/vec2_dot(ba,ba), 0.f, 1.f);
    
    return vec2_sq_length(vec2_sub(pa, vec2_scale(ba, h)));
}

//-----------------------------------------------------------------------------
// kind of copy/paste from the previous function, template? ;)
static int get_line_samples_count(vec2 top_left, vec2 offset_step, int max_samples, vec2 a, vec2 b, float squared_width)
{
    int samples_count = 0;
    vec2 offset = offset_step;
    for(int y=0; y<max_samples; ++y)  
    {
        offset.x = offset_step.x;
        for(int x=0; x<max_samples; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            if (line_sq_distance(sample_position, a, b) < squared_width)
                samples_count++;
            offset.x += offset_step.x;
        }
        offset.y += offset_step.y;
    }
    return samples_count;
}

//-----------------------------------------------------------------------------
static int get_triangle_samples_count(vec2 top_left, vec2 offset_step, int max_samples, vec2 a, vec2 b, vec2 c)
{
    int samples_count = 0;
    vec2 offset = offset_step;
    for(int y=0; y<max_samples; ++y)  
    {
        offset.x = offset_step.x;
        for(int x=0; x<max_samples; ++x)
        {            
            vec2 sample_position = vec2_add(top_left, offset);
            if (test_point_triangle(a, b, c, sample_position))
                samples_count++;
            offset.x += offset_step.x;
        }
        offset.y += offset_step.y;
    }
    return samples_count;
}



#define COMPUTE_INTEGER_AABB \
    shape_aabb.min = vec2_mul(shape_aabb.min, image->uv_to_xy); \
    shape_aabb.max = vec2_mul(shape_aabb.max, image->uv_to_xy); \
    int column_start = clamp_int((int)floorf(shape_aabb.min.x), 0, image->width); \
    int column_end = clamp_int((int)ceilf(shape_aabb.max.x), 0, image->width); \
    int row_start = clamp_int((int)floorf(shape_aabb.min.y), bucket_row_start, bucket_row_end); \
    int row_end = clamp_int((int)floorf(shape_aabb.max.y), bucket_row_start, bucket_row_end);

//-----------------------------------------------------------------------------
void rasterize_line(image_buffers *image, vec2 p0, vec2 p1, float width, uint32_t color, int bucket_row_start, int bucket_row_end)
{
    int max_samples_squared = image->max_samples * image->max_samples;

    vec2 border = {width, width};
    aabb shape_aabb = {vec2_min(p0, p1), vec2_max(p0, p1)};
    shape_aabb.min = vec2_sub(shape_aabb.min, border);
    shape_aabb.max = vec2_add(shape_aabb.max, border);

    COMPUTE_INTEGER_AABB;
    
    vec2 top_left = {0.f, (float)row_start * image->xy_to_uv.y};
    float squared_width = width * width;

    for(int y=row_start; y<row_end; ++y)
    {
        top_left.x = (float) column_start * image->xy_to_uv.x;
        for(int x=column_start; x<column_end; ++x)
        {
            int samples_count = get_line_samples_count(top_left, image->msaa_uv, image->max_samples, p0, p1, squared_width); 
            int alpha = (samples_count << 8) / max_samples_squared;
            uint32_t* pixel = &image->color_buffer[y * image->width + x];
            if (alpha)
                *pixel = lerp_RGBA(*pixel, color, alpha);
            top_left.x += image->xy_to_uv.x;
        }
        top_left.y += image->xy_to_uv.y;
    }
}

//-----------------------------------------------------------------------------
void rasterize_disc(image_buffers *image, vec2 center, float radius, uint32_t color, int bucket_row_start, int bucket_row_end)
{
    int max_samples_squared = image->max_samples * image->max_samples;
    vec2 extent = {radius, radius};
    aabb shape_aabb = {vec2_sub(center, extent), vec2_add(center, extent)};
    
    COMPUTE_INTEGER_AABB;
    
    float squared_radius = radius * radius;

    vec2 top_left = {0.f, (float)row_start * image->xy_to_uv.y};
    for(int y=row_start; y<row_end; ++y)
    {
        top_left.x = (float) column_start * image->xy_to_uv.x;
        for(int x=column_start; x<column_end; ++x)
        {
            int samples_count = get_disc_samples_count(top_left, image->msaa_uv, image->max_samples, center, squared_radius);
            int alpha = (samples_count << 8) / max_samples_squared;
            uint32_t* pixel = &image->color_buffer[y * image->width + x];
            if (alpha)
                *pixel = lerp_RGBA(*pixel, color, alpha);
            top_left.x += image->xy_to_uv.x;
        }
        top_left.y += image->xy_to_uv.y;
    }
}

//-----------------------------------------------------------------------------
void rasterize_triangle(image_buffers *image, vec2 a, vec2 b, vec2 c, uint32_t color, int bucket_row_start, int bucket_row_end)
{
    int max_samples_squared = image->max_samples * image->max_samples;
    aabb shape_aabb = {vec2_min(a, vec2_min(b, c)), vec2_max(a, vec2_max(b, c)) };
    
    COMPUTE_INTEGER_AABB;

    vec2 top_left = {0.f, (float)row_start * image->xy_to_uv.y};
    for(int y=row_start; y<row_end; ++y)
    {
        top_left.x = (float) column_start * image->xy_to_uv.x;
        for(int x=column_start; x<column_end; ++x)
        {
            int samples_count = get_triangle_samples_count(top_left, image->msaa_uv, image->max_samples, a, b, c);
            int alpha = (samples_count << 8) / max_samples_squared;
            uint32_t* pixel = &image->color_buffer[y * image->width + x];
            if (alpha)
                *pixel = lerp_RGBA(*pixel, color, alpha);
            top_left.x += image->xy_to_uv.x;
        }
        top_left.y += image->xy_to_uv.y;
    }
}
