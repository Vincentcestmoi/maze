/**
 * @file data_binheap.h
 * @brief
 * Implémentation d'un tas binaire générique.
 *
 * @details
 * On utilise un tableau.
 *
 * @remark
 * Cette structure de données est utilisée uniquement par l'algorithme A*.
 */

#ifndef BINHEAP_H_
#define BINHEAP_H_

#include <stdio.h>
#include <stdbool.h>
#include "alloc.h"

 /*  _____           _     _             _                */
 /* |_   _|_ _ ___  | |__ (_)_ __   __ _(_)_ __ ___  ___  */
 /*   | |/ _` / __| | '_ \| | '_ \ / _` | | '__/ _ \/ __| */
 /*   | | (_| \__ \ | |_) | | | | | (_| | | | |  __/\__ \ */
 /*   |_|\__,_|___/ |_.__/|_|_| |_|\__,_|_|_|  \___||___/ */



/**
 * @brief
 * Implémentation d'un tas binaire.
 */
typedef struct {
    void** array; //!< Le tableau des valeurs. Puisqu'il s'agit d'une implémentation générique, les éléments sont de type void*.
    int size_array; //!< Le taille du tableau.
    int size_heap; //!< Le nombre d'éléments dans le tas.
    int (*fc) (void*, void*); //!< Pointeur de fonction sur la fonction de comparaison utilisée.
} binheap;

/***************************/
/*+ Fonctions auxiliaires +*/
/***************************/

/**
 * @brief
 * Calcule le fils gauche d'un noeud.
 *
 * @return
 * L'indice du fils gauche.
 */
int left_binheap(int //!< L'indice du noeud.
);

/**
 * @brief
 * Calcule le fils droit d'un noeud.
 *
 * @return
 * L'indice du fils droit.
 */
int right_binheap(int //!< L'indice du noeud.
);

/**
 * @brief
 * Retourne le parent d'un noeud.
 *
 * @return
 * L'indice du parent.
 */
int parent_binheap(int //!< L'indice du noeud.
);


/**
 * @brief
 * Teste si un indice correspond à un noeud valide.
 *
 * @return
 * Un Booléen indiquant si l'indice est valide.
 */
bool isvalid_binheap(binheap* //!< Le tas binaire.
    , int //!< L'indice du noeud.
);

/**
 * @brief
 * Multiplication par deux de la taille du tableau.
 */
void grow_binheap(binheap* //!< Le tas binaire.
);

/**
 * @brief
 * Division par deux de la taille du tableau.
 */
void shrink_binheap(binheap* //!< Le tas binaire.
);

/****************/
/*+ Primitives +*/
/****************/

/**
 * @brief
 * Création d'un tas binaire vide.
 *
 * @return
 * Le tas vide créé
 */
binheap* create_binheap(int (*) (void*, void*) //!< Pointeur vers la fonction de comparaison à utiliser.
);

/**
 * @brief
 * Suppression d'un tas binaire.
 */
void delete_binheap(binheap* //!< Le tas binaire.
);

/**
 * @brief
 * Teste si le tas binaire est vide.
 *
 * @return
 * Un Booléen indiquant si le tas est vide.
 */
bool isempty_binheap(binheap* //!< Le tas binaire.
);

/**
 * @brief
 * Calcule la taille du tas binaire.
 *
 * @return
 * La taille du tas binaire.
 */
int getsize_binheap(binheap* //!< Le tas binaire.
);

/**
 * @brief
 * Insère une nouvelle valeur dans un tas binaire.
 */
void push_binheap(binheap* //!< Le tas binaire.
    , void* //!< Un pointeur vers la valeur à insérer.
);

/**
 * @brief
 * Récupère le minimum d'un tas binaire (sans le supprimer).
 *
 * @return
 * Le minimum du tas binaire.
 */
void* peekmin_binheap(binheap* //!< Le tas binaire.
);

/**
 * @brief
 * Récupère le minimum d'un tas binaire (en le supprimant).
 *
 * @return
 * Le minimum du tas binaire.
 */
void* popmin_binheap(binheap* //!< Le tas binaire.
);

#endif
