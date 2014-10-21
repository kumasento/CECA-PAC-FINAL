#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE 1024

struct offload_struct {
    int arr[ARRAYSIZE];
    int flag;
};

typedef struct offload_struct * offload_ptr;

__attribute__((target (mic:0))) offload_ptr ptr;

int main() {
    int i;
    ptr = (offload_ptr) malloc(sizeof(struct offload_struct));
    ptr->flag = -1;
    for (i = 0; i < ARRAYSIZE; i++) 
        ptr->arr[i] = i+1;

#pragma offload_transfer target(mic:0) \
    in(ptr: length(1) alloc_if(1))
    {
        int i;
        for (i = 0; i < ARRAYSIZE; i++)
            printf("%d\n", ptr->arr[i]);
    }
    
    return 0;
}
