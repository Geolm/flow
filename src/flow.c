#include "flow.h"
#include "angle.h"
#include "bucket.h"
#include "simulation.h"
#include "rasterization.h"

//-----------------------------------------------------------------------------
typedef struct 
{
    image_buffers* image;
    config* cfg;
} task_common_data;

//-----------------------------------------------------------------------------
#define TASK_HEIGHT_GRANULARITY (100)


//-----------------------------------------------------------------------------
static void fill_angle_func(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    task_common_data* data = (task_common_data*) pArg;

    fill_angle_buffer(data->image, data->cfg, p.start, p.end);
}


//-----------------------------------------------------------------------------
void generate_image(image_buffers* image, struct scheduler* sched, config* cfg)
{
    clear_image(image, cfg->clear_color);

    task_common_data common_data = {image, cfg};
    struct sched_task fill_angle_task;

    // generate angle buffer on multiple threads
    scheduler_add(sched, &fill_angle_task, fill_angle_func, &common_data, image->height, TASK_HEIGHT_GRANULARITY);

    // allocate particles buffer
    int num_particles = image->pixels_count / 4;
    particle* particles = (particle*) malloc(sizeof(particle) * num_particles);

    //scheduler_join(sched, &fill_angle_task);


    scheduler_wait(sched);


    free(particles);
}
