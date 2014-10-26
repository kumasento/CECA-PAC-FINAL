
#include <stdio.h>

#pragma offload_attribute(push, _Cilk_shared)
void print_scalar_param(int a) { 
#ifdef __MIC__
    printf("ON MIC: %d\n", a);
    fflush(0); // make sure of THIS!
    a = -1;
#else
    printf("ON HOST: %d\n", a);
#endif
}
void print_pointer_param(int _Cilk_shared*p) {
#ifdef __MIC__
    printf("Pointer ON MIC: %d\n", *p);
    fflush(0);
    p[0] = -1;
#else 
    printf("Pointer ON HOST: %d\n", *p);
    p[0] = -2;
#endif
}
#pragma offload_attribute(pop)

// not a local variable
_Cilk_shared int shared_a = 2;
int _Cilk_shared * p;

int main() {
    int noshared_a = 1;
    _Cilk_offload_to(0) print_scalar_param(noshared_a); // noshared_a could be passed to MIC
    _Cilk_offload_to(0) print_scalar_param(shared_a); // shared_a will be passed to MIC, too
    print_scalar_param(shared_a);

    p = (int _Cilk_shared*) 
        _Offload_shared_malloc(sizeof(int _Cilk_shared));

    p[0] = 0; // Init on HOST;
    _Cilk_offload_to(0) print_pointer_param(p);     // should be 0
    print_pointer_param(p);                         // should be -1
    _Cilk_offload_to(0) print_pointer_param(p);     // should be -2
    printf("Finally: %d\n", p[0]);                  // should be -1
    
    return 0;
}   
