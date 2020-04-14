#ifndef __RASTERIZATION_H__
#define __RASTERIZATION_H__

#include "extern/vec2.h"
#include "image_buffers.h"

//-----------------------------------------------------------------------------
void rasterize_line(image_buffers *buffer, vec2 p0, vec2 p1, float width, int row_start, int row_end);
void rasterize_disc(image_buffers *buffer, vec2 center, float radius, int row_start, int row_end);


#endif // __RASTERIZATION_H__
