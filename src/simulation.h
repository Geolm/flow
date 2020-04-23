#ifndef __SIMULATION__H_
#define __SIMULATION__H_

#include "image_buffers.h"
#include "particle.h"

//-----------------------------------------------------------------------------
void init_particles(image_buffers const* image, int* random_seed, particle* particles, int range_min, int range_max);
void update_particles(image_buffers const* image, float position_step, particle* particles, int range_min, int range_max);



#endif // __SIMULATION__H_