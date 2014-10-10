//
//
// hellomem
//
// A simple example that measures copy  memory bandwidth on 
// Intel(r) Xeon Phi(tm) co-processors using openmp to scale
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>

// dtime - utility routine that returns the current wall clock time

double dtime()
{
    double tseconds = 0.0;
    struct timeval mytime;
    gettimeofday(&mytime,(struct timezone*)0);
    tseconds = (double)(mytime.tv_sec + mytime.tv_usec*1.0e-6);
    return( tseconds );
}

// Set to float or double
#define REAL double

#define BW_ARRAY_SIZE (1000*1000*64) 
#define BW_ITERS 1000

// number of mem ops each iteration
// 1 read  + 1 write
#define OPS_PERITER 2    

// define some arrays 
// make sure they are 64 byte aligned - for fastest cache access 
REAL fa[BW_ARRAY_SIZE] __attribute__((align(64)));
REAL fb[BW_ARRAY_SIZE] __attribute__((align(64)));
REAL fc[BW_ARRAY_SIZE] __attribute__((align(64)));


//
// Main program - array copy
// 
int main(int argc, char *argv[] ) 
{
    int i,j,k;
    double tstart, tstop, ttime;
    double gbytes = 0.0;
    REAL a=1.1;
    //
    // initialize the compute arrays 
    //

    printf("Initializing\r\n");
#pragma omp parallel for
    for(i=0; i<BW_ARRAY_SIZE; i++)
    {
        fa[i] = (REAL)i + 0.1;
        fb[i] = (REAL)i + 0.2;
        fc[i] = (REAL)i + 0.3;
    }	

// print the # of threads to be used
// Just display from 1 thread - the "master"
#pragma omp parallel
#pragma omp master 
    printf("Starting BW Test on %d threads\r\n",omp_get_num_threads());

    tstart = dtime();

    // use omp to scale the test across 
    // the threads requested. Need to set environment 
    // variables OMP_NUM_THREADS and KMP_AFFINITY
    for (i=0; i<BW_ITERS; i++)
    {
    //
    // copy the arrays to/from memory (2 bw ops)
    // use openmp to scale and get aggregate bw 
    //
#pragma omp parallel for
        for(k=0; k<BW_ARRAY_SIZE; k++)  
        {
           fa[k] = fb[k];
        }
    }

    tstop = dtime();
    // # of gigabytes we just moved  

    gbytes = (double)( 1.0e-9 * OPS_PERITER * BW_ITERS * 
                       BW_ARRAY_SIZE*sizeof(REAL));    
    // elasped time
    ttime = tstop - tstart;
    //
    // Print the results
    //
    if ((ttime) > 0.0)
    {
        printf("Gbytes = %10.3lf, Secs = %10.3lf, GBytes per sec = %10.3lf\r\n", 
                     gbytes, ttime, gbytes/ttime);
    }
    return( 0 );
}

