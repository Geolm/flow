#ifndef __RASTERIZATION_H__
#define __RASTERIZATION_H__

#include "extern/vec2.h"
#include "image_buffers.h"

//-----------------------------------------------------------------------------
void rasterize_line(image_buffers *image, vec2 p0, vec2 p1, float width, int bucket_row_start, int bucket_row_end);
void rasterize_disc(image_buffers *image, vec2 center, float radius, int bucket_row_start, int bucket_row_end);


#endif // __RASTERIZATION_H__
