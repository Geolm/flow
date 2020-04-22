#include "simulation.h"
#include "angle.h"
#include "extern/random.h"

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
static void init_task(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    init_task_data* data = (init_task_data*) pArg;

    float random_step_part = data->life_step * 0.01f;
    for(int i=p.start; i<p.end; ++i)
    {
        particle* p = &data->particles[i];

        p->current_position = (vec2) {iq_random_float(data->random_seed), iq_random_float(data->random_seed)};
        p->current_position = vec2_mul(p->current_position, data->image->max_uv);
        p->last_position = p->current_position;
        p->life = 0.f;
        p->life_step = data->life_step + iq_random_float(data->random_seed) * random_step_part;
    }
}

//-----------------------------------------------------------------------------
static void update_task(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    update_task_data* data = (update_task_data*) pArg;

    for(int i=p.start; i<p.end; ++i)
    {
        particle* p = &data->particles[i];
        if (p->life < 1.f)
        {
            // update position
            vec2 position = p->current_position;
            float angle = fetch_angle_buffer(data->image, position);

            p->last_position = p->current_position;
            p->current_position = vec2_add(position, vec2_scale(vec2_angle(angle), data->position_step));

            // update life
            p->life += p->life_step;

            if (vec2_any_less(p->current_position, (vec2) {0.f, 0.f}) || 
                vec2_any_greater(p->current_position, data->image->max_uv))
            {
                // kill the particle if out of the image
                p->life = 1.f;
            }
        }
    }
}

#define TASK_MIN_RANGE (10000)

//-----------------------------------------------------------------------------
void init_particles(struct scheduler* sched, image_buffers const* image, int* random_seed, float life_step, particle* particles, int particles_count)
{
    init_task_data data = {image, random_seed, life_step, particles};
    struct sched_task task;

    scheduler_add(sched, &task, init_task, &data, particles_count, TASK_MIN_RANGE);
    scheduler_join(sched, &task);
}

//-----------------------------------------------------------------------------
void simulate_particles(struct scheduler* sched, image_buffers const* image, float position_step, particle* particles, int particles_count)
{
    update_task_data data = {image, position_step, particles};
    struct sched_task task;

    scheduler_add(sched, &task, update_task, &data, particles_count, TASK_MIN_RANGE);
    scheduler_join(sched, &task);
}
