#ifndef __FLOW_H__
#define __FLOW_H__

#include "image_buffers.h"
#include "extern/sched.h"
#include "config.h"

void generate_image(image_buffers* image, struct scheduler* sched, config* cfg);

#endif