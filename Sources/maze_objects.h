/**
 * @file maze_objects.h
 * @brief
 * Générateurs d'objets.
 *
 * @details
 * Contient les fonctions servant à générer des objets à l'intérieur d'un labyrinthe.
 *
 * @remark
 * Une telle fonction prend un labyrinthe déjà construit en entrée. Elle modifie son
 * tableau d'objets (elle ne retourne rien).
 */

#ifndef MAZEOBJ_H_
#define MAZEOBJ_H_

#include "maze.h"

 /*   ____   __         __           _   _                   _            */
 /*  / ___| /_/ _ __   /_/ _ __ __ _| |_(_) ___  _ __     __| | ___  ___  */
 /* | |  _ / _ \ '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \   / _` |/ _ \/ __| */
 /* | |_| |  __/ | | |  __/ | | (_| | |_| | (_) | | | | | (_| |  __/\__ \ */
 /*  \____|\___|_| |_|\___|_|  \__,_|\__|_|\___/|_| |_|  \__,_|\___||___/ */
 /*   ___ | |__ (_) ___| |_ ___                                           */
 /*  / _ \| '_ \| |/ _ \ __/ __|                                          */
 /* | (_) | |_) | |  __/ |_\__ \                                          */
 /*  \___/|_.__// |\___|\__|___/                                          */
 /*           |__/                                                        */


 /************************************/
 /*+ Liste des générateurs d'objets +*/
 /************************************/


/**
 * @brief
 * La liste des noms internes des générateurs d'objets.
 *
 * @attention
 * Le dernier nom ("OBJ_SIZE") est fictif: il sert uniquement à calculer automatiquement
 * le nombre de générateurs.
 *
 * @remark
 * Pour ajouter un nouveau générateur, vous devez procéder en quatre étapes:
 * 1. Écrire la fonction de génération.
 * 2. Ajouter un nouveau nom à la fin de cet "enum" (juste avant "OBJ_SIZE")
 * 3. Ajouter un pointeur vers la nouvelle fonction de génération à la fin du tableau "obj_funs" dans maze_objects.c
 * 4. Ajouter le nom qui apparaîtra dans l'interface pour ce générateur à la fin du tableau "obj_names" dans maze_objects.c
 */
typedef enum {
    OBJ_EMPTY,
    OBJ_SIMPLE,
    OBJ_SIZE,
} objgenerator;

/**
 * @brief Le tableau des fonctions de génération.
 */
extern void (*obj_funs[OBJ_SIZE]) (maze*);


/**
 * @brief Le tableau des noms qui apparaissent dans l'interface pour chaque fonction de génération.
 */
extern const char* obj_names[OBJ_SIZE];




/*****************************/
/*+ Fonctions de génération +*/
/*****************************/

/**
 * @brief Fonction de génération triviale: ne génère aucun objet.
 */
void obj_empty(maze* //!< Le labyrinthe.
);


/**
 * @brief Fonction de génération basique.
 * @details La fonction commence par créer une unique sortie sur une cellule choisie
 *  aléatoirement. Pour toutes les autres cellules, il y a une chance de 10% qu'elle
 * contienne un objet (autre qu'une sortie). Cet objet est alors choisi aléatoirement.
 */
void obj_simple(maze* //!< Le labyrinthe.
);

void generate_objects_maze(maze* //!< Le labyrinthe.
    , objgenerator //!< Le générateur d'objets à utiliser.
);



#endif
