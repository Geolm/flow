#include "angle.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define angle_pi (float) M_PI

void fill_angle_buffer(float* buffer, int width, int height, int row_start, int row_end)
{
    float x_to_u = 1.f / ((float)width - 1);
    float y_to_v = 1.f / ((float)height - 1);

    for(int y=row_start; y<row_end; ++y)
    {
        float v = (float)y * y_to_v;
        for(int x=0; x<width; ++x)
        {
            float u = (float)x * x_to_u;
            buffer[y * width + x] = v * angle_pi;
        }
    }
}
