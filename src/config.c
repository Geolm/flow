#include "config.h"
#include "extern/palette.h"

//-----------------------------------------------------------------------------
void setup_config(config* cfg, int config_value, int random_seed)
{
    switch(config_value)
    {
    // lines
    case 0 : 
        {
            cfg->shape = SHAPE_LINE;
            cfg->line_width = 0.0002f;
            cfg->num_steps = 100;
            cfg->position_step = 0.005f;
            cfg->starting_position = STARTING_POS_RANDOM;

            cfg->random_seed = random_seed;
            cfg->angle_quantization = 0.f;
            cfg->base_angle_scale = 1.0f;
            cfg->perlin_noise_scale = 0.1f;
            cfg->random_angle_scale = 0.f;

            cfg->clear_color = 0xffffffff;
            cfg->start_color = 0xff000000;
            cfg->end_color = 0xff000000;
        }
        break;
    }
}
