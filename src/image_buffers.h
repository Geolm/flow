#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#include <stdint.h>
#include <stdlib.h>
#include "extern/vec2.h"

typedef struct 
{
    int width;
    int height;
    int pixels_count;
    vec2 xy_to_uv;
    vec2 uv_to_xy;
    vec2 size;
    vec2 max_uv;
    vec2 msaa_uv;
    float width_float;
    float height_float;
    float* angle_buffer;
    uint32_t* color_buffer;
} image_buffers;

//-----------------------------------------------------------------------------
static inline void init_image_buffers(image_buffers* image, int width, int height)
{
    image->pixels_count = width * height;
    image->width = width;
    image->height = height;
    image->size = (vec2) {(float) width , (float) height};

    if (image->size.x > image->size.y)
        image->max_uv = (vec2) {1.f, image->size.y / image->size.x};
    else
        image->max_uv = (vec2) {image->size.x / image->size.y};

    image->xy_to_uv = vec2_div(image->max_uv, (vec2) {(float)width - 1,(float)height - 1});
    image->uv_to_xy = vec2_div((vec2) {1.f, 1.f}, image->xy_to_uv);
    image->msaa_uv = vec2_scale(image->xy_to_uv, 1.f / 5.f);
    image->angle_buffer = (float*) malloc(image->pixels_count * sizeof(float));
    image->color_buffer = (uint32_t*) malloc(image->pixels_count * sizeof(uint32_t));
}

//-----------------------------------------------------------------------------
static inline void terminate_image_buffers(image_buffers* image)
{
    free(image->angle_buffer);
    free(image->color_buffer);
}

#endif // __BUFFERS_H__