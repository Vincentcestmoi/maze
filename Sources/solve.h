/**
 * @file solve.h
 * @brief
 * Solveur: chemins simples.
 *
 * @details
 * Contient le type qu'on utilise pour représenter un chemin "simple" et les algorithmes
 * de parcours qu'on utilise pour les générer (BFS, DFS et A*).
 *
 * @remark
 * Un chemin simple est un chemin qui ne contient PAS de cycle. Même si le type qu'on va
 * utiliser ici permet théoriquement de représenter un chemin qui possède un cycle, nous
 * ne nous en servirons pas de cette façon. Les chemins qui peuvent contenir des cycles
 * sont traités par les fonctions décrites dans "solve_adv.h": on les manipule comme des
 * séquences de plusieurs chemins simples (on parle de chemins "complexes").
 */

#ifndef SOLVE_H_
#define SOLVE_H_

 /*  ____        _                     ____ _                    _            */
 /* / ___|  ___ | |_   _____ _ __ _   / ___| |__   ___ _ __ ___ (_)_ __  ___  */
 /* \___ \ / _ \| \ \ / / _ \ '__(_) | |   | '_ \ / _ \ '_ ` _ \| | '_ \/ __| */
 /*  ___) | (_) | |\ V /  __/ |   _  | |___| | | |  __/ | | | | | | | | \__ \ */
 /* |____/ \___/|_| \_/ \___|_|  (_)  \____|_| |_|\___|_| |_| |_|_|_| |_|___/ */
 /*  ___(_)_ __ ___  _ __ | | ___  ___                                        */
 /* / __| | '_ ` _ \| '_ \| |/ _ \/ __|                                       */
 /* \__ \ | | | | | | |_) | |  __/\__ \                                       */
 /* |___/_|_| |_| |_| .__/|_|\___||___/                                       */
 /*                 |_|                                                       */

#include "game.h"
#include "data_binheap.h"

/***************************************************/
/*+ Définition et manipulation d'un chemin simple **/
/***************************************************/

/**
 * @brief
 * Représentation d'une séquence de mouvements.
 *
 * @remark
 * On se sert d'une liste chaînée.
 */
struct sim_move_seq {
    move direction; //!< Un mouvement
    struct sim_move_seq* next; //!< Pointeur vers le mouvement suivant.
};

typedef struct sim_move_seq sim_move_seq;

/**
 * @brief
 * Représentation d'un chemin simple (sans cycle).
 *
 * @attention
 * Théoriquement, ce type permet de représenter un chemin qui contient des cycles
 * (rien n'empêche la séquence de mouvements d'induire un cycle). Néanmoins, nous
 * nous servirons de lui uniquement pour représenter des chemins simples. On voit
 * un chemin contenant des cycles comme une séquence de plusieurs chemins simples
 * et un type dédié est défini dans "solve_adv.h".
 */
typedef struct {
    int start; //!< La cellule de départ.
    int end; //!< La cellule d'arrivée.
    int length; //!< La longueur du chemin.
    sim_move_seq* moves; //!< La séquence de mouvements à effectuer depuis cette cellule (NULL si le chemin est vide).
} sim_path;

/**
 * @brief
 * Création d'un nouveau chemin (vide).
 *
 * @return
 * Le chemin vide.
 */
sim_path* sim_emptypath(int //!< Le point de départ.
);

/**
 * @brief
 * Ajout un mouvement supplémentaire au début d'un chemin.
 *
 * @attention
 * Puisqu'on ajoute le mouvement AU DÉBUT de la séquence, la cellule de départ et la longueur
 * doivent être modifiés.
 */
void sim_addtopath(maze*//!< Le labyrinthe (utilisé pour calculer le nouveau point de départ).
    , move //!< Le mouvement à ajouter.
    , sim_path* //! Le chemin à modifier.
);

/**
 * @brief Crée une copie d'un chemin existant.
 *
 * @return
 * La copie.
 */
