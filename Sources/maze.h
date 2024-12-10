/**
 * @file maze.h
 * @brief
 * Gestion des labyrinthes.
 *
 * @details
 * Contient la définition du type utilisé pour implémenter un labyrinthe et les fonctions basiques
 * qui manipulent ce type.
 *
 * @remark
 * C'est dans ce fichier que se trouvent les fonctions qui servent créér un proto-labyrinthe. Dans un
 * tel objet, toutes les cellules ont quatre murs. Les proto-labyrinthes sont le point de départ de
 * toutes les fonctions de génération qui "creusent" leurs murs (toutes les fonctions de génération
 * sont regroupées dans "maze_gen.h").
 */

#ifndef MAZE_H_
#define MAZE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "error.h"
#include "alloc.h"
#include "maze_masks.h"
#include "data_dynarray.h"


 /*   ____           _   _                   _                */
 /*  / ___| ___  ___| |_(_) ___  _ __     __| | ___  ___      */
 /* | |  _ / _ \/ __| __| |/ _ \| '_ \   / _` |/ _ \/ __|     */
 /* | |_| |  __/\__ \ |_| | (_) | | | | | (_| |  __/\__ \     */
 /*  \____|\___||___/\__|_|\___/|_| |_|  \__,_|\___||___/     */
 /* | |    __ _| |__  _   _ _ __(_)_ __ | |_| |__   ___  ___  */
 /* | |   / _` | '_ \| | | | '__| | '_ \| __| '_ \ / _ \/ __| */
 /* | |__| (_| | |_) | |_| | |  | | | | | |_| | | |  __/\__ \ */
 /* |_____\__,_|_.__/ \__, |_|  |_|_| |_|\__|_| |_|\___||___/ */
 /*                   |___/                                   */


 /***************/
 /*+ Les types +*/
 /***************/

/**
 * @brief Les quatre directions possibles dans un labyrinthe
 */
typedef enum {
    NORTH = 0, //!< Nord
    EAST, //!< Est
    SOUTH, //!< Sud
    WEST, //!< Ouest
} cardinal;


/**
 * @brief Les cinq mouvements possibles dans un labyrinthe.
 *
 * @attention Les quatre premiers mouvements sont prévus pour correspondre aux quatre
 * directions du type "cardinal" (ce sont les mêmes valeurs). Il est possible de passer
 * de l'un à l'autre selon les besoins. Cependant, il faudra toujours convertir le type
 * explicitement (dans le cas contraire, un warning est levé par le compilateur).
 */
typedef enum {
    M_NORTH = 0, //!< Nord
    M_EAST, //!< Est
    M_SOUTH, //!< Sud
    M_WEST, //!< Ouest
    M_WAIT, //!< Immobile
} move;

/**
 * @brief Les différents objets qu'on peut trouver dans un labyrinthe.
 */
typedef enum {
    NONE = 0, //!< Cellule vide (qui ne contient pas d'objet)
    SMALLT, //!< Petit trésor
    MEDT, //!< Moyen trésor
    LARGET, //!< Grand trésor
    BOMB, //!< Bombe
    POLY, //!< Poly d'algo
    EXIT, //!< Sortie
} object;

#define VALSMALL 50 //!<La valeur d'un petit trésor (pour le score).
#define VALMED 100 //!<La valeur d'un trésor moyen (pour le score).
#define VALLARGE 500 //!<La valeur d'un grand trésor (pour le score).


