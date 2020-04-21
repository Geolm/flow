#ifndef __RASTERIZATION_H__
#define __RASTERIZATION_H__

#include "extern/vec2.h"
#include "image_buffers.h"

//-----------------------------------------------------------------------------
void clear_image(image_buffers *image, uint32_t color);
void rasterize_line(image_buffers *image, vec2 p0, vec2 p1, float width, uint32_t color, int bucket_row_start, int bucket_row_end);
void rasterize_disc(image_buffers *image, vec2 center, float radius, uint32_t color, int bucket_row_start, int bucket_row_end);
void rasterize_triangle(image_buffers *image, vec2 a, vec2 b, vec2 c, uint32_t color, int bucket_row_start, int bucket_row_end);


#endif // __RASTERIZATION_H__
