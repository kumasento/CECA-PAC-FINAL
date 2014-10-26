#include <stdio.h>

struct label_node {
    char label;
    _Cilk_shared label_node* next;
    label_node(char x): label(x), next(NULL) {}
};
typedef _Cilk_shared label_node shared_label_node;

#pragma offload_attribute(push, _Cilk_shared)
void print_label_node(shared_label_node *head) {
#ifdef __MIC__
    puts("MIC:");
    fflush(0);
#else
    puts("HOST:");
#endif
    while (head->next != NULL) {
        printf("%c->", head->label);
        head = head->next;
    }
    printf("%c\n", head->label);
#ifdef __MIC__
    fflush(0);

    // Revise:
    head->next = (shared_label_node *) 
        _Offload_shared_malloc(sizeof(label_node));
    head->next->label = 'M';
#else
    head->next = (shared_label_node *) 
        _Offload_shared_malloc(sizeof(label_node));
    head->next->label = 'H';
#endif
}
#pragma offload_attribute(pop)

int main() {
    puts("Initializing ..");
    
    char label_list[] = {'h','e','l','l','o'};
    
    shared_label_node* head = (shared_label_node*) 
        _Offload_shared_malloc(sizeof(label_node));
    head->label = label_list[0];
    shared_label_node* p = head;
    for (int i = 1; i < 5; i++) {
        p->next = (shared_label_node*) 
            _Offload_shared_malloc(sizeof(label_node));
        p->next->label = label_list[i];
        p = p->next;
    }
    print_label_node(head);
    _Cilk_offload_to(0) print_label_node(head);
    print_label_node(head);
    _Cilk_offload_to(0) print_label_node(head);
    print_label_node(head);
    _Cilk_offload_to(0) print_label_node(head);
    // H->M->H->M->H

    return 0;
}
