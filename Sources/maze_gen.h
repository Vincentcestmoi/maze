/**
 * @file maze_gen.h
 * @brief
 * Générateurs de labyrinthes.
 *
 * @details
 * Contient les fonctions servant à générer un labyrinthe.
 *
 * @remark
 * Une telle fonction prend un proto-labyrinthe en entrée et le modifie en "creusant" ses
 * murs (elle ne retourne rien)
 *
 * @attention
 * Ces fonctions ne sont pas censées générer les minotaures et les objets: ce sera
 * fait plus tard lors de la création d'un jeu.
 */

#ifndef MAZEGEN_H_
#define MAZEGEN_H_

#include "maze.h"
#include "data_ufind.h"

 /*   ____   __         __           _   _                   _       */
 /*  / ___| /_/ _ __   /_/ _ __ __ _| |_(_) ___  _ __     __| | ___  */
 /* | |  _ / _ \ '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \   / _` |/ _ \ */
 /* | |_| |  __/ | | |  __/ | | (_| | |_| | (_) | | | | | (_| |  __/ */
 /*  \____|\___|_| |_|\___|_|  \__,_|\__|_|\___/|_| |_|  \__,_|\___| */
 /* | |    __ _| |__  _   _ _ __(_)_ __ | |_| |__   ___  ___         */
 /* | |   / _` | '_ \| | | | '__| | '_ \| __| '_ \ / _ \/ __|        */
 /* | |__| (_| | |_) | |_| | |  | | | | | |_| | | |  __/\__ \        */
 /* |_____\__,_|_.__/ \__, |_|  |_|_| |_|\__|_| |_|\___||___/        */
 /*                   |___/                                          */

 /***************************/
 /*+ Liste des générateurs +*/
 /***************************/

/**
 * @brief
 * La liste des noms internes des générateurs de labyrinthes.
 *
 * @attention
 * Le dernier nom ("GEN_SIZE") est fictif: il sert uniquement à calculer automatiquement
 * le nombre de générateurs.
 *
 * @remark
 * Pour ajouter un nouveau générateur, vous devez procéder en quatre étapes:
 * 1. Écrire la fonction de génération.
 * 2. Ajouter un nouveau nom à la fin de cet "enum" (juste avant "GEN_SIZE")
 * 3. Ajouter un pointeur vers la nouvelle fonction de génération à la fin du tableau "gen_funs" dans maze_gen.c
 * 4. Ajouter le nom qui apparaîtra dans l'interface pour ce générateur à la fin du tableau "gen_names" dans maze_gen.c
 */
typedef enum {
    GEN_TEST, //!< Fonction de test: retourne le labyrinthe créé par create_proto_maze sans casser de mur
    GEN_AB, //!< Aldous-Broder
    GEN_WIL, //!< Wilson
    GEN_HKLIN, //!< Hunt & Kill: Linéaire.
    GEN_HKDFS, //!< Hunt & Kill: DFS.
    GEN_HKRAND, //!< Hunt & Kill: Aléatoire.
    GEN_PRIM, //!< Prim
    GEN_KRUSKAL, //!< Kruskal
    GEN_REC, //!< Divisions récursives
    GEN_SIZE,
} generator;

/**
 * @brief
 * Le tableau des fonctions de génération.
 */
extern void (*gen_funs[GEN_SIZE]) (maze*);

/**
 * @brief
 * Le tableau des noms qui apparaissent dans l'interface pour chaque fonction de génération.
 */
extern const char* gen_names[GEN_SIZE];




/*****************************/
/*+ Fonctions de génération +*/
/*****************************/


/**
 * @brief
 * Cette fonction ne doit pas creuser le proto-labyrinthe passé en entrée. Elle sert
 * uniquement la création de proto-labyrinthe.
 */
void maze_test(maze* //!< Le proto-labyrinthe.
);


/**
 * @brief
 * Algorithme de Aldous-Broder.
 */
void random_maze_ab(maze* //!< Le proto-labyrinthe.
);

/**
 * @brief
 * Algorithme de Wilson.
 */
void random_maze_wilson(maze* //!< Le proto-labyrinthe.
);

/**
 * @brief
 * Algorithme Hunt & Kill: phase "hunt" basée sur une recherche linéaire
 */
void random_maze_hklinear(maze* //!< Le proto-labyrinthe.
);


/**
 * @brief
 * Algorithme Hunt & Kill: phase "hunt" basée sur un BFS
 */
void random_maze_hkdfs(maze* //!< Le proto-labyrinthe.
);

/**
 * @brief
 * Algorithme Hunt & Kill: phase "hunt" aléatoire
 */
void random_maze_hkrandom(maze* //!< Le proto-labyrinthe.
);




/**
 * @brief
 * Génération basée sur l'algorithme Prim.
 */
void random_maze_prim(maze* //!< Le proto-labyrinthe.
);



/**
 * @brief
 * Génération basée sur l'algorithme de Kruskal.
 */
void random_maze_kruskal(maze* //!< Le proto-labyrinthe.
);





/**
 * @brief
 * Génération basée sur la division récursive. Cette approche ne fonctionne que si le
 * labyrinthe n'a AUCUNE cellule masquée.
 */
void random_maze_rec(maze* //!< Le proto-labyrinthe.
);

void generate_maze(maze* //!< Le proto-labyrinthe.
    , generator //!< Le générateur à utiliser.
);


#endif
