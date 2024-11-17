#include "maze_gen.h"

void (*gen_funs[GEN_SIZE]) (maze*) = { &maze_test,&random_maze_ab, &random_maze_wilson,&random_maze_hklinear ,&random_maze_hkdfs, &random_maze_hkrandom, &random_maze_prim ,&random_maze_kruskal ,&random_maze_rec };
const char* gen_names[GEN_SIZE] = { "Test (pas de génération)", "Aldous-Broder", "Wilson", "Hunt & Kill: Linéaire", "Hunt & Kill: DFS","Hunt & Kill: Aléatoire", "Prim" ,"Kruskal" , "Récursif" };

void maze_test(maze*) {
    //ne fait rien
}

void random_maze_ab(maze *p_maze) {
    bool visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée
    for(int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        visited[i] = false; //aucune case n'a été visitée
    }
    int visited_count = p_maze->hsize * p_maze->vsize - 1; //nombre de case à visiter (la case de départ est déjà visitée)
    int cell = rand() % (p_maze->hsize * p_maze->vsize); //case de départ aléatoire
    visited[cell] = true; //on visite la case de départ
    cardinal card = rand() % 4; //direction aléatoire
    while(visited_count > 0)
    {
        const int neigbour = get_adj_maze(p_maze, cell, card); //case voisine
        if(neigbour !=1)
        {
            if(!visited[neigbour]) //si la case voisine n'a pas été visitée, on la visite
            {
                visited[neigbour] = true;
                visited_count--;
                mask_cell_maze(p_maze, cell); //on casse le mur entre la case actuelle et la case voisine
            }
            cell = neigbour; //on se déplace
        }
        card = rand() % 4; //nouvelle direction aléatoire
    }
}

void random_maze_wilson(maze*) {
    return;
}

void random_maze_hkdfs(maze*) {
    return;
}

void random_maze_hkrandom(maze*) {
    return;
}

void random_maze_hklinear(maze*) {
    return;
}

void random_maze_prim(maze*) {
    return;
}

void random_maze_kruskal(maze*) {
    return;
}

void random_maze_rec(maze*) {
    return;
}

