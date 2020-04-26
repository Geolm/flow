#include "rasterization_avx.h"
#include "extern/collision.h"
#include "immintrin.h"
#include "extern/clamp.h"
#include <stdint.h>


//----------------------------------------------------------------------------------------------------------------------
static inline uint32_t popcount(uint32_t value)
{
     value = value - ((value >> 1) & 0x55555555);
     value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
     return (((value + (value >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

//----------------------------------------------------------------------------------------------------------------------
#define COMPUTE_INTEGER_AABB \
    shape_aabb.min = vec2_mul(shape_aabb.min, image->uv_to_xy); \
    shape_aabb.max = vec2_mul(shape_aabb.max, image->uv_to_xy); \
    int column_start = clamp_int((int)floorf(shape_aabb.min.x), 0, image->width); \
    int column_end = clamp_int((int)ceilf(shape_aabb.max.x), 0, image->width); \
    int row_start = clamp_int((int)floorf(shape_aabb.min.y), bucket_row_start, bucket_row_end); \
    int row_end = clamp_int((int)floorf(shape_aabb.max.y), bucket_row_start, bucket_row_end);
//----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// the avx version has a number of samples hardcoded to 256 (16x16 samples for each pixel)
//-----------------------------------------------------------------------------
void rasterize_disc_avx(image_buffers *image, vec2 center, float radius, uint32_t color, int bucket_row_start, int bucket_row_end)
{
    vec2 extent = {radius, radius};
    aabb shape_aabb = {vec2_sub(center, extent), vec2_add(center, extent)};
    
    COMPUTE_INTEGER_AABB;

    __m256 squared_radius = _mm256_set1_ps(radius * radius);
    __m256 center_x = _mm256_set1_ps(center.x);
    __m256 center_y = _mm256_set1_ps(center.x);
    __m256 pixel_size = _mm256_set1_ps(image->xy_to_uv.x);
    __m256 top_left_y = _mm256_mul_ps(_mm256_set1_ps((float)row_start), pixel_size);
    __m256 msaa_offset = _mm256_set1_ps(image->msaa_uv.x);
    __m256 msaa_offset_x_left = _mm256_mul_ps(msaa_offset, _mm256_set_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f));
    __m256 msaa_offset_x_right = _mm256_mul_ps(msaa_offset, _mm256_set_ps(9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f));

    for(int y=row_start; y<row_end; ++y)
    {
        __m256 top_left_x = _mm256_mul_ps(_mm256_set1_ps((float)column_start), pixel_size);
        __m256 left = _mm256_add_ps(top_left_x, msaa_offset_x_left);
        __m256 right = _mm256_add_ps(top_left_x, msaa_offset_x_right);
        
        for(int x=column_start; x<column_end; ++x)
        {
            int alpha = 0;

            for(int i=0; i<16; ++i)
            {
                __m256 y = _mm256_add_ps(top_left_y, msaa_offset);

                
            }
            /*int samples_count = get_disc_samples_count(top_left, image->msaa_uv, image->max_samples, center, squared_radius);
            int alpha = (samples_count << 8) / max_samples_squared;
            uint32_t* pixel = &image->color_buffer[y * image->width + x];
            if (alpha)
                *pixel = lerp_RGBA(*pixel, color, alpha);
            top_left.x += image->xy_to_uv.x;
            */

           left = _mm256_add_ps(left, pixel_size);
           right = _mm256_add_ps(right, pixel_size);
        }

        top_left_y = _mm256_add_ps(top_left_y, pixel_size);
    }
}

