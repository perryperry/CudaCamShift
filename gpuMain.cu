

#include "timing.c"
#include "gpuMerge.h"

#define UPB 100
#define LEVELS 5
#define MAXDRET 102400
#define THRESH 2

int * fillArray(int n, int upbound)
{
   int i;

   int *ret = (int *)malloc(sizeof(int) * n );

   /* Intializes random number generator */
   //seeds the random number generator used by the function rand.
    srand(1);//time(NULL));

   /* generate n random numbers from 0 to unbound - 1 */
   for( i = 0 ; i < n ; i++ ) {
      ret[i] = i;//rand() % upbound * 1.0f;
   }

   return ret;
}

void printArray(int *arr, int n){

   int i;

   for(i = 0; i < n; i ++)
      printf("%d ", arr[i]);

   printf("\n");
}

int cpuReduce(int *h_in, int n)
{
    int total = 0;

	int i;
    for(i = 0; i < n; i ++)
        total += h_in[i];

	//printf("Total found by  cpu == %d\n", total);

    return total;
}

void usage()
{
   printf("Usage: ./progName blockWidth numElementsInput p \n");
}


int gpuMain(int argc, const char **argv)
{
   // create a large workload so we can easily measure the
   // performance difference on CPU and GPU

   // to run this program: ./a.out blockWidth numElements p
   int shouldPrint = 0;
   if(argc < 3 || argc > 4) {
      usage();
      return 1;
   } else  if(argc == 3){
         shouldPrint = 0;
   } else if(argv[3][0]=='p'){
         shouldPrint=1;
   } else {
         usage();
         return 1;
   }

   //
   float tile_width = atoi(argv[1]);

   if ( ! tile_width )
   {
       printf("Wrong argument passed in for blockWidth!\n");
       exit(-1);
   }
   int n = atoi(argv[2]); //size of 1D input array

   if ( ! n )
   {
       printf("Wrong argument passed in for size of input array!\n");
       exit(-1);
   }

   // set up host memory
   int *h_in, *h_out, *d_in, *d_out;

   //int sizeDout[LEVELS]; //we can have at most 5 levels of kernel launch

   h_out = (int *)malloc(MAXDRET * sizeof(int));

   memset(h_out, 0, MAXDRET * sizeof(int));

   //generate input data from random generator
   h_in = fillArray(n, UPB);

   if( ! h_in || ! h_out )
   {
       printf("Error in host memory allocation!\n");
       exit(-1);
   }

   float num_block = ceil(n / (float)tile_width);
   dim3 block(tile_width, 1, 1);
   dim3 grid(num_block, 1, 1);

   // allocate storage for the device
   cudaMalloc((void**)&d_in, sizeof(int) * n);
   cudaMalloc((void**)&d_out, sizeof(int) * MAXDRET);
   cudaMemset(d_out, 0, sizeof(int) * MAXDRET);

   // copy input to the device
   cudaMemcpy(d_in, h_in, sizeof(int) * n, cudaMemcpyHostToDevice);

   // time the kernel launches using CUDA events
   cudaEvent_t launch_begin, launch_end;
   cudaEventCreate(&launch_begin);
   cudaEventCreate(&launch_end);

   printf("The input array is:\n");
   //print out original array
   if(shouldPrint)
       printArray(h_in, n);


   //----------------------time many kernel launches and take the average time--------------------
   
   float average_simple_time = 0;
   int num_in = n, num_out = ceil((float)n / tile_width);
   int launch = 1;

   printf("Timing simple GPU implementation… \n");
   
       // record a CUDA event immediately before and after the kernel launch
       cudaEventRecord(launch_begin,0);
    



       while( 1 )
       {
           if(launch % 2 == 1) // odd launch
               gpuSummationReduce<<<grid, block, tile_width * sizeof(float)>>>(d_in, d_out, num_in);
           else
               gpuSummationReduce<<< grid, block, tile_width * sizeof(float) >>>(d_out, d_in, num_in);

           cudaDeviceSynchronize();

           // if the number of local max returned by kernel is greater than the threshold,
           // we do reduction on GPU for these returned local maxes for another pass,
           // until, num_out < threshold
           if(num_out >= THRESH)
           {
               num_in = num_out;
               num_out = ceil((float)num_out / tile_width);
               grid.x = num_out; //change the grid dimension in x direction
               //cudaMemset(d_in, 0, n * sizeof(int));//reset d_in, used for output of next iteration
           }
           else
           {
               //copy the ouput of last lauch back to host,
               if(launch % 2 == 1)
                  cudaMemcpy(h_out, d_out, sizeof(int) * num_out, cudaMemcpyDeviceToHost);
               else
                  cudaMemcpy(h_out, d_in, sizeof(int) * num_out, cudaMemcpyDeviceToHost);

               break;
           }
           launch ++;
       }//end of while





       cudaEventRecord(launch_end,0);
       cudaEventSynchronize(launch_end);

       // measure the time spent in the kernel
       float time = 0;
       cudaEventElapsedTime(&time, launch_begin, launch_end);

       average_simple_time += time;
 
 
  printf(" done! GPU time cost in second: %f\n", average_simple_time / 1000);
  printf(" done! GPU time cost in second: %f\n", time / 1000);

  printf("The output array from device is:\n");
  //if(shouldPrint)
      printArray(h_out, num_out);


  //------------------------ now time the sequential code on CPU------------------------------

  // time many multiplication calls and take the average time
  float average_cpu_time = 0;
  clock_t now, then;

  long cpuTotal = 0;

  printf("Timing CPU implementation…\n");

 
    // timing on CPU
    then = clock();
    cpuTotal = cpuReduce(h_in, n);
    now = clock();

    // measure the time spent on CPU
   time = 0;
    time = timeCost(then, now);

    average_cpu_time += time;
 
  //average_cpu_time /= num_cpu_test;
  printf(" done. CPU time cost in second: %f\n", average_cpu_time);
  printf(" done. CPU time cost in second: %f\n", time);

  //if (shouldPrint)
      printf("CPU finding total is %ld\n", cpuTotal);

  //--------------------------------clean up-----------------------------------------------------
  cudaEventDestroy(launch_begin);
  cudaEventDestroy(launch_end);

  // deallocate device memory
  cudaFree(d_in);
  cudaFree(d_out);

  free(h_in);
  free(h_out);

  return 0;
}

