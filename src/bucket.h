#ifndef __BUCKET__H_
#define __BUCKET__H_

#include "particle.h"
#include "extern/collision.h"

typedef struct
{
    aabb bbox;
    int row_start;
    int row_end;
    int particles_count;
    int max_particles;
    particle* particles;
} bucket;

void init_buckets(bucket* buckets, int buckets_count, int height, int max_particles);
void add_particle_to_buckets(bucket* buckets, int buckets_count, particle* particle);
void terminate_buckets(bucket* buckets, int buckets_count);

#endif