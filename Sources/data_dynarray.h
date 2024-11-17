/**
 * @file data_dynarray.h
 * @brief
 * Implémentation d'un tableau dynamique de nombres entiers.
 *
 * @details
 * La structure de données est identique à celle utilisée pour implémenter une pile.
 *
 * @remarks
 * Cette structure de données est utilisée régulièrement dans le projet. En particulier,
 * il est possible de s'en servir lorsque l'on a besoin d'une pile.
 */

#ifndef DYNARRAY_H_
#define DYNARRAY_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "alloc.h"
#include "error.h"

 /*  _____     _     _                                          */
 /* |_   _|_ _| |__ | | ___  __ _ _   ___  __                   */
 /*   | |/ _` | '_ \| |/ _ \/ _` | | | \ \/ /                   */
 /*   | | (_| | |_) | |  __/ (_| | |_| |>  <                    */
 /*   |_|\__,_|_.__/|_|\___|\__,_|\__,_/_/\_\                   */
 /*   __| |_   _ _ __   __ _ _ __ ___ (_) __ _ _   _  ___  ___  */
 /*  / _` | | | | '_ \ / _` | '_ ` _ \| |/ _` | | | |/ _ \/ __| */
 /* | (_| | |_| | | | | (_| | | | | | | | (_| | |_| |  __/\__ \ */
 /*  \__,_|\__, |_| |_|\__,_|_| |_| |_|_|\__, |\__,_|\___||___/ */
 /*        |___/                            |_|                 */



/**
 * @brief
 * Implémentation d'un tableau dynamique.
 */
typedef struct {
    int* array; //!< Le tableau contenant les valeurs.
    int size; //!< Le nombre de valeurs dans la structure.
    int capacity; //!< La capcité totale actuelle du tableau array.
} dynarray;

/***************************************/
/*+ Modification de la taille interne +*/
/***************************************/

/**
 * @brief
 * Multiplication par deux de la taille du tableau array.
 */
void double_dyn(dynarray* dyn //!< Le tableau dynamique.
);

/**
 * @brief
 * Division par deux de la taille du tableau array.
 */
void divide_dyn(dynarray* //!< Le tableau dynamique.
);

/****************/
/*+ Primitives +*/
/****************/

/**
 * @brief
 * Création d'un tableau dynamique vide.
 *
 * @return
 * Le tableau dynamique créé.
 */
dynarray* create_dyn(void);

/**
 * @brief
 * Libération d'un tableau dynamique.
 */
void free_dyn(dynarray* //!< Le tableau dynamique.
);

/**
 * @brief
 * Calcule la taille d'un tableau dynamique.
 *
 * @return
 * La taille du tableau dynamique.
 */
int size_dyn(dynarray* //!< Le tableau dynamique.
);

/**
 * @brief
 * Teste si un tableau dynamique est vide.
 *
 * @return
 * Un Booléeen indiquant si le tableau dynamique est vide.
 */
bool is_empty_dyn(dynarray* //!< Le tableau dynamique.
);

/**
 * @brief
 * Insertion d'une nouvelle valeur à la fin d'un tableau dynamique.
 */
void push_dyn(int //!< La valeur à insérer.
    , dynarray* //!< Le tableau dynamique.
);

/**
 * @brief
 * Suppression de la dernière valeur à la fin d'un tableau dynamique.
 *
 * @return
 * La valeur supprimée.
 */
int pop_dyn(dynarray* //!< Le tableau dynamique.
);



#endif 
