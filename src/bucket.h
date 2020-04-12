#ifndef __BUCKET__H_
#define __BUCKET__H_

#include "particle.h"

typedef struct
{
    vec2 aabb_min;
    vec2 aabb_max;
    int row_start;
    int row_end;
    int particles_count;
    particle* particles;
} bucket;

void init_buckets(bucket** buckets, int height, int num_buckets, int max_particles);

#endif