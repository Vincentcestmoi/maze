#include "maze_objects.h"

void (*obj_funs[OBJ_SIZE]) (maze*) = { &obj_empty, &obj_simple };
const char* obj_names[OBJ_SIZE] = { "Pas d'objets", "Simple" };


void obj_empty(maze*) {
    return;
}

void obj_simple(maze*) {
    return;
}

void generate_objects_maze(maze* p_maze, objgenerator f) {
    switch(f)
    {
    case OBJ_EMPTY:
        obj_empty(p_maze);
        return;
    case OBJ_SIMPLE:
        obj_simple(p_maze);
        return;
    default:
        return;
    }
}
