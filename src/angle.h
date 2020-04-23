#ifndef __ANGLE__H_
#define __ANGLE__H_

#include "image_buffers.h"
#include "extern/vec2.h"
#include "config.h"

void fill_angle_buffer(image_buffers* image, config const* cfg, int row_start, int row_end);
float fetch_angle_buffer(image_buffers const* image, vec2 uv);

#endif // __ANGLE__H_
