#include "maze_braiding.h"
#include "maze.h"


bool is_dead_end(maze* p_maze, const int cell) {
    char c = 0;
    const unsigned char value = p_maze->props[cell] & 31;
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
    bool tab_prio[4] = {0};
    if (valid_maze(p_maze, cell) && is_dead_end(p_maze, cell) && rand() % 100 < odds){
        for (int i = 0 ; i < 4 ; i++){
            if(has_wall_maze(p_maze, cell, i))
            {
                continue;
            }
            //on n'a pas de mur
            int n = get_adj_maze(p_maze, cell, (i + 2) % 4);
            printf("cell : %d, mur libre : %d, voisin : %d\n", cell, i, n);
            if(n != -1)
            {
                printf("mur entre %d et %d cassable\n", cell, n);
                tab[(i + 2) % 4] = true;
                if(is_dead_end(p_maze, n))
                {
                    tab_prio[(i + 2) % 4] = true;
                }
            }
            n = get_adj_maze(p_maze, cell, (i + 1) % 4);
            if(n != -1 && has_wall_maze(p_maze, n, (i + 3) % 4))
            {
                printf("mur de %d cassable vers %d", cell, (i + 1) % 4);
                tab[(i + 1) % 4] = true;
                if(is_dead_end(p_maze, n))
                {
                    tab_prio[(i + 1) % 4] = true;
                }
            }
            n = get_adj_maze(p_maze, cell, (i + 3) % 4);
            if(n != -1 && has_wall_maze(p_maze, n, (i + 1) % 4))
            {
                printf("mur de %d cassable vers %d", cell, (i + 3) % 4);
                tab[(i + 3) % 4] = true;
                if(is_dead_end(p_maze, n))
                {
                    tab_prio[(i + 3) % 4] = true;
                }
            }
        }
    }

    // On choisit un mur à enlever parmis les prioritaires
    char c = 0, cp = 0;
    for(int i = 0 ; i < 4 ; i++)
    {
        if(tab_prio[i])
        {
            cp++;
        }
        if(tab[i])
        {
            c++;
        }
    }
    if(cp > 0)
    {
        int r = rand() % 4;
        while (tab_prio[r] == false)
        {
            r = rand() % 4;
        }
        del_wall_maze(p_maze, cell, r);
        return;
    }
    if(c > 0)
    {
        int r = rand() % 4;
        while (tab[r] == false)
        {
            r = rand() % 4;
        }
        del_wall_maze(p_maze, cell, r);
    }
}

void braid_maze(maze* p_maze, const int odds) {
    for (int i = 0 ; i < p_maze->hsize * p_maze->vsize ; i++){
        printf("i = %d\n", i);
        remove_one_dead_end(p_maze, i, odds);
    }
    printf("fin\n");
}
