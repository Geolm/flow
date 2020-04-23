#include "flow.h"
#include "angle.h"
#include "bucket.h"
#include "simulation.h"
#include "rasterization.h"

//-----------------------------------------------------------------------------
typedef struct 
{
    image_buffers* image;
    config const* cfg;
    particle* particles;
} task_common_data;

typedef struct 
{
    image_buffers* image;
    config const* cfg;
    bucket* bucket;
    particle* particles;
    int num_particles;
} bucket_data;


//-----------------------------------------------------------------------------
#define TASK_HEIGHT_GRANULARITY (100)
#define TASK_PARTICLE_GRANULARITY (1000)


//-----------------------------------------------------------------------------
static void fill_angle_func(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    task_common_data* data = (task_common_data*) pArg;
    fill_angle_buffer(data->image, data->cfg, p.start, p.end);
}

//-----------------------------------------------------------------------------
static void init_particles_func(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    task_common_data* data = (task_common_data*) pArg;
    init_particles(data->image, data->cfg, data->particles, p.start, p.end);
}

//-----------------------------------------------------------------------------
static void update_particles_func(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    task_common_data* data = (task_common_data*) pArg;
    update_particles(data->image, data->cfg, data->particles, p.start, p.end);
}

//-----------------------------------------------------------------------------
static void rasterize_func(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    bucket_data* data = (bucket_data*) pArg;
}

//-----------------------------------------------------------------------------
void generate_image(image_buffers* image, struct scheduler* sched, config* cfg)
{
    // allocations
    int num_particles = image->pixels_count / 4;
    particle* particles = (particle*) malloc(sizeof(particle) * num_particles);

    int num_buckets = sched->threads_num;
    bucket* buckets = (bucket*) malloc(sizeof(bucket) * num_buckets);
    bucket_data* buckets_data = (bucket_data*)malloc(sizeof(bucket_data) * num_buckets);
    struct sched_task* buckets_tasks = (struct sched_task*)malloc(sizeof(struct sched_task) * num_buckets);

    task_common_data common_data = {image, cfg, particles};
    struct sched_task fill_angle_task, init_particles_task, update_particles_task;

    // fork : angle buffer generation and particles initialization
    scheduler_add(sched, &fill_angle_task, fill_angle_func, &common_data, image->height, TASK_HEIGHT_GRANULARITY);
    scheduler_add(sched, &init_particles_task, init_particles_func, &common_data, num_particles, TASK_PARTICLE_GRANULARITY);

    clear_image(image, cfg->clear_color);
    init_buckets(buckets, num_buckets, image->height, num_particles);

    scheduler_wait(sched);

    for(int step=0; step<cfg->num_steps; ++step)
    {
        scheduler_add(sched, &update_particles_task, update_particles_func, &common_data, num_particles, TASK_PARTICLE_GRANULARITY);
        scheduler_join(sched, &update_particles_task);

        for(int i=0; i<num_buckets; ++i)
        {
            buckets_data[i].bucket = &buckets[i];
            buckets_data[i].cfg = cfg;
            buckets_data[i].image = image;
            buckets_data[i].num_particles = num_particles;
            buckets_data[i].particles = particles;

            scheduler_add(sched, &buckets_tasks[i], rasterize_func, &buckets_data[i], 1, 1);
        }
        scheduler_wait(sched);
    }

    // free memory
    free(particles);
    terminate_buckets(buckets, num_buckets);
    free(buckets);
    free(buckets_data);
    free(buckets_tasks);
}