sim_path* sim_copypath(sim_path* //! Le chemin à copier.
);

/**
 * @brief Libération d'un chemin.
 */
void sim_freepath(sim_path* //! Le chemin à libérer.
);



/**********************************************/
/*+ Définition et manipulation d'un parcours **/
/**********************************************/

/**
 * @brief
 * Les différents algorithmes de parcours disponibles.
 */
typedef enum {
    ALG_BFS, //!< Parcours en largeur.
    ALG_DFS, //!< Parcours en profondeur.
    ALG_AST, //!< Algorithme A*.
    ALG_SIZE,
} sim_algorithm;

/**
 * @brief
 * Les différents types d'objets qu'on peut rechercher en utilisant un parcours.
 */
typedef enum {
    GOA_TREASURE, //!< Trésor.
    GOA_BOMB, //!< Bombe.
    GOA_POLY, //!< Poly d'algo.
    GOA_EXIT, //!< Sortie.
    GOA_SIZE,
} sim_goal;

/**
 * @brief
 * Représentation de l'information récupérée lors d'un parcours du labyrinthe
 * en utilisant un des algorithmes de parcours.
 */
typedef struct {
    sim_algorithm algo; //!< L'algorithme utilisé pour le calcul.
    sim_goal goal; //!< L'objet qui était recherché.
    dynarray* search; //!< La liste des sommets visités par le parcours stockée dans l'ordre de visite à l'intérieur d'un tableau dynamique.
    sim_path* path; //!< Le chemin vers une sortie trouvé par le parcours (NULL si aucun chemin n'a été trouvé).
}
sim_search;

/**
 * @brief
 * Tableau des noms des algorithmes de parcours qui appraissent dans l'interface.
 */
extern const char* salgo_names[ALG_SIZE];

/**
 * @brief
 * Tableau des noms des types d'objets qui appraissent dans l'interface.
 */
extern const char* sgoal_names[GOA_SIZE];

/**
 * @brief
 * Création d'un nouveau parcours "vide". Il sera ensuite rempli par l'un des algorithmes.
 */
sim_search* sim_create_search(sim_algorithm //!< L'algorithme qui va être utilisé.
    , sim_goal //!< Le type d'objet qui va être recherché.
);

/**
 * @brief
 * Libération d'un parcours.
 */
void sim_free_search(sim_search* //<! Le parcours à libérer.
);


/******************************************************************/
/*+ Les heuristiques  (uniquement utilisées pas l'algorithme A*) **/
/******************************************************************/

/**
 * @brief La liste des noms internes des heuristiques (utile uniquement pour l'algorithme A*).
 *
 * @attention
 * Le dernier nom ("HEU_SIZE") est fictif: il sert uniquement à calculer automatiquement
 * le nombre d'heuristiques.
 *
 * @remark
 * Pour ajouter une nouvelle heuristique, vous devez procéder en quatre étapes:
 * 1. Écrire la fonction.
 * 2. Ajouter un nouveau nom à la fin de cet "enum" (juste avant "HEU_SIZE")
 * 3. Ajouter un pointeur vers la nouvelle fonction à la fin du tableau "heu_funs" dans solve_astar.c
 * 4. Ajouter le nom qui apparaîtra dans l'interface pour cete heuristique à la fin du tableau "heu_names" dans solve_astar.c
 */
typedef enum {
    HEU_NONE, //!< Heurstique constante.
    HEU_MANH, //!< Distance manhattan avec la sortie la plus proche.
    HEU_RUNA, //!< Augmente plus les minotaures sont proches.
    HEU_SIZE,
}heuristic;

/**
 * @brief
 * Le tableau des noms qui apparaissent dans l'interface pour chaque heuristique (utile uniquement pour l'algorithme A*).
 */
extern const char* heu_names[HEU_SIZE];

