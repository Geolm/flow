#include "angle.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#define angle_pi (float) M_PI

// ----------------------------------------------------------------------------
static inline float lerp_float(float a, float b, float t)
{
    return (1.f - t) * a + t * b;
}

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
float fetch_angle_buffer(image_buffers const* buffer, float u, float v)
{
    float x = u * buffer->width_float;
    float y = v * buffer->height_float;
    float x_int, y_int;
    float x_frac, y_frac;

    x_frac = modff(x, &x_int);
    y_frac = modff(y, &y_int);

    int pixel_x = (int) x_int;
    int pixel_y = (int) y_int;

    // do bilinear filtering if not on the edge
    if (pixel_x < (buffer->width - 1) && pixel_y < (buffer->height - 1))
    {
        // a b
        // c d
        int address = pixel_y * buffer->width + pixel_x;

        float a = buffer->angle_buffer[address];
        float b = buffer->angle_buffer[address + 1];
        float c = buffer->angle_buffer[address + buffer->width];
        float d = buffer->angle_buffer[address + buffer->width + 1];

        return lerp_float(lerp_float(a, b, x_frac), lerp_float(c, d, x_frac), y_frac);
    }
    else
    {
        // otherwise point sampling
        return buffer->angle_buffer[pixel_y * buffer->width + pixel_x];
    }
}

