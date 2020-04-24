#ifndef __CLAMP_H__
#define __CLAMP_H__

#include <math.h>

//-----------------------------------------------------------------------------
static inline float clamp_float(float value, float min_value, float max_value)
{
    return fmin(max_value, fmax(value, min_value));
}

//-----------------------------------------------------------------------------
static inline int clamp_int(int value, int value_min, int value_max)
{
    if (value > value_max)
        value = value_max;
    if (value < value_min)
        value = value_min;

    return value;
}

#endif