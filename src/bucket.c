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
        buckets[i].row_start = row_start; row_start += height_step;
        buckets[i].row_end = row_start;
    }
    
    buckets[buckets_count-1].row_end = height;
    buckets[buckets_count-1].bbox.max = (vec2) {1.f, 1.f};
}
