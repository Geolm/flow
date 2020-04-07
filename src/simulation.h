#ifndef __SIMULATION__H_
#define __SIMULATION__H_

#include "extern/vec2.h"
#include "extern/color.h"

typedef struct
{
    vec2 current_position;
    vec2 last_position;
} particle;


void simulate_particle(particle* particle_buffer, int range_min, int range_max);

#endif // __SIMULATION__H_