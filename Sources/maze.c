#include "maze.h"

#include <bits/types/siginfo_t.h>

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
    // ReSharper disable once CppDFANullDereference
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
    for(int i = 0; i < p_maze->vsize * p_maze->hsize; i++)
    {
        if(p_maze->objects[i] == EXIT)
        {
            exits++;
        }
    }
    return exits;
}

/**************************************/
/*+ Création d'un labyrinthe basique +*/
/**************************************/


maze* create_proto_maze_nomask(const int size, const int height) {
    maze *p_maze = malloc(sizeof(maze));
    //check sécurité
    if(p_maze == NULL) {
        fprintf(stderr, "create_proto_maze_nomask: erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    p_maze->vsize = size;
    p_maze->hsize = height;
    p_maze->props = malloc(size * height * sizeof(unsigned char));
    p_maze->objects = malloc(size * height * sizeof(object));
    //check sécurité
    if(p_maze->props == NULL || p_maze->objects == NULL) {
        fprintf(stderr, "create_proto_maze_nomask: erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    p_maze->nb_minotaurs = 0;
    p_maze->minotaurs = NULL;
    p_maze->nb_reachable = size * height; //puisque les cellules sont murées, aucune n'est accessible
    for(int i = 0; i < size * height; i++) {
        p_maze->props[i] = 79; // On initialise les propriétés des cellules (toutes les cellules sont murées dans les quatre directions, non occupées et non masquées)
        p_maze->objects[i] = NONE; // On initialise les objets à NONE
    }
    const int i = rand() % (size * height); // NOLINT(*-msc50-cpp)
    p_maze->player = i; // On place le joueur dans une cellule aléatoire
    p_maze->props[i] += 16; // On marque la cellule comme occupée (on met le 5ème bit à 1)
    return p_maze;
}

maze* create_proto_maze(mask *m) {
    maze *p_maze = malloc(sizeof(maze));
    //check sécurité
    if(p_maze == NULL) {
        fprintf(stderr, "create_proto_maze: erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    p_maze->vsize = m->vsize;
    p_maze->hsize = m->hsize;
    p_maze->props = malloc(m->vsize * m->hsize * sizeof(unsigned char));
    p_maze->objects = malloc(m->vsize * m->hsize * sizeof(object));
    //check sécurité
    if(p_maze->props == NULL || p_maze->objects == NULL) {
        fprintf(stderr, "create_proto_maze: erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    p_maze->nb_minotaurs = 0;
    p_maze->minotaurs = NULL;
    p_maze->nb_reachable = 1; //puisque le joueur est placé dans une cellule, une seule cellule est accessible
    for(int i = 0; i < m->vsize * m->hsize; i++) {
        p_maze->props[i] = 15; // On initialise les murs (toutes les cellules sont murées dans les quatre directions)
        p_maze->objects[i] = NONE;
        if(m->grid[i]) {
            p_maze->props[i] += 32; // On masque la cellule (on met le 6ème bit à 1)
        }
    }
    int i = rand() % (m->vsize * m->hsize); // NOLINT(*-msc50-cpp)
    while(!valid_maze(p_maze, i)) // On placera le joueur dans une cellule aléatoire valide
    {
        i = rand() % (m->vsize * m->hsize); // NOLINT(*-msc50-cpp)
    }
    p_maze->player = i; // On place le joueur dans une cellule aléatoire
    p_maze->props[i] += 16; // On marque la cellule comme occupée (on met le 5ème bit à 1)
    return p_maze;
}

void free_maze(maze *p_maze) {
    free(p_maze->objects);
    free(p_maze->props);
    free(p_maze->minotaurs);
    free(p_maze);
}

/****************************/
/*+ Gestion des minotaures +*/
/****************************/

int has_minotaur_maze(maze *p_maze, const int cellule) {
    for(int i = 0; i < p_maze->nb_minotaurs; i++) {
        if(p_maze->minotaurs[i] == cellule) {
            return i;
        }
    }
    return -1;
}

void gen_minotaurs_maze(maze *p_maze, int nb_minotaurs) {
    //la boucle aléatoire est plutôt lourde, on pourrait l'alléger en utilisant un tableau de booléens par exemple
    if(p_maze->minotaurs)
    {
        free(p_maze->minotaurs);
    }
    if(nb_minotaurs > p_maze -> hsize * p_maze -> vsize - 1) { // Si le nombre de minotaures est supérieur au nombre de cellules - 1
        nb_minotaurs = p_maze -> hsize * p_maze -> vsize - 1; // On réduit le nombre de minotaures au nombre de cellules - 1
    }
    p_maze->nb_minotaurs = nb_minotaurs;
    p_maze->minotaurs = malloc(nb_minotaurs * sizeof(int));
    //check sécurité
    if(p_maze->minotaurs == NULL) {
        fprintf(stderr, "gen_minotaurs_maze: erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < nb_minotaurs; i++) {
        int cellule = rand() % (p_maze->vsize * p_maze->hsize); // NOLINT(*-msc50-cpp)
        while(!valid_maze(p_maze, cellule) || is_occupied_maze(p_maze, cellule)) // On place les minotaures dans des cellules aléatoires valides et non occupées
        {
            cellule = rand() % (p_maze->vsize * p_maze->hsize); // NOLINT(*-msc50-cpp)
        }
        p_maze->minotaurs[i] = cellule; // On place le minotaure dans la cellule
    }
}

/********************************/
/*+ Récupération d'information +*/
/********************************/

bool valid_move_maze(maze *p_maze, const int cellule, const move mv)
{
    if(mv == M_WAIT)
    {
        return true; // Si on est sur une cellule, c'est qu'elle est valide
    }
    const int neighbour = get_adj_maze(p_maze, cellule, (cardinal)mv);
    if(neighbour == -1) {
        return false; // Si il n'y a pas de cellule adjacente dans la direction donnée, le mouvement n'est pas valide
    }
    return !has_wall_maze(p_maze, cellule, (cardinal)mv) && valid_maze(p_maze, neighbour) && !is_occupied_maze(p_maze, neighbour);
    // Si il n'y a pas de mur, que la cellule adjacente est valide et non occupée, le mouvement est valide
}















