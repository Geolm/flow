#include "config.h"
#include "extern/palette.h"
#include <string.h>


//-----------------------------------------------------------------------------
float sdf_line(vec2 p)
{
    float d0 = sd_circle(p, (vec2) {0.0f, 0.0f}, 0.4f);
    float d1 = sd_triangle(p, (vec2) {0.6f, 0.0f}, (vec2) {0.3f, 0.3f}, (vec2) {0.8f, 0.5f});
    float d2 = sd_circle(p, (vec2) {1.0f, 0.6f}, 0.4f);
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
float sdf_null(vec2 p)
{
    return 0.f;
}

//-----------------------------------------------------------------------------
void setup_config(config* cfg, int config_value, int random_seed)
{
    memset(cfg,0, sizeof(config));
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
            cfg->starting_position = STARTING_POS_LEFT;
            cfg->starting_pos_param = 0.4f;

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
            cfg->disc_radius = 0.0025f;
            cfg->num_steps = 100;
            cfg->num_particles = 4000;
            cfg->position_step = 0.01f;
            cfg->starting_position = STARTING_POS_GRID;
            cfg->starting_pos_param = 0.2f;

            cfg->random_seed = random_seed;
            cfg->angle_quantization = 0.f;
            cfg->sdf_angle_scale = 1.0f;
            cfg->perlin_noise_scale = 0.0f;//75f;
            cfg->perlin_noise_uv_scale = (vec2) {5.f, 5.f};
            cfg->random_angle_scale = 0.0f;

            cfg->clear_color = 0xff000000;
            cfg->start_color = 0xff000000;
            cfg->end_color = na16_light_green;

            cfg->sdf_func = sdf_line;
        }
        break;
    }
}
