#include "maze.h"

/*****************************************/
/*+ Récupération de numéros de cellules +*/
/*****************************************/

int get_adj_maze(maze* p_maze, const int cellule, const cardinal card) {
    switch (card) {
        case NORTH:
            if(cellule < p_maze->vsize) { // Si on est sur la première ligne (on ne peut donc aller plus au nord)
                return -1;
            }
            return cellule - p_maze->vsize;
        case EAST:
            if(cellule % p_maze->vsize == p_maze->vsize - 1) { // Si on est sur la dernière colonne (on ne peut donc aller plus à l'est)
                return -1;
            }
            return cellule + 1;
        case SOUTH:
            if(cellule >= p_maze->vsize * (p_maze->hsize - 1)) { // Si on est sur la dernière ligne (on ne peut donc aller plus au sud)
                return -1;
            }
            return cellule + p_maze->vsize;
        case WEST:
            if(cellule % p_maze->vsize == 0) { // Si on est sur la première colonne (on ne peut donc aller plus à l'ouest)
                return -1;
            }
            return cellule - 1;
        default:
            return -1;
    }
}

/****************************************/
/*+ Gestion des cellules du labyrinthe +*/
/****************************************/


void mask_cell_maze(maze *p_maze, const int cellule) {
    p_maze->props[cellule] |= 32; // On masque la cellule (on met le 6ème bit à 1)
}

bool valid_maze(maze *p_maze, const int cellule) {
    if(cellule < 0 || cellule / p_maze->vsize >= p_maze->hsize || cellule % p_maze->vsize >= p_maze->vsize) { // Si la cellule n'est pas dans la grille
        return false;
    }
    return !(p_maze->props[cellule] & 32); // Si la cellule est accessible (booléen stocké dans le 6ème bit)
}

bool is_reach_maze(maze *p_maze, const int cellule) {
    return p_maze->props[cellule] & 64; // Si la cellule est accessible (booléen stocké dans le 7ème bit)
}

void make_reach_maze(maze *p_maze, const int cellule) {
    p_maze->props[cellule] |= 64; // On rend la cellule accessible (on met le 7ème bit à 1)
}

bool has_wall_maze(maze *p_maze, const int cellule, const cardinal card) {
    switch (card)
    {
        case NORTH:
            return p_maze->props[cellule] & 1; // Si il y a un mur au nord (booléen stocké dans le 1er bit)
        case EAST:
            return p_maze->props[cellule] & 2; // Si il y a un mur à l'est (booléen stocké dans le 2ème bit)
        case SOUTH:
            return p_maze->props[cellule] & 4; // Si il y a un mur au sud (booléen stocké dans le 3ème bit)
        case WEST:
            return p_maze->props[cellule] & 8; // Si il y a un mur à l'ouest (booléen stocké dans le 4ème bit)
        default:
            fprintf(stderr, "has_wall_maze: cardinal inconnu\n");
            exit(EXIT_FAILURE);
    }
}

