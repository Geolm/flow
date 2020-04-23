#include "simulation.h"
#include "angle.h"
#include "extern/random.h"

//-----------------------------------------------------------------------------
void init_particles(image_buffers const* image, config const* cfg, particle* particles, int range_min, int range_max)
{
    int random_seed = cfg->random_seed + range_min;
    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];

        p->current_position = (vec2) {iq_random_float(&random_seed), iq_random_float(&random_seed)};
        p->current_position = vec2_mul(p->current_position, image->max_uv);
        p->last_position = p->current_position;
    }
}

//-----------------------------------------------------------------------------
void update_particles(image_buffers const* image, config const* cfg, particle* particles, int range_min, int range_max)
{
    vec2 border = {cfg->line_width, cfg->line_width};

    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];
        if (vec2_all_greater(p->current_position, (vec2) {0.f, 0.f}) || 
                vec2_all_less(p->current_position, image->max_uv))
        {
            // update position
            vec2 position = p->current_position;
            float angle = fetch_angle_buffer(image, position);

            p->last_position = p->current_position;
            p->current_position = vec2_add(position, vec2_scale(vec2_angle(angle), cfg->position_step));
            p->bbox = (aabb) {vec2_sub(vec2_min(p->current_position, p->last_position), border), 
                              vec2_add(vec2_max(p->current_position, p->last_position), border)};
        }
    }
}