/**
 * @brief
 * Le tableau des heuristiques (utile uniquement pour l'algorithme A*).
 *
 * @details
 * Une heuristique prend trois paramètres:
 * 1. Le jeu.
 * 2. L'index d'une cellule.
 * 3. La liste des cellules qu'on cherche à atteindre (sous la forme d'un tableau dynamique).
 *
 * Elle retourne un entier qui, intuitivement, correspond à une approximation de la distance qui
 * reste à parcourir pour atteindre l'une des cellules voulues.
 */
extern int(*heu_funs[HEU_SIZE]) (game*, int, dynarray*);

/**
 * @brief
 * Heuristique constante (donne un parcours BFS).
 */
int astar_none(game* //!< Le jeu.
    , int //!< La cellule.
    , dynarray* //!< La liste des cellules qu'on recherche.
);

/**
 * @brief
 * Distance manhattan avec la sortie la plus proche.
 */
int astar_manhattan(game* //!< Le jeu.
    , int //!< La cellule.
    , dynarray* //!< La liste des cellules qu'on recherche.
);

/**
 * @brief
 * Heuristique cherchant à éviter les minotaures.
 */
int astar_runaway(game* //!< Le jeu.
    , int //!< La cellule.
    , dynarray* //!< La liste des cellules qu'on recherche.
);


/*********************************/
/*+ Les algorithmes de parcours **/
/*********************************/

/**
 * @brief
 * Tableaux des fonctions qui implémentent les algorithmes de parcours.
 *
 * @remark
 * Une telle fonction retourne le parcours effectué et prend les deux paramètres suivants
 * en entrée:
 * 1. Le jeu.
 * 2. La cellule dont part le parcours.
 * 3. Une heuristique. Ce paramètre est utilisé uniquement par l'algorithme A* (pour les autres
 *    algorithmes, il est là car toutes les fonctions de parcours doivent avoir le même type).
 * 4. Un Booléen qui indique si les minotaures doivent être pris en compte ou pas (si c'est le
 *    cas, ils bloquent le joueur mais sont supposés immobiles).
 */
extern sim_search* (*salgo_funs[ALG_SIZE]) (game*, int, sim_goal, int(heu_fun)(game*, int, dynarray*), bool);


/**
 * @brief Algorithme de parcours en largeur.
 *
 * @remark
 * L'heuristique passée en entrée n'est pas utilisée par cet algorithme (elle est là car toutes
 * les fonctions de parcours doivent avoir le même type).
 *
 * @return
 * Le parcours.
 */
sim_search* sim_bfs(game* //!< Le jeu.
    , int //!< La cellule de départ.
    , sim_goal //!< Le type d'objet recherché.
    , int(*)(game*, int, dynarray*) // L'heuristique (inutilisée ici).
    , bool //!< Doit-on prendre en compte les minotaures?
);

/**
 * @brief Algorithme de parcours en profondeur.
 *
 * @remark
 * L'heuristique passée en entrée n'est pas utilisée par cet algorithme (elle est là car toutes
 * les fonctions de parcours doivent avoir le même type).
 *
 * @return
 * Le parcours.
 */
sim_search* sim_dfs(game* //!< Le jeu.
    , int //!< La cellule de départ.
    , sim_goal //!< Le type d'objet recherché.
    , int(*)(game*, int, dynarray*) // L'heuristique (inutilisée ici).
    , bool //!< Doit-on prendre en compte les minotaures?
);

/**
 * @brief Algorithme A*.
 *
 * @remark
 * Il s'agit du seul parmi les trois algorithmes qui utilise l'heuristique passée en entrée.
 *
 * @return
 * Le parcours.
 */
sim_search* sim_astar(game* //!< Le jeu.
    , int //!< La cellule de départ.
    , sim_goal //!< Le type d'objet recherché.
    , int(*)(game*, int, dynarray*) // L'heuristique.
    , bool //!< Doit-on prendre en compte les minotaures?
);


#endif