void build_wall_maze(maze *p_maze, const int cellule, const cardinal card) {
    switch (card)
    {
    case NORTH:
        p_maze->props[cellule] |= 1; // On met un mur au nord (on met le 1er bit à 1)
        int neighbour = get_adj_maze(p_maze, cellule, NORTH);
        if(neighbour != -1) {
            p_maze->props[neighbour] |= 4; // On met un mur au sud de la cellule voisine (on met le 3ème bit à 1)
        }
        return;
    case EAST:
        p_maze->props[cellule] |= 2; // On met un mur à l'est (on met le 2ème bit à 1)
        neighbour = get_adj_maze(p_maze, cellule, EAST);
        if(neighbour != -1) {
            p_maze->props[neighbour] |= 8; // On met un mur à l'ouest de la cellule voisine (on met le 4ème bit à 1)
        }
        return;
    case SOUTH:
        p_maze->props[cellule] |= 4; // On met un mur au sud (on met le 3ème bit à 1)
        neighbour = get_adj_maze(p_maze, cellule, SOUTH);
        if(neighbour != -1) {
            p_maze->props[neighbour] |= 1; // On met un mur au nord de la cellule voisine (on met le 1er bit à 1)
        }
        return;
    case WEST:
        p_maze->props[cellule] |= 8; // On met un mur à l'ouest (on met le 4ème bit à 1)
        neighbour = get_adj_maze(p_maze, cellule, WEST);
        if(neighbour != -1) {
            p_maze->props[neighbour] |= 2; // On met un mur à l'est de la cellule voisine (on met le 2ème bit à 1)
        }
        return;
    default:
        fprintf(stderr, "build_wall_maze: cardinal inconnu\n");
        exit(EXIT_FAILURE);
    }
}
void del_wall_maze(maze *p_maze, const int cellule, const cardinal card) {
    switch (card)
    {
    case NORTH:
        p_maze->props[cellule] &= 254; // On enlève le mur au nord (on met le 1er bit à 0)
        int neighbour = get_adj_maze(p_maze, cellule, NORTH);
        if(neighbour != -1) {
            p_maze->props[neighbour] &= 251; // On enlève le mur au sud de la cellule voisine (on met le 3ème bit à 0)
        }
        return;
    case EAST:
        p_maze->props[cellule] &= 253; // On enlève le mur à l'est (on met le 2ème bit à 0)
        neighbour = get_adj_maze(p_maze, cellule, EAST);
        if(neighbour != -1) {
            p_maze->props[neighbour] &= 247; // On enlève le mur à l'ouest de la cellule voisine (on met le 4ème bit à 0)
        }
        return;
    case SOUTH:
        p_maze->props[cellule] &= 251; // On enlève le mur au sud (on met le 3ème bit à 0)
        neighbour = get_adj_maze(p_maze, cellule, SOUTH);
        if(neighbour != -1) {
            p_maze->props[neighbour] &= 254; // On enlève le mur au nord de la cellule voisine (on met le 1er bit à 0)
        }
        return;
    case WEST:
        p_maze->props[cellule] &= 247; // On enlève le mur à l'ouest (on met le 4ème bit à 0)
        neighbour = get_adj_maze(p_maze, cellule, WEST);
        if(neighbour != -1) {
            p_maze->props[neighbour] &= 253; // On enlève le mur à l'est de la cellule voisine (on met le 2ème bit à 0)
        }
        return;
    default:
        fprintf(stderr, "build_wall_maze: cardinal inconnu\n");
        exit(EXIT_FAILURE);
    }
}

bool is_occupied_maze(maze *p_maze, const int cellule) {
    return p_maze->props[cellule] & 16; // Si la cellule est occupée (booléen stocké dans le 5ème bit)
}

void make_occupied_maze(maze *p_maze, const int cellule) {
    p_maze->props[cellule] |= 16; // On marque la cellule comme occupée (on met le 5ème bit à 1)
}
void free_occupied_maze(maze *p_maze, const int cellule) {
    p_maze->props[cellule] &= 239; // On libère la cellule (on met le 5ème bit à 0)
}


/************************/
/*+ Gestion des objets +*/
/************************/


object get_object_maze(maze *p_maze, const int cellule) {
    //on ne vérifie pas si la cellule est valide
    return p_maze->objects[cellule]; // On retourne l'objet contenu dans la cellule
}


void add_object_maze(maze *p_maze, const int cellule, const object obj) {
    //on ne vérifie pas si la cellule ou l'objet sont valides
    p_maze->objects[cellule] = obj; // On ajoute un objet dans la cellule
}

int get_exits_maze(maze *p_maze) {
    int exits = 0;
    //TODO : comprendre la consigne
    p_maze->props[p_maze->player] |= 64; // action au pif pour bypath le warning
    return exits;
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















