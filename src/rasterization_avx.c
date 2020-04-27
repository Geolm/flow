#include "rasterization_avx.h"
#include "extern/collision.h"
#include "immintrin.h"
#include "extern/clamp.h"
#include "extern/color.h"


//----------------------------------------------------------------------------------------------------------------------
static inline unsigned int popcount(unsigned int value)
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
    int row_end = clamp_int((int)ceilf(shape_aabb.max.y), bucket_row_start, bucket_row_end);
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
    __m256 center_y = _mm256_set1_ps(center.y);
    __m256 pixel_size = _mm256_set1_ps(image->xy_to_uv.x);
    __m256 top_left_y = _mm256_mul_ps(_mm256_set1_ps((float)row_start), pixel_size);
    __m256 msaa_offset = _mm256_set1_ps(image->msaa_uv.x);
    __m256 msaa_offset_x_left = _mm256_mul_ps(msaa_offset, _mm256_set_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f));
    __m256 msaa_offset_x_right = _mm256_mul_ps(msaa_offset, _mm256_set_ps(9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f));

    for(int y=row_start; y<row_end; ++y)
    {
        __m256 top_left_x = _mm256_mul_ps(_mm256_set1_ps((float)column_start), pixel_size);
        __m256 sample_x_left = _mm256_add_ps(top_left_x, msaa_offset_x_left);
        __m256 sample_x_right = _mm256_add_ps(top_left_x, msaa_offset_x_right);
                
        for(int x=column_start; x<column_end; ++x)
        {
            int alpha = 0;
            __m256 sample_y = top_left_y;

            for(int i=0; i<16; ++i)
            {
                 sample_y = _mm256_add_ps(sample_y, msaa_offset);

                // left part
                __m256 delta_x = _mm256_sub_ps(sample_x_left, center_x);
                __m256 delta_y = _mm256_sub_ps(sample_y, center_y);

                delta_x = _mm256_mul_ps(delta_x, delta_x);
                delta_y = _mm256_mul_ps(delta_y, delta_y);
                __m256 squared_distance = _mm256_add_ps(delta_x, delta_y);
                __m256 result = _mm256_cmp_ps(squared_distance, squared_radius, _CMP_LT_OS);

                alpha += popcount(_mm256_movemask_ps(result));

                // right part
                delta_x = _mm256_sub_ps(sample_x_right, center_x);
                squared_distance = _mm256_fmadd_ps(delta_x, delta_x, delta_y);
                result = _mm256_cmp_ps(squared_distance, squared_radius, _CMP_LT_OS);

                alpha += popcount(_mm256_movemask_ps(result));
            }
    
            uint32_t* pixel = &image->color_buffer[y * image->width + x];
            if (alpha)
                *pixel = lerp_RGBA(*pixel, color, alpha);
            
            sample_x_left = _mm256_add_ps(sample_x_left, pixel_size);
            sample_x_right = _mm256_add_ps(sample_x_right, pixel_size);
        }
        top_left_y = _mm256_add_ps(top_left_y, pixel_size);
    }
}

