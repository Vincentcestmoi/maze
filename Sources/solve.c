#include "solve.h"

/***************************************************/
/*+ Définition et manipulation d'un chemin simple **/
/***************************************************/

sim_path *sim_emptypath(int start)
{
    sim_path *path = (sim_path *)malloc(sizeof(sim_path));
    if (path == NULL)
    {
        perror("Failed to allocate memory for sim_path");
        exit(EXIT_FAILURE);
    }
    path->start = start;
    path->end = start;
    path->length = 0;
    path->moves = NULL;
    return path;
}

void sim_addtopath(maze *p_maze, move direction, sim_path *path)
{
    sim_move_seq *new_move = (sim_move_seq *)malloc(sizeof(sim_move_seq));
    if (new_move == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for sim_move_seq in sim_addtopath");
        exit(EXIT_FAILURE);
    }
    new_move->direction = direction;
    new_move->next = path->moves;
    path->moves = new_move;
    path->length++;

    int adj_cell = get_adj_maze(p_maze, path->start, (cardinal)direction);
    if (adj_cell != -1)
    {
        path->start = adj_cell;
    }
}

sim_path *sim_copypath(sim_path *original)
{
    if (original == NULL)
        return NULL;

    sim_path *copy = (sim_path *)malloc(sizeof(sim_path));
    if (copy == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for sim_path in sim_copypath");
        exit(EXIT_FAILURE);
    }
    copy->start = original->start;
    copy->end = original->end;
    copy->length = original->length;
    copy->moves = NULL;

    sim_move_seq *current = original->moves;
    sim_move_seq **copy_current = &copy->moves;

    while (current != NULL)
    {
        *copy_current = (sim_move_seq *)malloc(sizeof(sim_move_seq));
        if (*copy_current == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for sim_move_seq in sim_copypath");
            exit(EXIT_FAILURE);
        }
        (*copy_current)->direction = current->direction;
        (*copy_current)->next = NULL;
        copy_current = &(*copy_current)->next;
        current = current->next;
    }

    return copy;
}

void sim_freepath(sim_path *path)
{
    if (path == NULL)
        return;

    sim_move_seq *current = path->moves;
    while (current != NULL)
    {
        sim_move_seq *next = current->next;
        free(current);
        current = next;
    }

    free(path);
}

/**********************************************/
/*+ Définition et manipulation d'un parcours **/
/**********************************************/

const char *salgo_names[ALG_SIZE] = {"BFS", "DFS", "A*"};
const char *sgoal_names[GOA_SIZE] = {"Trésor", "Bombe", "Poly d'algo", "Sortie"};


sim_search *sim_create_search(sim_algorithm algo, sim_goal goal)
{
    sim_search *search = (sim_search *)malloc(sizeof(sim_search));
    if (search == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for sim_search in sim_create_search");
        exit(EXIT_FAILURE);
    }
    search->algo = algo;
    search->goal = goal;
    search->search = create_dyn();
    search->path = NULL;
    return search;
}

void sim_free_search(sim_search *search)
{
    if (search == NULL)
        return;

    free_dyn(search->search);
    sim_freepath(search->path);
    free(search);
}


/******************************************************************/
/*+ Les heuristiques  (uniquement utilisées pas l'algorithme A*) **/
/******************************************************************/


// Tableau des noms de stratégies (pour l'affichage)
const char *heu_names[HEU_SIZE] = {"Neutre", "Manhattan", "Eviter les minotaures"};

// Tableau des fonctions heuristiques
int (*heu_funs[HEU_SIZE])(game *, int, dynarray *) = {&astar_none, &astar_manhattan, &astar_runaway};


int astar_none(game *, int, dynarray *) { return 0; }

int astar_manhattan(game *, int, dynarray *) { return 0; }

int astar_runaway(game *, int, dynarray *) { return 0; }


/*********************************/
/*+ Les algorithmes de parcours **/
/*********************************/

sim_search *(*salgo_funs[ALG_SIZE])(game *, int, sim_goal, int(heu_fun)(game *, int, dynarray *), bool) = {&sim_bfs, &sim_dfs, &sim_astar};


sim_search *sim_bfs(game *, int, sim_goal, int (*)(game *, int, dynarray *), bool) { return NULL; }

sim_search *sim_dfs(game *, int, sim_goal, int (*)(game *, int, dynarray *), bool) { return NULL; }

sim_search *sim_astar(game *, int, sim_goal, int (*)(game *, int, dynarray *), bool) { return NULL; }
