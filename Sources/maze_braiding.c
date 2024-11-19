#include "maze_braiding.h"
#include "maze.h"


bool is_dead_end(maze* p_maze, const int cell) {
    char c = 0;
    const unsigned char value = p_maze->props[cell] & 1111;
    if (value & 1){
        c++;
    }
    if (value & 2){
        c++;
    }
    if (value & 4){
        c++;
    }
    if (value & 8){
        c++;
    }
    return c == 3;
}

int count_dead_ends(maze* p_maze) {
    int c = 0;
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++){
        if(is_dead_end(p_maze, i)){
            c++;
        }
    }
    return c;
}

void remove_one_dead_end(maze* p_maze, const int cell, const int odds) {
    bool tab[4] = {0};
    if (valid_maze(p_maze, cell) && is_dead_end(p_maze, cell) && rand() % 100 < odds){
        for (int i = 0 ; i < 4 ; i++){
            if (has_wall_maze(p_maze, cell, i) &&
                (!has_wall_maze(p_maze, cell, (i + 2) % 4) ||
                (!has_wall_maze(p_maze, cell, (i + 1) % 4 ) && get_adj_maze(p_maze, cell, (i + 1) % 4) != -1 && has_wall_maze(p_maze, get_adj_maze(p_maze, cell, i), i))||
                (!has_wall_maze(p_maze, cell, (i + 3) % 4) && get_adj_maze(p_maze, cell, (i + 3) % 4) != -1 && has_wall_maze(p_maze, get_adj_maze(p_maze, cell, i), i)))){
                tab[i] = 1;
            }
        }
    }
    char c = 0;
    for(int i = 0; i < 4 ;i++) {
        if (tab[i]){
            c++;
        }
    }
    if (c == 0){
        return;
    }
    unsigned char card = rand() % c;
    while(!has_wall_maze(p_maze, cell, card)){
        card = (card + 1) % 4;
    }
    del_wall_maze(p_maze, cell, card);
}

void braid_maze(maze* p_maze, const int odds) {
    for (int i = 0 ; i < p_maze->hsize * p_maze->vsize ; i++){
        remove_one_dead_end(p_maze, i, odds);
    }
}
