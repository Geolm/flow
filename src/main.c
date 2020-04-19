//
//  main.c
//  Flow
//
//  Created by Geolm on 2020-02-13.
//  Copyright © 2020 Geolm. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
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

    printf("flow %d.%d\n\n", FLOW_MAJOR_VERSION, FLOW_MINOR_VERSION);
    if (argc == 1)
    {
        printf("usage : flow [-h height] [-w width] [-o output_filename]\n\n");
    }

    int output_width = 256;
    int output_height = 256;
    const char* output_filename = "output.png";
    bool run_tests = false;

    parg_init(&ps);
    while ((c = parg_getopt(&ps, argc, argv, "whot")) != -1) 
    {
		switch (c) 
        {
        case 'w' : output_width = atoi(ps.optarg); break;
        case 'h' : output_height = atoi(ps.optarg); break;
        case 'o' : output_filename = ps.optarg; break;
        case 't' : run_tests = true; break;
        }
    }
    
    printf("ouput %dx%d %s\n", output_width, output_height, output_filename);

    image_buffers image;
    init_image_buffers(&image, output_height, output_width);

    if (run_tests)
    {
        test_rasterization(&image);
    }

    terminate_image_buffers(&image);

    return 0;
}
