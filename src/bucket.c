#include "bucket.h"
#include <stdlib.h>
#include <assert.h>

//-----------------------------------------------------------------------------
void init_buckets(bucket* buckets, int buckets_count, int height, int max_particles)
{
    float v_step = 1.f / (float) buckets_count;
    float v = 0.f;
    int height_step = height / buckets_count;
    int row_start = 0; 

    for(int i=0; i<buckets_count; ++i)
    {
        buckets[i].bbox.min = (vec2) {0.f, v}; v += v_step;
        buckets[i].bbox.max = (vec2) {1.f, v};
        buckets[i].particles_count = 0;
        buckets[i].max_particles = max_particles;
        buckets[i].particles = (particle *) malloc(max_particles * sizeof(particle));
        buckets[i].row_start = row_start; row_start += height_step;
        buckets[i].row_end = row_start;
    }
    
    buckets[buckets_count-1].row_end = height;
}

//-----------------------------------------------------------------------------
void add_particle_to_buckets(bucket* buckets, int buckets_count, particle* particle)
{

}

//-----------------------------------------------------------------------------
void terminate_buckets(bucket* buckets, int buckets_count)
{
    for(int i=0; i<buckets_count; ++i)
    {
        free(buckets[i].particles);
    }
}
