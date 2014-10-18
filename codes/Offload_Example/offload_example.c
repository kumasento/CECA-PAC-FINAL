#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <omp.h>

#define ASIZE (1024 * 1024)

double dtime() {
    double tseconds = 0.0;
    struct timeval mytime;
    gettimeofday(&mytime, (struct timezone*)0);
    tseconds = (double) (mytime.tv_sec + 
                         mytime.tv_usec * 1.0e-6);
    return tseconds;
}

int main(int argc, char *argv[]) {
    #pragma offload target(mic:0)
    printf("hello world!\n");

    int* A = (int *) malloc(sizeof(int) * ASIZE);
    int i;

    for (i = 0; i < ASIZE; i++)
        A[i] = i;

    int *R = (int *) malloc(sizeof(int) * ASIZE);

    double start = dtime();
    #pragma offload_transfer target(mic:0)\
        in(A: length(ASIZE) alloc_if(1) free_if(0))
    double end = dtime();
    printf("Source data upload: %.3f\n", end - start);
    #pragma offload target(mic:0)\
        in(A: length(0) alloc_if(0) free_if(0))\
        out(R: length(ASIZE) alloc_if(1))
    {
        int i;
        for (i = 0; i < ASIZE; i++)
            R[i] = A[i] + A[i];
    }

    puts("correctness ...");

    for (i = 0; i < ASIZE; i++)
        if (R[i] != (A[i] + A[i]))
            puts("error");


    return 0;
}
