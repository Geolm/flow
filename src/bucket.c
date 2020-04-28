#include "bucket.h"
#include <stdlib.h>
#include <assert.h>

//-----------------------------------------------------------------------------
void init_buckets(bucket* buckets, int buckets_count, vec2 max_uv, int height, int max_particles)
{
    int height_step = height / buckets_count;
    int row_start = 0;
    float ratio = max_uv.y / (float) height;

    for(int i=0; i<buckets_count; ++i)
    {
        buckets[i].row_start = row_start;
        buckets[i].bbox.min = (vec2) {0.f, ((float) row_start) * ratio};
        
        row_start += height_step;
        
        buckets[i].row_end = row_start;
        buckets[i].bbox.max = (vec2) {1.f, ((float) row_start) * ratio};
    }
    buckets[buckets_count-1].row_end = height;
    buckets[buckets_count-1].bbox.max = max_uv;
}
