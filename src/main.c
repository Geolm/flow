//
//  main.c
//  Flow
//
//  Created by Geolm on 2020-02-13.
//  Copyright © 2020 Geolm. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "extern/sched.h"
#include "parg.h"
#include "image_buffers.h"
#include "angle.h"
#include "test.h"

#define FLOW_MAJOR_VERSION (0)
#define FLOW_MINOR_VERSION (1)

//-----------------------------------------------------------------------------
int main(int argc, char* const argv[])
{
    struct parg_state ps;
    int c;

    // argument parsing
    printf("flow %d.%d\n\n", FLOW_MAJOR_VERSION, FLOW_MINOR_VERSION);
    if (argc == 1)
    {
        printf("usage : flow [-h height] [-w width] [-o output_filename] [-q quality_level 1-16]\n\n");
        return 0;
    }

    int output_width = 1600;
    int output_height = 900;
    int horizontal_samples_count = 4;
    const char* output_filename = "output.png";
    bool run_tests = false;

    parg_init(&ps);
    while ((c = parg_getopt(&ps, argc, argv, "w:sh:so:stq:s")) != -1)
    {
		switch (c) 
        {
        case 'w' : output_width = atoi(ps.optarg); break;
        case 'h' : output_height = atoi(ps.optarg); break;
        case 'o' : output_filename = ps.optarg; break;
        case 'q' : horizontal_samples_count = atoi(ps.optarg); break;
        case 't' : run_tests = true; break;
        }
    }
    
    printf("ouput %dx%d %s\n", output_width, output_height, output_filename);


    // scheduler initialization
    void *sched_memory;
    sched_size needed_memory;
    struct scheduler sched;
    scheduler_init(&sched, &needed_memory, SCHED_DEFAULT, 0);
    sched_memory = malloc(needed_memory); 
    scheduler_start(&sched, sched_memory);

    // init image buffers
    image_buffers image;
    init_image_buffers(&image, output_width, output_height, horizontal_samples_count);


    if (run_tests)
    {
        printf("running tests ");
        test_clear(&image);
        test_rasterization(&image);
        test_buckets(&image);
        test_multithread(&image, &sched);
        test_simulation(&image, &sched);
        printf(" done\n");
    }

    // terminate
    terminate_image_buffers(&image);
    scheduler_stop(&sched, 1);
    free(sched_memory);
    
    return 0;
}