/**
* @brief
* Le type utilisé pour représenter un labyrinthe.
*
* @attention
* Les cellules d'un labyrinthe sont indexées linéairement ligne par ligne du Nord au Sud.
* La première cellule (dans le coin Nord-Ouest) a pour numéro 0. La dernière cellule (dans
* le coin Sud-Est) a pour numéro hsize*vsize-1.
*
* @remark
* Chaque case du tableau des propriétés code 7 Booléens dans les 7 premiers bits d'un unsigned char (1: Vrai 0: Faux)
* - bit 0: est-ce que la cellule a un mur au Nord ?
* - bit 1: est-ce que la cellule a un mur à l'Est ?
* - bit 2: est-ce que la cellule a un mur au Sud ?
* - bit 3: est-ce que la cellule a un mur à l'Ouest ?
* - bit 4: est-ce que la cellule est occupée ? (par un joueur ou un minotaure)
* - bit 5: est-ce que la cellule est masquée ? (n'appartient pas au labyrinthe)
* - bit 6: est-ce que la cellule est accessible ?
*   (des cellules peuvent être inaccessibles car séparées du joueur par des cases masquées)
*
*/
typedef struct {

    int hsize; //!< Taille horizontale
    int vsize; //!< Taille verticale

    unsigned char* props; //!< Tableau des propriétés des cellules (indexé par les cellules: la ième case donne les propriétés de la ième cellule)

    object* objects; //!< Tableau des objets (indexé par les cellules: la ième case décrit l'objet de la ième cellule)

    int player; //!< Cellule qui contient le joueur

    int nb_minotaurs; //!< Nombre de minotaures

    int* minotaurs; //!< Tableau de taille nb_minotaurs (NULL si nb_minotaurs vaut 0) qui indique les cellules contenant chaque minotaure

    int nb_reachable; //!< Nombre de cellules accessibles depuis la position initiale du joueur
} maze;

/*****************************************/
/*+ Récupération de numéros de cellules +*/
/*****************************************/

/**
 * @brief
 * Récupèration de l'index d'une cellule adjacente (dans une direction donnée).
 *
 * @return
 * Le numéro de la cellule adjacente si il y en a une dans la direction donnée
 * ou -1 si il n'y a pas de cellule adjacente.
 */
int get_adj_maze(maze* //!< Le labyrinthe.
    , int //!< La cellule.
    , cardinal //!< La direction.
);


/****************************************/
/*+ Gestion des cellules du labyrinthe +*/
/****************************************/

/**
 * @brief
 * Marque une cellule comme masquée.
 */
void mask_cell_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);


/**
 * @brief
 * Teste si une cellule est valide: est-elle bien dans la grille et non masquée ?
 *
 * @return
 *  Un Booléen indiquant si la cellule est valide.
 */
bool valid_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);

/**
 * @brief
 * Teste si une cellule est accessible.
 *
 * @remark
 * On suppose la labyrinthe est déjà construit et que les cellules accessibles ont
 * déjà été marquées dans le tableau "props" (l'information doit donc être récupérée
 * dans ce tableau).
 *
 * @return
 *  Un Booléen indiquant si la cellule est accessible.
 */
bool is_reach_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);


/**
 * @brief
 * Marque une cellule comme accessible.
 */
void make_reach_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);

/**
 * @brief
 * Teste si une cellule possède un mur dans la direction passée en entrée.
 *
 * @return
 * Un Booléen indiquant si la cellule possède un mur dans la direction passée en entrée.
 */
bool has_wall_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
    , cardinal//!< La direction.
);

/**
 * @brief
 * Construit un mur dans une cellule vers la direction passée en entrée.
 */
void build_wall_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
    , cardinal//!< La direction.
);

/**
 * @brief
 * Supprime le mur dans une cellule vers la direction passée en entrée.
 */
void del_wall_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
    , cardinal//!< La direction.
);


/**
 * @brief
 * Teste si une cellule est occupée: est-ce qu'elle contient le joueur ou un minotaure ?
 *
 * @return
 * Un Booléen indiquant si la cellule est occupée.
 */
bool is_occupied_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);

/**
 * @brief
 * Marque une cellule comme étant occupée.
 */
void make_occupied_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);

/**
 * @brief
 * Libère l'occupation d'une cellule.
 */
void free_occupied_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);

/************************/
/*+ Gestion des objets +*/
/************************/

/**
 * @brief
 * Retourne l'objet contenu dans une cellule.
 *
 * @return
 * L'objet contenu dans la cellule.
 */
object get_object_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);

/**
 * @brief
 * Modifie l'objet contenu dans une cellule.
 */
void add_object_maze(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
    , object //!< Le nouvel objet.
);

/**
 * @brief
 * Calcule le nombre de sorties d'un labyrinthe.
 *
 * @return
 * Un entier égal au nombre de sorties dans un labyrinthe.
 */
int get_exits_maze(maze* //!< Le labyrinthe.
);



/************************************/
/*+ Création d'un proto-labyrinthe +*/
/************************************/

