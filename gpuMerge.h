#ifndef GPUMERGE_H
#define GPUMERGE_H

#include <stdio.h>
#include <stdlib.h>


#define SHARED_SIZE_LIMIT 1024

__global__ void gpuSummationReduce(float *in, float *out, int n);

#endif
