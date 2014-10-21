#include <stdio.h>
#include <stdlib.h>

#pragma offload_attribute (push, target(mic))
void print_hello() { printf("hello world\n"); }
void print_hello2() { printf("hello world 2\n"); }

static void(*func_map[2])() = {
    &print_hello,
    &print_hello2
};
#pragma offload_attribute (pop)


int main() {
    puts("On Host");
    (*func_map[0])();
    (*func_map[1])();

    int i;
    for (i = 0; i < 2; i++){
        void (*tmp)() = func_map[i];
        #pragma offload_transfer target(mic:0) in(tmp)
    }
    #pragma offload target(mic:0) nocopy(func_map)
    {
        puts("On MIC");
        (*func_map[0])();
        (*func_map[1])();
    }

    return 0;
}
