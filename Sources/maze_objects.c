#include "maze_objects.h"
#include "sys/random.h"

void (*obj_funs[OBJ_SIZE]) (maze*) = { &obj_empty, &obj_simple };
const char* obj_names[OBJ_SIZE] = { "Pas d'objets", "Simple" };


void obj_empty(maze*) {
}

void obj_simple(maze *p_maze) {
    int cell;
    do {
      getrandom(&cell, sizeof(cell), 0);
      cell %= (p_maze->vsize * p_maze->hsize);
    }while(!valid_maze(p_maze, cell));
    p_maze->objects[cell] = EXIT;
    for(int i = 0; i < p_maze->hsize * p_maze->vsize; i++) {
        getrandom(&cell, sizeof(cell), 0);
        cell %= 50;
        if(cell == 0 && p_maze->objects[i] != EXIT) {
            p_maze->objects[i] = SMALLT;
        }
        else if(cell == 1 && p_maze->objects[i] != EXIT) {
            p_maze->objects[i] = MEDT;
        }
        else if(cell == 2 && p_maze->objects[i] != EXIT) {
            p_maze->objects[i] = LARGET;
        }
        else if(cell == 3 && p_maze->objects[i] != EXIT) {
            p_maze->objects[i] = BOMB;
        }
        else if(cell == 4 && p_maze->objects[i] != EXIT) {
            p_maze->objects[i] = POLY;
        }
    }
}