/**
 * @brief
 * Création d'un proto-labyrinthe (il sera ensuite creusé par une fonction de génération).
 * Version sans masque.
 *
 * @remark
 * La fonction doit:
 * 1. Créer la structure (les dimensions du proto-labyrinthe sont passées en paramètre)
 * 2. Construire les murs: TOUTES les cellules sont murées dans les quatre directions.
 * 3. Marquer toutes les cellules comme non-masquées (il n'y a pas de masque).
 * 4. Placer le joueur à une position aléatoire.
 * 5. Marquer toutes les cellules comme accessibles: il n'y a pas de masque.
 *
 * @attention
 * Les objets et les minotaures ne sont pas placés par cette fonction (ils le seront après
 * la génération). La fonction ci-dessous doit seulement:
 * - initialiser le tableau object et remplir toutes ses cases par NONE.
 * - initialiser le nombre de minotaures à zéro et le tableau minotaurs à NULL.
 *
 * @return
 * Le proto-labyrinthe créé.
 */
maze* create_proto_maze_nomask(int //!< La taille horizontale.
    , int//!< La taille verticale.
);

/**
 * @brief
 * Création d'un proto-labyrinthe (il sera ensuite creusé par une fonction de génération).
 * Version avec masque.
 *
 * @remark
 * La fonction prend un masque en entrée. Elle doit:
 * 1. Créer la structure (les dimensions du proto-labyrinthe sont données par le masque)
 * 2. Construire les murs: TOUTES les cellules sont murées dans les quatre directions.
 * 3. Enregistrer les cellules masquées (elles sont indiquées par le masque).
 * 4. Placer le joueur à une position aléatoire.
 * 5. Calculer les cellules accessibles: celles qui ne sont pas séparées du joueur par des cases masquées.
 *
 * @attention
 * Les objets et les minotaures ne sont pas placés par cette fonction (ils le seront après
 * la génération). La fonction ci-dessous doit seulement:
 * - initialiser le tableau object et remplir toutes ses cases par NONE.
 * - initialiser le nombre de minotaures à zéro et le tableau minotaurs à NULL.
 *
 * @return
 * Le proto-labyrinthe créé.
 */
maze* create_proto_maze(mask* //!< Le masque à utiliser.
);

/**
 * @brief
 * Libération d'un labyrinthe.
 */
void free_maze(maze* //! Le labyrinthe à libérer.
);


/****************************/
/*+ Gestion des minotaures +*/
/****************************/


/**
 * @brief
 * Teste si il y a un minotaure sur une cellule.
 *
 * @return
 * Le numéro du minotaure se trouvant dans la cellule ou -1 si il n'y a pas de minotaure
 * dans la cellule.
 */
int has_minotaur_maze(maze* //!< Le labyrinthe.
    , int //!< La cellule.
);



/**
 * @brief
 * Génèration d'un nombre donné minotaures. Cette fonction est utilisée à la création d'un
 * nouveau jeu APRÈS la génération du labyrinthe et le placement du joueur.
 *
 * @remark
 * Le placement des nouveaux minotaures est aléatoire (la cellule qui contient le
 * joueur doit cependant être évitée).  Ils doivent être placés dans des cellules
 * accessibles. Si le nombre de minotaures demandé dépasse le nombre de cellules
 * accessibles, on le réduira le premier au second.
 *
 * @attention
 * Si il y a déjà des minotaures dans le labyrinthe, ils devront être supprimés et le tableau
 * qui contient leurs positions doit être réinitialisé (il faut le libèrer et le réallouer).
 */
void gen_minotaurs_maze(maze* //!< Le labyrinthe
    , int //!< Le nombre de minotaures demandé
);

/********************************/
/*+ Récupération d'information +*/
/********************************/

/**
 * @brief
 * Teste si un mouvement est valide à partir d'une cellule donnée. Le passage est-il ouvert
 * (il n'y a pas de mur) ? La cellule de destination est-elle valide et non occupée ?
 *
 * @return
 * Un Booléen indiquant si le mouvement est valide.
 */
bool valid_move_maze(maze* //!< Le labyrinthe.
    , int //!< Le cellule de départ.
    , move //!< Le mouvement à effectuer.
);



#endif
