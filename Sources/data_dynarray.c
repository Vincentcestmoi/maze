#include "data_dynarray.h"

/***************************************/
/** Modification de la taille interne **/
/***************************************/

void double_dyn(dynarray *dyn) {
    dyn -> capacity *= 2;
    dyn->array = realloc(dyn->array, dyn->capacity * sizeof(dyn->array));
    if(dyn->array == NULL) {
        fprintf(stderr, "Error: realloc failed.\n");
        exit(EXIT_FAILURE);
    }
}

void divide_dyn(dynarray *dyn) {
    if(dyn -> capacity < 2) { // On ne peut pas diviser un tableau de taille 1.
        return;
    }
    dyn->capacity /= 2;
    dyn->array = realloc(dyn->array, dyn->capacity * sizeof(dyn->array));
    if(dyn->array == NULL) {
        fprintf(stderr, "Error: realloc failed.\n");
        exit(EXIT_FAILURE);
    }
}

/****************/
/** Primitives **/
/****************/

dynarray* create_dyn(void) {
    dynarray *dyn = malloc(sizeof(dynarray));
    dyn->array = malloc(sizeof(*dyn -> array));
    dyn->size = 0;
    dyn->capacity = 1;
    return dyn;
}

void free_dyn(dynarray *dyn) {
    free(dyn -> array);
    free(dyn);
}


int size_dyn(dynarray *dyn) {
    return dyn -> size;
}

void push_dyn(const int value, dynarray *dyn) {
    if(dyn -> size == dyn -> capacity) {
        double_dyn(dyn);
    }
    dyn -> array[dyn -> size] = value;
    dyn -> size++;
}

int pop_dyn(dynarray* dyn) {
    if(dyn -> size == 0) {
        fprintf(stderr, "You're trying to pop an empty dynarray, poor fool.\n");
        exit(EXIT_FAILURE);
    }
    dyn -> size--;
    if(dyn -> size < dyn -> capacity / 4) {
        divide_dyn(dyn);
    }
    return dyn -> array[dyn -> size];
}

bool is_empty_dyn(dynarray *dyn) {
    return dyn -> size == 0;
}
