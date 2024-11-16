#include "data_binheap.h"

/*************************/
/* Fonctions auxiliaires */
/*************************/

int left_binheap(int) {
    return 0;
}
int right_binheap(int) {
    return 0;
}
int parent_binheap(int) {
    return 0;
}
bool isvalid_binheap(binheap*, int) {
    return false;
}

void grow_binheap(binheap*) {
    return;
}
void shrink_binheap(binheap*) {
    return;
}


/************************/
/* Fonctions primitives */
/************************/

binheap* create_binheap(int (*) (void*, void*)) {
    return NULL;
}

void delete_binheap(binheap*) {
    return;
}

bool isempty_binheap(binheap*) {
    return false;
}

int getsize_binheap(binheap*) {
    return 0;
}

void push_binheap(binheap*, void*) {
    return;
}

void* peekmin_binheap(binheap*) {
    return NULL;
}

void* popmin_binheap(binheap*) {
    return NULL;
}
