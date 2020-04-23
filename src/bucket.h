#ifndef __BUCKET__H_
#define __BUCKET__H_

#include "particle.h"
#include "extern/collision.h"
#include "config.h"

typedef struct
{
    aabb bbox;
    int row_start;
    int row_end;
} bucket;

void init_buckets(bucket* buckets, int buckets_count, int height, int max_particles);
bool test_particle_bucket(bucket* bucket, config const* cfg, particle* particle);

#endif