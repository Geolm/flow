#ifndef __ANGLE__H_
#define __ANGLE__H_

#include "image_buffers.h"

void fill_angle_buffer(image_buffers* buffer, int row_start, int row_end);
float fetch_angle_buffer(image_buffers const* buffer, float u, float v);

#endif // __ANGLE__H_
