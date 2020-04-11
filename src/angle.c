#include "angle.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#define angle_pi (float) M_PI

//-----------------------------------------------------------------------------
void fill_angle_buffer(image_buffers* buffer, int row_start, int row_end)
{
    for(int y=row_start; y<row_end; ++y)
    {
        float v = (float)y * buffer->y_to_v;
        for(int x=0; x<buffer->width; ++x)
        {
            //float u = (float)x * buffer->x_to_u;
            buffer->angle_buffer[y * buffer->width + x] = v * angle_pi;
        }
    }
}

//-----------------------------------------------------------------------------
float fetch_angle_buffer(image_buffers* buffer, float u, float v)
{
    // bilinear filtering here
    return 0.f;
}

