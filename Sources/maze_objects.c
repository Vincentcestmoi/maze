#include "maze_objects.h"

void (*obj_funs[OBJ_SIZE]) (maze*) = { &obj_empty, &obj_simple };
const char* obj_names[OBJ_SIZE] = { "Pas d'objets", "Simple" };


void obj_empty(maze*) {
    return;
}

void obj_simple(maze*) {
    return;
}

