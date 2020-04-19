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
void fill_angle_buffer(image_buffers* image, int row_start, int row_end)
{
    for(int y=row_start; y<row_end; ++y)
    {
        float v = (float)y * image->xy_to_uv.y;
        for(int x=0; x<image->width; ++x)
        {
            //float u = (float)x * image->xy_to_uv.x;
            image->angle_buffer[y * image->width + x] = v * angle_pi;
        }
    }
}

//-----------------------------------------------------------------------------
float fetch_angle_buffer(image_buffers const* image, vec2 uv)
{
    vec2 xy = vec2_mul(uv, image->uv_to_xy);
    float x_int, y_int;
    float x_frac, y_frac;

    x_frac = modff(xy.x, &x_int);
    y_frac = modff(xy.y, &y_int);

    int pixel_x = (int) x_int;
    int pixel_y = (int) y_int;

    // do bilinear filtering if not on the edge
    if (pixel_x < (image->width - 1) && pixel_y < (image->height - 1))
    {
        // a b
        // c d
        int address = pixel_y * image->width + pixel_x;

        float a = image->angle_buffer[address];
        float b = image->angle_buffer[address + 1];
        float c = image->angle_buffer[address + image->width];
        float d = image->angle_buffer[address + image->width + 1];

        return lerp_float(lerp_float(a, b, x_frac), lerp_float(c, d, x_frac), y_frac);
    }
    else
    {
        // otherwise point sampling
        return image->angle_buffer[pixel_y * image->width + pixel_x];
    }
}

