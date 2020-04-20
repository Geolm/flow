#ifndef __TEST_H__
#define __TEST_H__


#include "image_buffers.h"
#include "extern/sched.h"

void test_clear(image_buffers* image);
void test_rasterization(image_buffers* image);
void test_buckets(image_buffers* image);
void test_multithread(image_buffers* image, struct scheduler* sched);

#endif