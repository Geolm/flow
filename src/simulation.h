#ifndef __SIMULATION__H_
#define __SIMULATION__H_

#include "image_buffers.h"
#include "particle.h"
#include "extern/sched.h"

//-----------------------------------------------------------------------------
void init_particles(struct scheduler* sched, image_buffers const* image, int* random_seed, float life_step, particle* particles, int particles_count);
void update_particles(struct scheduler* sched, image_buffers const* image, float position_step, particle* particles, int particles_count);



#endif // __SIMULATION__H_