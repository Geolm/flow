#include "simulation.h"
#include "angle.h"
#include "extern/random.h"

//-----------------------------------------------------------------------------
void init_particles(image_buffers const* image, config const* cfg, particle* particles, int num_particles, int range_min, int range_max)
{
    int random_seed = cfg->random_seed + range_min;

    float cell_width = 0;
    float cell_height = 0;
    int grid_width = 0;
    if (cfg->starting_position == STARTING_POS_GRID)
    {
        float rsqr_num_particles = sqrtf((float) num_particles);
        cell_width = image->max_uv.x / rsqr_num_particles;
        cell_height = image->max_uv.y / rsqr_num_particles;
        grid_width = (int)rsqr_num_particles;
    }
    
    vec2 center = vec2_scale(image->max_uv, 0.5f);
    vec2 radius;
    radius.x = fminf(center.x, center.y);
    radius.y = radius.x;

    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];

        switch(cfg->starting_position)
        {
        case STARTING_POS_RANDOM :
            {
                p->current_position = (vec2) {iq_random_float(&random_seed), iq_random_float(&random_seed)};
                p->current_position = vec2_mul(p->current_position, image->max_uv);
            }
            break;
        case STARTING_POS_GRID :
            {
                float y = (float)(i / grid_width) * cell_height;
                float x = (float)(i % grid_width) * cell_width;

                p->current_position = (vec2) {x + iq_random_float(&random_seed) * cell_width, 
                                              y + iq_random_float(&random_seed) * cell_height};
            }
            break;
        case STARTING_POS_DISC :
            {
                float angle = iq_random_angle(&random_seed);
                vec2 pos = vec2_add( vec2_scale(vec2_mul(vec2_angle(angle), radius), iq_random_float(&random_seed)), center);
                p->current_position = pos;
            }
            break;
        }

        p->last_position = p->current_position;
    }
}

//-----------------------------------------------------------------------------
void update_particles(image_buffers const* image, config const* cfg, particle* particles, int range_min, int range_max)
{
    vec2 border;
    if (cfg->shape == SHAPE_LINE)
        border = (vec2) {cfg->line_width, cfg->line_width};
    else
        border = (vec2) {cfg->position_step, cfg->position_step};

    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];
        if (vec2_all_greater(p->current_position, (vec2) {0.f, 0.f}) &&
            vec2_all_less(p->current_position, image->max_uv))
        {
            // update position
            vec2 position = p->current_position;
            float angle = fetch_angle_buffer(image, position);

            p->last_position = p->current_position;
            p->current_position = vec2_add(position, vec2_scale(vec2_angle(angle), cfg->position_step));
            p->bbox.min = vec2_sub(vec2_min(p->current_position, p->last_position), border);
            p->bbox.max = vec2_add(vec2_max(p->current_position, p->last_position), border);
        }
        else
        {
            p->bbox.min = (vec2) {10.f, 10.f};
            p->bbox.max = (vec2) {11.f, 11.f};
        }
    }
}

