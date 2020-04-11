#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct 
{
    int width;
    int height;
    int pixels_count;
    float x_to_u;
    float y_to_v;
    float* angle_buffer;
    uint32_t* color_buffer;
} image_buffers;

//-----------------------------------------------------------------------------
static inline void init_image_buffers(image_buffers* buffers, int width, int height)
{
    buffers->pixels_count = width * height;
    buffers->width = width;
    buffers->height = height;
    buffers->x_to_u = 1.f / ((float)width - 1);
    buffers->y_to_v = 1.f / ((float)height - 1);

    buffers->angle_buffer = (float*) malloc(buffers->pixels_count * sizeof(float));
    buffers->color_buffer = (uint32_t*) malloc(buffers->pixels_count * sizeof(uint32_t));
}

//-----------------------------------------------------------------------------
static inline void terminate_image_buffers(image_buffers* buffers)
{
    free(buffers->angle_buffer);
    free(buffers->color_buffer);
}

#endif // __BUFFERS_H__