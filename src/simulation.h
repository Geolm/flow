#ifndef __SIMULATION__H_
#define __SIMULATION__H_

#include "image_buffers.h"
#include "particle.h"

//-----------------------------------------------------------------------------
void init_particles(int* random_seed, float life_step, particle* particles, int range_min, int range_max);
void simulate_particles(image_buffers const* images, float position_step, particle* particles, int range_min, int range_max);


#endif // __SIMULATION__H_