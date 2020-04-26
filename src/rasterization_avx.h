#ifndef __RASTERIZATION_AVX_H__
#define __RASTERIZATION_AVX_H__

#include "extern/vec2.h"
#include "image_buffers.h"

void rasterize_line_avx(image_buffers *image, vec2 p0, vec2 p1, float width, uint32_t color, int bucket_row_start, int bucket_row_end);
void rasterize_disc_avx(image_buffers *image, vec2 center, float radius, uint32_t color, int bucket_row_start, int bucket_row_end);
void rasterize_triangle_avx(image_buffers *image, vec2 a, vec2 b, vec2 c, uint32_t color, int bucket_row_start, int bucket_row_end);


#endif 