#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "extern/collision.h"

typedef struct
{
    vec2 current_position;
    vec2 last_position;
    aabb bbox;
    float life;
    float life_step;
} particle;

#endif 