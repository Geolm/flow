#include "simulation.h"
#include "angle.h"

//-----------------------------------------------------------------------------
void simulate_particle(image_buffers const* images, float position_step, particle* particles, int range_min, int range_max)
{
    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];
        if (p->life < 1.f)
        {
            // update position
            vec2 position = p->current_position;
            float angle = fetch_angle_buffer(images, position.x, position.y);

            p->last_position = p->current_position;
            p->current_position = vec2_add(position, vec2_scale(vec2_angle(angle), position_step));

            // update life
            p->life += p->life_step;

            if (vec2_any_less(p->current_position, (vec2) {0.f, 0.f}) || 
                vec2_any_greater(p->current_position, (vec2) {1.f, 1.f}))
            {
                // kill the particle if out of the image
                p->life = 1.f;
            }
        }
    }
}