//-----------------------------------------------------------------------------
// a lot of code is copied from disc function
//-----------------------------------------------------------------------------
void rasterize_line_avx(image_buffers *image, vec2 p0, vec2 p1, float width, uint32_t color, int bucket_row_start, int bucket_row_end)
{
    vec2 border = {width, width};
    aabb shape_aabb = {vec2_min(p0, p1), vec2_max(p0, p1)};
    shape_aabb.min = vec2_sub(shape_aabb.min, border);
    shape_aabb.max = vec2_add(shape_aabb.max, border);

    COMPUTE_INTEGER_AABB;

    __m256 squared_width = _mm256_set1_ps(width * width);
    __m256 p0_x = _mm256_set1_ps(p0.x);
    __m256 p0_y = _mm256_set1_ps(p0.y);
    __m256 p1_x = _mm256_set1_ps(p1.x);
    __m256 p1_y = _mm256_set1_ps(p1.y);
    __m256 pixel_size = _mm256_set1_ps(image->xy_to_uv.x);
    __m256 top_left_y = _mm256_mul_ps(_mm256_set1_ps((float)row_start), pixel_size);
    __m256 msaa_offset = _mm256_set1_ps(image->msaa_uv.x);
    __m256 msaa_offset_x_left = _mm256_mul_ps(msaa_offset, _mm256_set_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f));
    __m256 msaa_offset_x_right = _mm256_mul_ps(msaa_offset, _mm256_set_ps(9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f));

    __m256 ba_x = _mm256_sub_ps(p1_x, p0_x);
    __m256 ba_y = _mm256_sub_ps(p1_y, p0_y);
    __m256 dot_ba = _mm256_fmadd_ps(ba_x, ba_x, _mm256_mul_ps(ba_y, ba_y));
    __m256 one = _mm256_set1_ps(1.f);
    __m256 zero = _mm256_setzero_ps();

    for(int y=row_start; y<row_end; ++y)
    {
        __m256 top_left_x = _mm256_mul_ps(_mm256_set1_ps((float)column_start), pixel_size);
        __m256 sample_x_left = _mm256_add_ps(top_left_x, msaa_offset_x_left);
        __m256 sample_x_right = _mm256_add_ps(top_left_x, msaa_offset_x_right);
                
        for(int x=column_start; x<column_end; ++x)
        {
            int alpha = 0;
            __m256 sample_y = top_left_y;

            for(int i=0; i<16; ++i)
            {
                sample_y = _mm256_add_ps(sample_y, msaa_offset);

                // left 8 samples
                __m256 pa_x = _mm256_sub_ps(sample_x_left, p0_x);
                __m256 pa_y = _mm256_sub_ps(sample_y, p0_y);
                __m256 dot_pa_ab = _mm256_fmadd_ps(ba_x, pa_x, _mm256_mul_ps(ba_y, pa_y));
                __m256 h = _mm256_div_ps(dot_pa_ab, dot_ba);
                h = _mm256_max_ps(zero, _mm256_min_ps(one, h));

                __m256 v_x = _mm256_sub_ps(pa_x, _mm256_mul_ps(ba_x, h));
                __m256 v_y = _mm256_sub_ps(pa_y, _mm256_mul_ps(ba_y, h));
                __m256 v_y_sq = _mm256_mul_ps(v_y, v_y);
                __m256 squared_distance = _mm256_fmadd_ps(v_x, v_x, v_y_sq);
                __m256 result = _mm256_cmp_ps(squared_distance, squared_width, _CMP_LT_OS);
                alpha += popcount(_mm256_movemask_ps(result));

                // right 8 samples (update only x part)
                pa_x = _mm256_sub_ps(sample_x_right, p0_x);
                dot_pa_ab = _mm256_fmadd_ps(ba_x, pa_x, _mm256_mul_ps(ba_y, pa_y));
                h = _mm256_div_ps(dot_pa_ab, dot_ba);
                h = _mm256_max_ps(zero, _mm256_min_ps(one, h));
                v_x = _mm256_sub_ps(pa_x, _mm256_mul_ps(ba_x, h));
                squared_distance = _mm256_fmadd_ps(v_x, v_x, v_y_sq);
                result = _mm256_cmp_ps(squared_distance, squared_width, _CMP_LT_OS);
                alpha += popcount(_mm256_movemask_ps(result));
            }
    
            uint32_t* pixel = &image->color_buffer[y * image->width + x];
            if (alpha)
                *pixel = lerp_RGBA(*pixel, color, alpha);
            
            sample_x_left = _mm256_add_ps(sample_x_left, pixel_size);
            sample_x_right = _mm256_add_ps(sample_x_right, pixel_size);
        }
        top_left_y = _mm256_add_ps(top_left_y, pixel_size);
    }
}
