#include <stdio.h>
#pragma offload_attribute(push, _Cilk_shared)
void print_hello() { 
#ifdef __MIC__
    puts("hello world! MIC!");
#else
    puts("hello world! HOST!");
#endif
}
#pragma offload_attribute(pop)
void print_hello_noshared() {
    puts("I'm a noshared function");
}

int main() {
    //Async
    _Cilk_offload print_hello();
    _Cilk_offload_to(0) print_hello();
    // This one will cause compiler error:
    // _Cilk_offload print_hello_noshared();

    print_hello();
    print_hello_noshared();
    return 0;
}
