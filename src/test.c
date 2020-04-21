#include <assert.h>
#include <stdio.h>
#include "extern/stb_image_write.h"
#include "extern/palette.h"
#include "test.h"
#include "rasterization.h"
#include "simulation.h"
#include "bucket.h"

//-----------------------------------------------------------------------------
void test_clear(image_buffers* image)
{
    clear_image(image, na16_light_grey);

    int result = stbi_write_png("test_clear.png", image->width, image->height, 4, image->color_buffer, sizeof(uint32_t) * image->width);

    assert(result != 0);
    assert(image->color_buffer[237] == na16_light_grey);
    
    printf(".");
}

//-----------------------------------------------------------------------------
void test_rasterization(image_buffers* image)
{
    rasterize_disc(image, (vec2){0.5f, 0.25f}, 0.25f, na16_pink, 0, image->height);
    rasterize_line(image, (vec2) {0.1f, 0.1f}, (vec2) {0.3f, 0.3f}, 0.05f, na16_orange, 0, image->height);

    int result = stbi_write_png("test_rasterization.png", image->width, image->height, 4, image->color_buffer, sizeof(uint32_t) * image->width);

    assert(result != 0);
    
    printf(".");
}

//-----------------------------------------------------------------------------
#define NUM_BUCKETS (8)
void test_buckets(image_buffers* image)
{
    bucket buckets[NUM_BUCKETS];

    init_buckets(buckets, NUM_BUCKETS, image->height, 1000);

    for(int i=0; i<NUM_BUCKETS; ++i)
    {
        bucket* b = &buckets[i];

        rasterize_line(image, (vec2) {1.f, 0.f}, (vec2) {0.1f, 0.5f}, 0.001f, na16_dark_green, b->row_start, b->row_end);
    }

    terminate_buckets(buckets, NUM_BUCKETS);

    int result = stbi_write_png("test_buckets.png", image->width, image->height, 4, image->color_buffer, sizeof(uint32_t) * image->width);

    assert(result != 0);
    
    printf(".");
}

//-----------------------------------------------------------------------------
typedef struct 
{
    image_buffers* image;
    bucket* image_bucket;
} rasterization_task_data;

//-----------------------------------------------------------------------------
void rasterization_task(void *pArg, struct scheduler *s, struct sched_task_partition p, sched_uint thread_num)
{
    rasterization_task_data* data = (rasterization_task_data*) pArg;
    image_buffers* image = data->image;
    bucket* b = data->image_bucket;
    rasterize_triangle(image, (vec2) {0.f, 0.25f}, (vec2) {0.25f, 0.0f}, (vec2) {1.f, 0.5f}, na16_light_green, b->row_start, b->row_end);
}

//-----------------------------------------------------------------------------
void test_multithread(image_buffers* image, struct scheduler* sched)
{
    bucket buckets[NUM_BUCKETS];
    struct sched_task tasks[NUM_BUCKETS];
    rasterization_task_data data[NUM_BUCKETS];

    init_buckets(buckets, NUM_BUCKETS, image->height, 1000);

    for(int i=0; i<NUM_BUCKETS; ++i)
    {
        data[i].image = image;
        data[i].image_bucket = &buckets[i];

        scheduler_add(sched, &tasks[i], rasterization_task, &data[i], 1, 1);
    }

    scheduler_wait(sched);

    int result = stbi_write_png("test_multithread.png", image->width, image->height, 4, image->color_buffer, sizeof(uint32_t) * image->width);

    assert(result != 0);

    printf(".");
}
