#ifndef __SIMULATION__H_
#define __SIMULATION__H_

#include "extern/vec2.h"
#include "extern/color.h"

typedef struct
{
    vec2 position;
    color4f color_start;
    color4f color_end;
} particle;



#endif // __SIMULATION__H_