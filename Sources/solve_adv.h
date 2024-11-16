/**
 * @file solve_adv.h
 * @brief
 * Solveur: chemins complexes.
 *
 * @details
 * Contient les types et les fonctions utilisées pour calculer des chemins complexes. Il s'agit de la partie
 * la plus libre du projet: seul le type est imposé. Ce que font les algorithmes et leur implémentation sont
 * libres. Un seul exemple est suggéré dans le fichier: écrire une fonction qui calcule un chemin complexe
 * visant à récupérer tous les trésors du labyrinthe.
 *
 * @remark
 * Un chemin complexe est implémenté comme une séquence de chemins simples qui se suivent.
 */

#ifndef SOLVEADV_H_
#define SOLVEADV_H_

 /*  ____        _                     ____ _                    _            */
 /* / ___|  ___ | |_   _____ _ __ _   / ___| |__   ___ _ __ ___ (_)_ __  ___  */
 /* \___ \ / _ \| \ \ / / _ \ '__(_) | |   | '_ \ / _ \ '_ ` _ \| | '_ \/ __| */
 /*  ___) | (_) | |\ V /  __/ |_  _  | |___| | | |  __/ | | | | | | | | \__ \ */
 /* |____/_\___/|_| \_/ \___|_| |(_)__\____|_| |_|\___|_| |_| |_|_|_| |_|___/ */
 /*  / __/ _ \| '_ ` _ \| '_ \| |/ _ \ \/ / _ \/ __|                          */
 /* | (_| (_) | | | | | | |_) | |  __/>  <  __/\__ \                          */
 /*  \___\___/|_| |_| |_| .__/|_|\___/_/\_\___||___/                          */
 /*                     |_|                                                   */

#include "game.h"
#include "solve.h"

 /**
  * @brief
  * Les noms internes des fonctions qui calculent des chemins complexes.
  *
  * @attention
  * Le dernier nom ("CMP_SIZE") est fictif: il sert uniquement à calculer automatiquement
  * le nombre de fonctions.
  *
  * @remark
  * Pour ajouter une nouvelle fonction, vous devez procéder en quatre étapes:
  * 1. Ecrire la fonction.
  * 2. Ajouter un nouveau nom à la fin de cet "enum" (juste avant "CMP_SIZE")
  * 3. Ajouter un pointeur vers la nouvelle fonction de génération à la fin du tableau "cmp_funs" dans solve_adv.c
  * 4. Ajouter le nom qui apparaîtra dans l'interface pour ce générateur à la fin du tableau "cmp_names" dans solve_adv.c
  */
typedef enum {
  CMP_TREAS, //!< Récupéreration de TOUS les trésors avant de sortir du labyrinthe (minotaures ignorés). 
  CMP_SIZE,
} cmp_algorithm;


/**
  * @brief
  * Implémentation d'un chemin complexe: il s'agit d'une séquence de chemins simples (sans cycle) qu'on
  * stocke dans un tableau.
  *
  * @attention
  * Bien que ce ne soit pas imposé par le type, deux chemins simples consécutifs sont censés se suivre.
  * C'est-à-dire que le point d'arrivée d'un chemin doit être égal au point de départ du chemin qui le
  * suit dans la séquence.
  */
typedef struct {
  cmp_algorithm algo; //!< Algorithme utilisé pour calculer le chemin.
  int size; //!< Le nombre de chemins simples qui composent le chemin complexe.
  sim_path** array; //!< tableau de taille size qui contient tous les chemins simples qui composent le chemin complexe (dans l'ordre).
  int length; //!< Longueur totale du chemin.
} cmp_search;

/**
 * @brief
 * Le tableau des fonctions qui calculent des chemins complexes.
 *
 * @remark
 * Une telle fonction prend un entrée un jeu et retourne un chemin complexe.
 */
extern cmp_search* (*cmp_funs[CMP_SIZE]) (game*);

/**
 * @brief
 * Le tableau des noms qui apparaissent dans l'interface pour chaque fonction qui calcule des chemins complexes.
 */
extern const char* cmp_names[CMP_SIZE];

/**
 * @brief
 * Libération d'un chemin complexe.
 */
void cmp_free(cmp_search* //!< Le chemin complexe.
);

/**
 * @brief
 * Calcule un parcours complexe qui permet de récupérer tous les trésors et de sortir du labyrinthe.
 *
 * @remark
 * Les minotaures sont ignorés.
 *
 * @return
 * Le chemin complexe (ou NULL si aucun chemin n'a été trouvé).
 */
cmp_search* cmp_taketreasures(game* //!< Le jeu
);



#endif
