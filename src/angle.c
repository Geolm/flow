#include "angle.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#define angle_pi (float) M_PI

//-----------------------------------------------------------------------------
void init_angle_buffer(angle_buffer* buffer, int width, int height)
{
    int pixels_count = width * height;

    buffer->data = (float*) malloc(pixels_count * sizeof(float));
    buffer->width = width;
    buffer->height = height;
    buffer->x_to_u = 1.f / ((float)width - 1);
    buffer->y_to_v = 1.f / ((float)height - 1);
}

//-----------------------------------------------------------------------------
void fill_angle_buffer(angle_buffer* buffer, int row_start, int row_end)
{
    for(int y=row_start; y<row_end; ++y)
    {
        float v = (float)y * buffer->y_to_v;
        for(int x=0; x<buffer->width; ++x)
        {
            //float u = (float)x * buffer->x_to_u;
            buffer->data[y * buffer->width + x] = v * angle_pi;
        }
    }
}

//-----------------------------------------------------------------------------
float fetch_angle_buffer(angle_buffer* buffer, float u, float v)
{
    // bilinear filtering here
    return 0.f;
}

//-----------------------------------------------------------------------------
void terminate_angle_buffer(angle_buffer* buffer)
{
    free(buffer->data);
}
