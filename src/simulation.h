#ifndef __SIMULATION__H_
#define __SIMULATION__H_

#include "image_buffers.h"
#include "particle.h"
#include "config.h"

//-----------------------------------------------------------------------------
void init_particles(image_buffers const* image, config const* cfg, particle* particles, int num_particles, int range_min, int range_max);
void update_particles(image_buffers const* image, config const* cfg, particle* particles, int range_min, int range_max);



#endif // __SIMULATION__H_