/**
 * @file data_queue.h
 * @brief
 * Implémentation d'une files d'entiers.
 *
 * @remark
 * Cette structure de données est utilisée pour les parcours BFS.
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "alloc.h"

 /*  _____ _ _            */
 /* |  ___(_) | ___  ___  */
 /* | |_  | | |/ _ \/ __| */
 /* |  _| | | |  __/\__ \ */
 /* |_|   |_|_|\___||___/ */


/**
 * @brief
 * Implémentation d'une file.
 */
typedef struct {
    int* array; //!< Tableau des valeurs.
    int size_array; //!< Taille du tableau des valeurs.
    int left;  //!< Indice de la valeur au début de la file (si non-vide). 
    int right; //!< Indice qui suit celui de la valeur à la fin de la file (si non-vide).
    bool empty; //!< Booléen indiquant si la file est vide.
} queue;

/***************************/
/*+ Fonctions auxiliaires +*/
/***************************/

/**
 * @brief
 * Multiplication par deux de la taille de la file.
 */
void grow_queue(queue* //!< La file.
);

/**
 * @brief
 * Division par deux de la taille de la file.
 */
void shrink_queue(queue* //!< La file.
);

/****************/
/*+ Primitives +*/
/****************/

/**
 * @brief
 * Création d'une file vide.
 *
 * @return
 * La file vide.
 */
queue* create_queue(void);

/**
 * @brief
 * Suppression d'une file.
 */
void delete_queue(queue* //!< La file.
);

/**
 * @brief
 * Calcul de la taille d'une file.
 *
 * @return
 * La taille de la file.
 */
int getsize_queue(queue* //!< La file.
);

/**
 * @brief
 * Teste si une file est vide.
 *
 * @return.
 * Un Booléen indiquant si la file est vide.
 */
bool is_empty_queue(queue* //!< La file.
);

/**
 * @brief
 * Enfile une nouvelle valeur dans la file.
 */
void enqueue(int //!< La valeur à enfiler.
    , queue* //!< La file.
);

/**
 * @brief
 * Lit une valeur quelconque de la file (sans la modifier).
 *
 * @return
 * La valeur lue.
 */
int read_queue(queue* //!< La file.
    , int //!< L'indice de la valeur à lire (à partir du début de la file, la première valeur a pour indice 0).
);

/**
 * @brief
 * Défile la valeur à la fin de la file.
 *
 * @return
 * La valeur défilée.
 */
int dequeue(queue* //!< La file.
);



#endif
