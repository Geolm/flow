#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "extern/vec2.h"

typedef struct
{
    vec2 current_position;
    vec2 last_position;
    float life;
    float life_step;
} particle;

#endif 