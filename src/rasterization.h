#ifndef __RASTERIZATION_H__
#define __RASTERIZATION_H__

#include "extern/vec2.h"

// MSAA 16x rasterization of a disc
//
// top_left of the pixel (uv)
// offset_step is 1/5 of pixel

float compute_pixel_opacity(vec2 top_left, vec2 offset_step, vec2 disc_center, float squared_radius);


#endif // __RASTERIZATION_H__
