#ifndef __BUCKET__H_
#define __BUCKET__H_

#include "particle.h"
#include "extern/collision.h"
#include "extern/vec2.h"

typedef struct
{
    aabb bbox;
    int row_start;
    int row_end;
} bucket;

void init_buckets(bucket* buckets, int buckets_count, vec2 max_uv, int height, int max_particles);

#endif
