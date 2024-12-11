#include "game_strategies.h"

#include <sys/random.h>

#include "game.h"


const char* str_names[STR_SIZE] = { "Immobile","Aléatoire", "Opposée", "Agressive", "BFS", "Attroupement", "Anti-jeu", "Détection" };
void(*str_funs[STR_SIZE]) (maze*, move, move*) = { &minotaurs_still, &minotaurs_random, &minotaurs_reverse, &minotaurs_closein, &minotaurs_bfs, &minotaurs_centre, &minotaurs_exit, &minotaurs_range };


void minotaurs_still(maze *m, move, move *mino_move) {
    for (int i = 0; i < m->nb_minotaurs; i++) {
        mino_move[i] = M_WAIT;
    }
}

void minotaurs_random(maze *m, move, move *mino_move) {
    move r;
    for (int i = 0; i < m->nb_minotaurs; i++) {
        getrandom(&r, sizeof(r), 0);
        mino_move[i] = r % 5;
    }
}

void minotaurs_reverse(maze *m, const move mv, move *mino_move) {
    for (int i = 0; i < m->nb_minotaurs; i++) {
        switch (mv){
            case M_NORTH:
                mino_move[i] = M_SOUTH;
                break;
            case M_EAST:
                mino_move[i] = M_WEST;
                break;
            case M_SOUTH:
                mino_move[i] = M_NORTH;
                break;
            case M_WEST:
                mino_move[i] = M_EAST;
                break;
            default: //inclue M_WAIT
                mino_move[i] = M_WAIT;
        }
    }
}

void minotaurs_closein(maze *m, move, move *mino_move) {
    const int x_player = m->player % m->hsize;
    const int y_player = m->player / m->hsize;
    for (int i = 0; i < m->nb_minotaurs; i++)
    {
        const int delta_x = x_player - (m->minotaurs[i] % m->hsize);
        const int delta_y = y_player - (m->minotaurs[i] / m->hsize);
        if (abs(delta_x) > abs(delta_y) || (abs(delta_x) == abs(delta_y) && rand() % 2 == 0))
        {
            if (delta_x > 0)
            {
                mino_move[i] = M_EAST;
            }
            else
            {
                mino_move[i] = M_WEST;
            }
        }
        else
        {
            if (delta_y > 0)
            {
                mino_move[i] = M_SOUTH;
            }
            else
            {
                mino_move[i] = M_NORTH;
            }
        }
    }
}

void minotaurs_bfs(maze *m, move, move *mino_move) {
    queue *q = create_queue();
    bool visited[m->hsize * m->vsize];
    for (int i = 0; i < m->hsize * m->vsize; i++) {
        visited[i] = false;
    }
    enqueue(m->player, q);
    visited[m->player] = true;
    while (!is_empty_queue(q))
    {
        const int cell = dequeue(q);
        for (cardinal c = NORTH; c < 4; c++)
        {
            const int neighbour = get_adj_maze(m, cell, c);
            if (valid_maze(m, neighbour) && !visited[neighbour] && !has_wall_maze(m, cell, c))
            {
                visited[neighbour] = true;
                enqueue(neighbour, q);
                for (int i = 0; i < m->nb_minotaurs; i++)
                {
                    if (m->minotaurs[i] == neighbour)
                    {
                        mino_move[i] = (move)((c + 2) % 4);
                    }
                }
            }
        }
    }
}

void minotaurs_centre(maze *m, move, move *mino_move) {
    queue *q = create_queue();
    bool visited[m->hsize * m->vsize];
    for (int i = 0; i < m->hsize * m->vsize; i++) {
        visited[i] = false;
    }
    enqueue(m->vsize /2 * m->hsize + m->hsize / 2, q);
    visited[m->player] = true;
    while (!is_empty_queue(q))
    {
        const int cell = dequeue(q);
        for (cardinal c = NORTH; c < 4; c++)
        {
            const int neighbour = get_adj_maze(m, cell, c);
            if (valid_maze(m, neighbour) && !visited[neighbour] && !has_wall_maze(m, cell, c))
            {
                visited[neighbour] = true;
                enqueue(neighbour, q);
                for  (int i = 0; i < m->nb_minotaurs; i++)
                {
                    if (m->minotaurs[i] == neighbour)
                    {
                        mino_move[i] = (move)((c + 2) % 4);
                    }
                }
            }
        }
    }
}

void minotaurs_exit(maze *m, move, move *mino_move) {
    queue *q = create_queue();
    bool visited[m->hsize * m->vsize];
    for (int i = 0; i < m->hsize * m->vsize; i++) {
        visited[i] = false;
    }
    for (int i = 0; i < m->vsize * m->hsize; i++) {
        if (get_object_maze(m, i) == EXIT) {
            enqueue(i, q);
            visited[i] = true;
            break;
        }
    }
    visited[m->player] = true;
    while (!is_empty_queue(q))
    {
        const int cell = dequeue(q);
        for (cardinal c = NORTH; c < 4; c++)
        {
            const int neighbour = get_adj_maze(m, cell, c);
            if (valid_maze(m, neighbour) && !visited[neighbour] && !has_wall_maze(m, cell, c))
            {
                visited[neighbour] = true;
                enqueue(neighbour, q);
                for (int i = 0; i < m->nb_minotaurs; i++)
                {
                    if (m->minotaurs[i] == neighbour)
                    {
                        mino_move[i] = (move)((c + 2) % 4);
                    }
                }
            }
        }
    }
}

void minotaurs_range(maze *m, move, move *mino_move){
    queue *q = create_queue();
    bool visited[m->hsize * m->vsize];
    for (int i = 0; i < m->hsize * m->vsize; i++) {
        visited[i] = false;
    }
    enqueue(m->player, q);
    visited[m->player] = true;
    int size_gen = 0;
    int gen = 0;
    int gen_rest = 1;
    while (!is_empty_queue(q) && gen < 10) //les minotaures sont trop bêtes pour prévoir plus de 10 cases à l'avance
    {
        const int cell = dequeue(q);
        gen_rest--;
        for (cardinal c = NORTH; c < 4; c++)
        {
            const int neighbour = get_adj_maze(m, cell, c);
            if (valid_maze(m, neighbour) && !visited[neighbour] && !has_wall_maze(m, cell, c))
            {
                visited[neighbour] = true;
                enqueue(neighbour, q);
                size_gen++;
                for (int i = 0; i < m->nb_minotaurs; i++)
                {
                    if (m->minotaurs[i] == neighbour)
                    {
                        mino_move[i] = (move)((c + 2) % 4);
                    }
                }
            }
        }
        if (gen_rest == 0)
        {
            gen++;
            gen_rest = size_gen;
            size_gen = 0;
        }
    }
}