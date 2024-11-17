#include "maze_gen.h"

void (*gen_funs[GEN_SIZE]) (maze*) = { &maze_test,&random_maze_ab, &random_maze_wilson,&random_maze_hklinear ,&random_maze_hkdfs, &random_maze_hkrandom, &random_maze_prim ,&random_maze_kruskal ,&random_maze_rec };
const char* gen_names[GEN_SIZE] = { "Test (pas de génération)", "Aldous-Broder", "Wilson", "Hunt & Kill: Linéaire", "Hunt & Kill: DFS","Hunt & Kill: Aléatoire", "Prim" ,"Kruskal" , "Récursif" };

void maze_test(maze*) {
    //ne fait rien
}

void random_maze_ab(maze*) {
    return;
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

