/**
 * @file data_ufind.h
 * @brief
 * Implémentation de union-find.
 *
 * @remark
 * Cette structure de données est uniquement utilisée dans l'algorithme de Kruskal.
 */

#ifndef UNIONFIND_H
#define UNIONFIND_H


 /*  _   _       _                   _____ _           _  */
 /* | | | |_ __ (_) ___  _ __       |  ___(_)_ __   __| | */
 /* | | | | '_ \| |/ _ \| '_ \ _____| |_  | | '_ \ / _` | */
 /* | |_| | | | | | (_) | | | |_____|  _| | | | | | (_| | */
 /*  \___/|_| |_|_|\___/|_| |_|     |_|   |_|_| |_|\__,_| */

#include <stdbool.h>
#include <stdlib.h>
#include "alloc.h"


/**
 * @brief
 * Implémentation de union-find.
 */
typedef struct
{
    int size_tab; //!< Taille des tableaux.
    int size_set; //!< Taille de l'ensemble partitionné.
    int size_par; //!< Taille de la partition.
    int* parent;  //!< Tableau de la relation parent.
    int* rank;    //!< Tableau des rangs (significatif seulement pour les racines).
    int* sizec;   //!< Tableau des tailles des classes (significatif seulement pour les racines).
} ufind;

/********************/
/*+ Les primitives +*/
/********************/

/**
 * @brief
 * Crée un nouvel union-find représentant une partition triviale en singletons.
 *
 * @return
 * Le union-find créé.
 */
ufind* create_ufind(int //!< Le nombre d'éléments à partitionner.
);

/**
 * @brief
 * Libération d'un union-find.
 */
void delete_ufind(ufind* //!< Le union-find.
);

/**
 * @brief
 * Calcule la taille de l'ensemble partitionné.
 *
 * @return
 * La taille de l'ensemble partitionné.
 */
int sizeset_ufind(ufind* //!< Le union-find.
);

/**
 * @brief
 * Calcule le nombre de classes dans la partition.
 *
 * @return
 * Le nombre de classes dans la partition.
 */
int sizepar_ufind(ufind* //!< Le union-find.
);

/**
 * @brief
 * Agrandit l'ensemble à partitionner en ajoutant un élément.
 *
 * @remark
 * La classe de l'élément ajouté est un singleton.
 */
void makeset_ufind(ufind* //!< Le union-find.
);

/**
 * @brief
 * Récupére le représentant de la classe d'un élément.
 *
 * @return
 * Le numéro du représentant.
 */
int find_ufind(int i //!< L'élément.
    , ufind* //!< Le union-find.
);

/**
 * @brief
 * Calcule la taille de la classe d'un élément.
 *
 * @return
 * La taille de la classe.
 */
int sizeclass_ufind(int i //!< L'élément.
    , ufind* //!< Le union-find.
);

/**
 * @brief
 * Réalise l'union des classes de deux éléments.
 */
void union_ufind(int i //!< Le premier élément.
    , int j //!< Le second élément.
    , ufind* //!< Le union-find.
);

#endif
