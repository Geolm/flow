#ifndef __SIMULATION__H_
#define __SIMULATION__H_

#include "extern/vec2.h"
#include "image_buffers.h"

typedef struct
{
    vec2 current_position;
    vec2 last_position;
    float life;
    float life_step;
} particle;

//-----------------------------------------------------------------------------
void simulate_particle(image_buffers const* images, float position_step, particle* particles, int range_min, int range_max);


#endif // __SIMULATION__H_