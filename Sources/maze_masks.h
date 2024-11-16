/**
 * @file maze_masks.h
 * @brief
 * Définition et manipulation des masques.
 *
 * @details
 * Contient la définition du type utilisé pour représenter un masque ainsi que les fonctions
 * basiques qu'on utilisera pour manipuler ce type.
 */

#ifndef MASKS_H_
#define MASKS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "error.h"
#include "alloc.h"

 /*   ____           _   _                   _            */
 /*  / ___| ___  ___| |_(_) ___  _ __     __| | ___  ___  */
 /* | |  _ / _ \/ __| __| |/ _ \| '_ \   / _` |/ _ \/ __| */
 /* | |_| |  __/\__ \ |_| | (_) | | | | | (_| |  __/\__ \ */
 /*  \____|\___||___/\__|_|\___/|_| |_|  \__,_|\___||___/ */
 /* |  \/  | __ _ ___  __ _ _   _  ___  ___               */
 /* | |\/| |/ _` / __|/ _` | | | |/ _ \/ __|              */
 /* | |  | | (_| \__ \ (_| | |_| |  __/\__ \              */
 /* |_|  |_|\__,_|___/\__, |\__,_|\___||___/              */
 /*                      |_|                              */


 /***********************/
 /*+ Fonctions de base +*/
 /***********************/

 /**
 * @brief
 * Le type utilisé pour représenter un masque.
 *
 * @remark
 * Les cellules sont indexées par les entiers allant de 0 à hsize*vsize-1. Elles sont ordonnées ligne par ligne du Nord au Sud.
 * La cellule d'indice 0 se trouve au coin Nord-Ouest du labyrinthe. Les cellules allant de 0 à hsize-1 constituent la première
 * ligne au Nord du labyrinthe, et ainsi de suite.
 *
 * @attention
 * On supposera toujours qu'un masque contient au moins une cellule non-masquée (cette propriété doit être vérifiée à la lecture
 * d'un fichier masque).
 */
typedef struct {
    int hsize; //!< Taille horizontale.
    int vsize; //!< Taille verticale.
    int nbmasked; //!< Nombre de cases masquées. Toujours strictement inférieur au nombre total de cases (au moins une case doit être non-masquée).
    bool* grid; //!< Tableau de Booléens qui indique les cellules masquées. La cellule i est dans la ième case du tableau (true: masqué, false: non-masqué).
} mask;


/**
 * @brief
 * Création d'une masque vide: aucune case n'est masquée.
 *
 * @return
 * Le masque vide créé.
 */
mask* create_empty_mask(int //!< Taille horizontale.
    , int //!< Taille verticale.
);

/**
 * @brief
 * Libération d'un masque.
 */
void free_mask(mask* //!< Le masque à libérer.
);

/**
 * @brief
 * Redimensionnement d'un masque.
 *
 * @details
 * Conversion un masque dont les dimensions sont arbitraires pour lui donner les dimensions voulues.
 *
 * @attention
 * Il doit toujours y avoir au moins une case non-masquée après redimensionnement.
 *
 */
void resize_mask(mask* //!< Le masque à redimensionner.
    , int //!< Nouvelle taille horizontale.
    , int //!< Nouvelle taille verticale.
);

/**
 * @brief
 * Affichage d'un masque sur le terminal (utile uniquement pour faire des tests).
 */
void print_mask(mask* //!< Le masque à afficher.
);

/**************************************/
/*+ Génération à partir d'un fichier +*/
/**************************************/

/**
 * @brief
 * Génération d'un masque à partir d'un fichier.
 *
 * @attention
 * Le fichier doit contenir une représentation ASCII de la grille:
 * - Le caractère X correspond à une cellule masquée.
 * - Tout autre caractère indique une cellule non-masquée.
 *
 * @remark
 * Des exemples de tels fichiers sont donnés dans le répertoire "Masques".
 *
 * @attention
 * Si toutes les cases sont masquées: on devra retourner NULL.
 *
 * @return
 * Le masque lu dans le fichier.
 */
mask* read_mask(const char* //!< Chaîne de caractères contenant le chemin qui mène au fichier.
);




#endif
