#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include "sdf.h"

#define SHAPE_LINE (0)
#define SHAPE_DISC (1)
#define SHAPE_TRIANGLE (2)

#define STARTING_POS_GRID (0)
#define STARTING_POS_RANDOM (1)
#define STARTING_POS_CIRCLEPACKING (2)


typedef struct
{
    int shape;
    int random_seed;
    int num_particles;

    float line_width;
    float position_step;
    int num_steps;
    int starting_position;

    float angle_quantization;
    float sdf_angle_scale;
    float perlin_noise_scale;
    float random_angle_scale;

    uint32_t clear_color;
    uint32_t start_color;
    uint32_t end_color;

    fetch_sdf sdf_func;
} config;

void setup_config(config* cfg, int config_value, int random_seed);

#endif
