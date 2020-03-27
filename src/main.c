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

int main(int argc, const char * argv[])
{
    struct parg_state ps;
    int c;

    if (argc == 1)
    {
        printf("flow [-h height] [-w width] [-o output_filename]");
        return -1;
    }

    int output_width = 256;
    int output_height = 256;
    const char* output_filename = "output.png";

    parg_init(&ps);
    while ((c = parg_getopt(&ps, argc, argv, "who")) != -1) 
    {
		switch (c) 
        {
        case 'w' : output_width = atoi(ps.optarg); break;
        case 'h' : output_height = atoi(ps.optarg); break;
        case 'o' : output_filename = ps.optarg; break;
        }
    }

    return 0;
}
