#include "config.h"
#include "extern/palette.h"


//-----------------------------------------------------------------------------
float sdf_two_circles_one_segment(vec2 p)
{
    float d0 = sd_circle(p, (vec2) {0.2f, 0.2f}, 0.05f);
    float d1 = sd_circle(p, (vec2) {0.8f, 0.4f}, 0.2f);
    float d2 = sd_segment(p, (vec2) {0.3f, 0.8f}, (vec2) {1.0f, 0.0f});
    return smooth_min_cubic(d0, smooth_min_cubic(d1, d2, 0.1f), 0.1f);
}

//-----------------------------------------------------------------------------
float sdf_cross(vec2 p)
{
    float d0 = sd_segment(p, (vec2) {0.4f, 0.2f}, (vec2) {1.f, 1.f});
    float d1 = sd_segment(p, (vec2) {0.0f, 0.8f}, (vec2) {1.0f, 0.0f});
    float d2 = sd_circle(p, (vec2) {0.0f, 0.4f}, 0.2f);
    return smooth_min_cubic(d0, smooth_min_cubic(d1, d2, 0.1f), 0.1f);
}


//-----------------------------------------------------------------------------
void setup_config(config* cfg, int config_value, int random_seed)
{
    switch(config_value)
    {
    // lines
    case 0 : 
        {
            cfg->shape = SHAPE_LINE;
            cfg->line_width = 0.0003f;
            cfg->num_steps = 1000;
            cfg->num_particles = 1000;
            cfg->position_step = 0.001f;
            cfg->starting_position = STARTING_POS_DISC;

            cfg->random_seed = random_seed;
            cfg->angle_quantization = 0.f;
            cfg->sdf_angle_scale = 1.0f;
            cfg->perlin_noise_scale = 0.0f;
            cfg->random_angle_scale = 0.0f;

            cfg->clear_color = 0xffffffff;
            cfg->start_color = 0xff000000;
            cfg->end_color = 0xffffffff;

            cfg->sdf_func = sdf_cross;
        }
        break;
    // disc
    case 1 :
        {
            cfg->shape = SHAPE_DISC;
            cfg->line_width = 0.0f;
            cfg->num_steps = 100;
            cfg->num_particles = 2000;
            cfg->position_step = 0.005f;
            cfg->starting_position = STARTING_POS_RANDOM;

            cfg->random_seed = random_seed;
            cfg->angle_quantization = 0.f;
            cfg->sdf_angle_scale = 1.0f;
            cfg->perlin_noise_scale = 0.0f;
            cfg->random_angle_scale = 0.0f;

            cfg->clear_color = 0xffffffff;
            cfg->start_color = 0xff000000;
            cfg->end_color = 0xffffffff;

            cfg->sdf_func = sdf_cross;
        }
        break;
    }
}
