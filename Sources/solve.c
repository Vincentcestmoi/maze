#include "solve.h"
#include "data_dynarray.h"
#include "data_queue.h"
#include "maze.h"
#include "maze_2.h"

/***************************************************/
/*+ Définition et manipulation d'un chemin simple **/
/***************************************************/

sim_path *sim_emptypath(const int start)
{
    sim_path *path = malloc(sizeof(sim_path));
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

void sim_addtopath(maze *p_maze, const move direction, sim_path *path)
{
    sim_move_seq *new_move = malloc(sizeof(sim_move_seq));
    if (new_move == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for sim_move_seq in sim_addtopath");
        exit(EXIT_FAILURE);
    }
    new_move->direction = direction;
    new_move->next = path->moves;
    path->moves = new_move;
    path->length++;

    const int adj_cell = get_adj_maze(p_maze, path->start, (cardinal)direction);
    if (adj_cell != -1)
    {
        path->start = adj_cell;
    }
}

sim_path *sim_copypath(sim_path *original)
{
    if (original == NULL)
    {
        return NULL;
    }
    sim_path *copy = malloc(sizeof(sim_path));
    if (copy == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for sim_path copy\n");
        exit(EXIT_FAILURE);
    }
    copy->start = original->start;
    copy->end = original->end;
    copy->length = original->length;

    copy->moves = NULL;
    sim_move_seq *current_original = original->moves;
    sim_move_seq **current_copy_ptr = &(copy->moves);

    while (current_original != NULL)
    {
        sim_move_seq *new_move = malloc(sizeof(sim_move_seq));
        if (new_move == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for sim_move_seq in sim_copypath\n");
            sim_freepath(copy);
            exit(EXIT_FAILURE);
        }
        new_move->direction = current_original->direction;
        new_move->next = NULL;
        *current_copy_ptr = new_move;
        current_copy_ptr = &(new_move->next);
        current_original = current_original->next;
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


sim_search *sim_create_search(const sim_algorithm algo, const sim_goal goal)
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


int astar_none(game *, int, dynarray *) { return 0;}

int astar_manhattan(game *, int, dynarray *) { return 0; }

int astar_runaway(game *, int, dynarray *) { return 0; }


/*********************************/
/*+ Les algorithmes de parcours **/
/*********************************/

static sim_goal obj_to_goal(object obj) {
    switch (obj) {
        case SMALLT:
        case MEDT:
        case LARGET:
            return GOA_TREASURE;
        case BOMB:
            return GOA_BOMB;
        case POLY:
            return GOA_POLY;
        case EXIT:
            return GOA_EXIT;
        default: //inclue NONE
            return GOA_SIZE;
    }
}

sim_search *(*salgo_funs[ALG_SIZE])(game *, int, sim_goal, int(heu_fun)(game *, int, dynarray *), bool) = {&sim_bfs, &sim_dfs, &sim_astar};

sim_search *sim_bfs(game *g, int init_cell, sim_goal goal, int (*heuristique)(game *, int, dynarray *), bool mino) {
    if (heuristique){
        //warning friendly
    }

    queue *q = create_queue();

    // Initialisation d'un tableau de booléens pour marquer les cellules déjà visitées
    bool visited[g->m->hsize * g->m->vsize];
    cardinal card_tab[g->m->hsize * g->m->vsize];
    sim_search *search = sim_create_search(ALG_BFS, goal);
    dynarray *search_order = create_dyn();
    for (int i = 0; i < g->m->hsize * g->m->vsize; i++){
        visited[i] = false;
        card_tab[i] = NORTH;
    }
    enqueue(init_cell, q);
    visited[init_cell] = true;
    while(!is_empty_queue(q)){
        int cell = dequeue(q);
        push_dyn(cell, search_order);
        if(obj_to_goal(get_object_maze(g->m, cell)) == goal){
            sim_path *path = sim_emptypath(cell);
            search->path = path;
            while(cell != init_cell){
                fflush(stdout);
                sim_addtopath(g->m, (move)card_tab[cell], path);
                cell = get_adj_maze(g->m, cell, (card_tab[cell] + 2) % 4);
            }
            search->search = search_order;
            delete_queue(q);
            return search;
        }
        for (cardinal card = NORTH; card <= WEST; card++){
            int adj = get_adj_maze(g->m, cell, card);
            if(!has_wall_maze(g->m, cell, card) && can_be_used(g->m, adj) && !visited[adj]){
                bool valid = true;
                if(mino){
                    for(int i = 0; i < g->m->nb_minotaurs; i++){
                        if(g->minotaurs_alive[i] && g->m->minotaurs[i] == adj){
                            //un minotaure bloque la route
                            valid = false;
                            break;
                        }
                    }
                }
                if(valid){
                    enqueue(adj, q);
                    visited[adj] = true;
                    card_tab[adj] = card;
                }
            }
        }
    }
    search->search = search_order;
    delete_queue(q);
    return search;
}

sim_search *sim_dfs(game *g, int init_cell, sim_goal goal, int (*heuristique)(game *, int, dynarray *), bool mino) {
    if (heuristique)
    {
        // warning friendly
    }

    dynarray *d = create_dyn();

    // Initialisation d'un tableau de booléens pour marquer les cellules déjà visitées
    bool visited[g->m->hsize * g->m->vsize];
    cardinal card_tab[g->m->hsize * g->m->vsize];
    sim_search *search = sim_create_search(ALG_DFS, goal);
    dynarray *search_order = create_dyn();
    for (int i = 0; i < g->m->hsize * g->m->vsize; i++)
    {
        visited[i] = false;
        card_tab[i] = NORTH;
    }
    push_dyn(init_cell, d);
    visited[init_cell] = true;
    while (!is_empty_dyn(d))
    {
        int cell = pop_dyn(d);
        push_dyn(cell, search_order);
        if (obj_to_goal(get_object_maze(g->m, cell)) == goal)
        {
            sim_path *path = sim_emptypath(cell);
            search->path = path;
            while (cell != init_cell)
            {
                fflush(stdout);
                sim_addtopath(g->m, (move)card_tab[cell], path);
                cell = get_adj_maze(g->m, cell, (card_tab[cell] + 2) % 4);
            }
            search->search = search_order;
            free_dyn(d);
            return search;
        }
        for (cardinal card = NORTH; card <= WEST; card++)
        {
            int adj = get_adj_maze(g->m, cell, card);
            if (!has_wall_maze(g->m, cell, card) && can_be_used(g->m, adj) && !visited[adj])
            {
                if (mino)
                {
                    for (int i = 0; i < g->m->nb_minotaurs; i++)
                    {
                        if (g->minotaurs_alive[i] && g->m->minotaurs[i] == adj)
                        {
                            // un minotaure bloque la route
                            push_dyn(adj, d);
                            visited[adj] = true;
                            card_tab[adj] = card;
                        }
                    }
                }
                else{
                    push_dyn(adj, d);
                    visited[adj] = true;
                    card_tab[adj] = card;
                }
            }
        }
    }
    search->search = search_order;
    free_dyn(d);
    return search;
}

sim_search *sim_astar(game *, int, sim_goal, int (*)(game *, int, dynarray *), bool) { return NULL; }
