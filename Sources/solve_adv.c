#include "solve_adv.h"


cmp_search* (*cmp_funs[CMP_SIZE]) (game*) = { &cmp_taketreasures };
const char* cmp_names[CMP_SIZE] = { "Récupération de tous les trésors" };


void cmp_free(cmp_search*) {
    return;
}

cmp_search* cmp_taketreasures(game*) {
    return NULL;
}
