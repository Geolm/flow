#include "simulation.h"
#include "angle.h"
#include "extern/random.h"

//-----------------------------------------------------------------------------
void init_particles(image_buffers const* image, int* random_seed, particle* particles, int range_min, int range_max)
{
    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];

        p->current_position = (vec2) {iq_random_float(random_seed), iq_random_float(random_seed)};
        p->current_position = vec2_mul(p->current_position, image->max_uv);
        p->last_position = p->current_position;
    }
}

//-----------------------------------------------------------------------------
void update_particles(image_buffers const* image, float position_step, particle* particles, int range_min, int range_max)
{
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
            p->current_position = vec2_add(position, vec2_scale(vec2_angle(angle), position_step));
        }
    }
}

