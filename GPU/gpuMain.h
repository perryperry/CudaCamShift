#ifndef GPUMAIN_H
#define GPUMAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <math.h>


int  gpuMain(int blockWidth, int numElementsInput, char p);

float * fillArray(int n, int upbound);

void printArray(int *arr, int n);

long cpuReduce(int *h_in, int n);

void usage();


#endif

