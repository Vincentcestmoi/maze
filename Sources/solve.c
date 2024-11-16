#include "solve.h"

/***************************************************/
/*+ Définition et manipulation d'un chemin simple **/
/***************************************************/

sim_path* sim_emptypath(int) {
    return NULL;
}

void sim_addtopath(maze*, move, sim_path*) {
    return;
}

sim_path* sim_copypath(sim_path*) {
    return NULL;
}

void sim_freepath(sim_path*) {
    return;
}

/**********************************************/
/*+ Définition et manipulation d'un parcours **/
/**********************************************/

const char* salgo_names[ALG_SIZE] = { "BFS", "DFS", "A*" };
const char* sgoal_names[GOA_SIZE] = { "Trésor", "Bombe", "Poly d'algo","Sortie" };


sim_search* sim_create_search(sim_algorithm, sim_goal) {
    return NULL;
}

void sim_free_search(sim_search*) {
    return;
}



/******************************************************************/
/*+ Les heuristiques  (uniquement utilisées pas l'algorithme A*) **/
/******************************************************************/


// Tableau des noms de stratégies (pour l'affichage)
const char* heu_names[HEU_SIZE] = { "Neutre", "Manhattan", "Eviter les minotaures" };

// Tableau des fonctions heuristiques
int(*heu_funs[HEU_SIZE]) (game*, int, dynarray*) = { &astar_none, &astar_manhattan, &astar_runaway };


int astar_none(game*, int, dynarray*) {
    return 0;
}

int astar_manhattan(game*, int, dynarray*) {
    return 0;
}

int astar_runaway(game*, int, dynarray*) {
    return 0;
}


/*********************************/
/*+ Les algorithmes de parcours **/
/*********************************/

sim_search* (*salgo_funs[ALG_SIZE]) (game*, int, sim_goal, int(heu_fun)(game*, int, dynarray*), bool) = { &sim_bfs, &sim_dfs, &sim_astar };


sim_search* sim_bfs(game*, int, sim_goal, int(*)(game*, int, dynarray*), bool) {
    return NULL;
}

sim_search* sim_dfs(game*, int, sim_goal, int(*)(game*, int, dynarray*), bool) {
    return NULL;
}

sim_search* sim_astar(game*, int, sim_goal, int(*)(game*, int, dynarray*), bool) {
    return NULL;
}
