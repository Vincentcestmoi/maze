#include "maze.h"

/*****************************************/
/*+ Récupération de numéros de cellules +*/
/*****************************************/

int get_adj_maze(maze*, int, cardinal) {
    return 0;
}

/****************************************/
/*+ Gestion des cellules du labyrinthe +*/
/****************************************/


void mask_cell_maze(maze*, int) {
    return;
}

bool valid_maze(maze*, int) {
    return false;
}

bool is_reach_maze(maze*, int) {
    return false;
}

void make_reach_maze(maze*, int) {
    return;
}

bool has_wall_maze(maze*, int, cardinal) {
    return false;
}

void build_wall_maze(maze*, int, cardinal) {
    return;
}
void del_wall_maze(maze*, int, cardinal) {
    return;
}

bool is_occupied_maze(maze*, int) {
    return false;
}

void make_occupied_maze(maze*, int) {
    return;
}
void free_occupied_maze(maze*, int) {
    return;
}


/************************/
/*+ Gestion des objets +*/
/************************/


object get_object_maze(maze*, int) {
    return NONE;
}


void add_object_maze(maze*, int, object) {
    return;
}

int get_exits_maze(maze*) {
    return 0;
}

/**************************************/
/*+ Création d'un labyrinthe basique +*/
/**************************************/


maze* create_proto_maze_nomask(int, int) {
    return NULL;
}

maze* create_proto_maze(mask*) {
    return NULL;
}

void free_maze(maze*) {
    return;
}

/****************************/
/*+ Gestion des minotaures +*/
/****************************/

int has_minotaur_maze(maze*, int) {
    return -1;
}

void gen_minotaurs_maze(maze*, int) {
    return;
}

/********************************/
/*+ Récupération d'information +*/
/********************************/

bool valid_move_maze(maze*, int, move) {
    return false;
}















