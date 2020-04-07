#ifndef __ANGLE__H_
#define __ANGLE__H_

typedef struct 
{
    float* data;
    int width;
    int height;
    float x_to_u;
    float y_to_v;
} angle_buffer;

void init_angle_buffer(angle_buffer* buffer, int width, int height);
void fill_angle_buffer(angle_buffer* buffer, int row_start, int row_end);
float fetch_angle_buffer(angle_buffer* buffer, float u, float v);
void terminate_angle_buffer(angle_buffer* buffer);

#endif // __ANGLE__H_
