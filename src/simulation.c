#include "simulation.h"
#include "angle.h"
#include "extern/random.h"

//-----------------------------------------------------------------------------
static void init_particles_range(int* random_seed, float life_step, particle* particles, int range_min, int range_max)
{
    float random_step_part = life_step * 0.01f;
    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];

        p->current_position.x = iq_random_float(random_seed);
        p->current_position.y = iq_random_float(random_seed);
        p->last_position = p->current_position;
        p->life = 0.f;
        p->life_step = life_step + iq_random_float(random_seed) * random_step_part;
    }
}

//-----------------------------------------------------------------------------
static void simulate_particles_range(image_buffers const* image, float position_step, particle* particles, int range_min, int range_max)
{
    for(int i=range_min; i<range_max; ++i)
    {
        particle* p = &particles[i];
        if (p->life < 1.f)
        {
            // update position
            vec2 position = p->current_position;
            float angle = fetch_angle_buffer(image, position);

            p->last_position = p->current_position;
            p->current_position = vec2_add(position, vec2_scale(vec2_angle(angle), position_step));

            // update life
            p->life += p->life_step;

            if (vec2_any_less(p->current_position, (vec2) {0.f, 0.f}) || 
                vec2_any_greater(p->current_position, image->max_uv))
            {
                // kill the particle if out of the image
                p->life = 1.f;
            }
        }
    }
}

//-----------------------------------------------------------------------------
typedef struct 
{
    image_buffers const* image;
    int* random_seed;
    float life_step;
    particle* particles;
} init_task_data;

typedef struct 
{
    image_buffers const* image;
    float position_step;
    particle* particles;
} update_task_data;

//-----------------------------------------------------------------------------
void init_particles(struct scheduler* sched, int* random_seed, float life_step, particle* particles, int particles_count)
{

}

//-----------------------------------------------------------------------------
void simulate_particles(struct scheduler* sched, image_buffers const* images, float position_step, particle* particles, int particles_count)
{

}
