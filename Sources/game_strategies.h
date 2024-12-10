/**
 * @file game_strategies.h
 * @brief
 * Stratégies pour les minotaures.
 *
 * @details
 * Contient les fonctions utilisées pour déterminer les mouvements des minotaures. Chaque fonction
 * correspond à une stratégie différente.
 *
 * @remark
 * Une fonction de stratégie n'a pas de retour (il se fait par pointeur). Elle prend les paramètres
 * suivants en entrée:
 * - Un labyrinthe
 * - Un mouvement (celui qui vient d'être effectué par le joueur).
 * - un tableau de mouvement. Sa taille doit être égale au nombre de minotaures. Il sert à retourner
 *   (par pointeur) les mouvements de tous les minotaures.
 *
 * @attention
 * Une stratégie ne doit pas forcément vérifier que les mouvements qu'elle donne pour les minotaures
 * sont réalisables (c'est possible mais pas nécessaire). Cette vérification sera effectuée plus tard
 * par le jeu.
 */

#ifndef STRATEGIES_H_
#define STRATEGIES_H_

#include "maze.h"
#include "data_queue.h"

 /*  ____  _             _    __       _                                     */
 /* / ___|| |_ _ __ __ _| |_ /_/  __ _(_) ___  ___   _ __   ___  _   _ _ __  */
 /* \___ \| __| '__/ _` | __/ _ \/ _` | |/ _ \/ __| | '_ \ / _ \| | | | '__| */
 /*  ___) | |_| | | (_| | ||  __/ (_| | |  __/\__ \ | |_) | (_) | |_| | |    */
 /* |____/ \__|_|  \__,_|\__\___|\__, |_|\___||___/ | .__/ \___/ \__,_|_|    */
 /* | | ___  ___   _ __ ___ (_)_ |___/___ | |_ __ _ |_| _ _ __ ___  ___      */
 /* | |/ _ \/ __| | '_ ` _ \| | '_ \ / _ \| __/ _` | | | | '__/ _ \/ __|     */
 /* | |  __/\__ \ | | | | | | | | | | (_) | || (_| | |_| | | |  __/\__ \     */
 /* |_|\___||___/ |_| |_| |_|_|_| |_|\___/ \__\__,_|\__,_|_|  \___||___/     */



 /**************************/
 /*+ Liste des stratégies +*/
 /**************************/

/**
 * @brief
 * La liste des noms internes des stratégies.
 *
 * @attention
 * Le dernier nom ("STR_SIZE") est fictif: il sert uniquement à calculer automatiquement
 * le nombre de stratégies.
 *
 * @remark
 * Pour ajouter une nouvelle fonction de stratégie, vous devez procéder en quatre étapes:
 * 1. Écrire la fonction de stratégie.
 * 2. Ajouter un nouveau nom à la fin de cet "enum" (juste avant "STR_SIZE")
 * 3. Ajouter un pointeur vers la nouvelle fonction de stratégie à la fin du tableau "str_funs" dans game_strategies.c
 * 4. Ajouter le nom qui apparaîtra dans l'interface pour cette stratégie à la fin du tableau "str_names" dans game_strategies.c
 */
typedef enum {
    STR_NOPE,
    STR_RAND,
    STR_OPPO,
    STR_AGRE,
    STR_PBFS,
    STR_MOB,
    STR_SIZE,
}strategy;

/**
 * @brief Le tableau des fonctions de stratégie.
 */
extern void(*str_funs[]) (maze*, move, move*);

/**
 * @brief Le tableau des noms qui apparaissent dans l'interface pour chaque stratégie.
 */
extern const char* str_names[];

/****************************/
/*+ Fonctions de stratégie +*/
/****************************/

/**
 * @brief Dans cette stratégie, on ne déplace jamais les minotaures: ils restent immobiles.
 */
void minotaurs_still(maze* //!< Le labyrinthe.
    , move //!< Le dernier mouvement effectué par le joueur.
    , move* //!< Un tableau dont la taille est le nombre de minotaures. Il sert à retourner par pointeur les mouvements demandés pour les minotaures.
);

/**
 * @brief Dans cette stratégie, les mouvements des minotaures sont aléatoires.
 */
void minotaurs_random(maze* //!< Le labyrinthe.
    , move //!< Le dernier mouvement effectué par le joueur.
    , move* //!< Un tableau dont la taille est le nombre de minotaures. Il sert à retourner par pointeur les mouvements demandés pour les minotaures.
);

/**
 * @brief Dans cette stratégie, les minotaures effectuent l'inverse des mouvements du joueur (si il va à l'Est, ils se déplacent vers l'Ouest, etc).
 */
void minotaurs_reverse(maze* //!< Le labyrinthe.
    , move //!< Le dernier mouvement effectué par le joueur.
    , move* //!< Un tableau dont la taille est le nombre de minotaures. Il sert à retourner par pointeur les mouvements demandés pour les minotaures.
);

/**
 * @brief Dans cette stratégie, les minotaures prennent la direction qui les rapproche le plus du joueur.
 */
void minotaurs_closein(maze* //!< Le labyrinthe.
    , move //!< Le dernier mouvement effectué par le joueur.
    , move* //!< Un tableau dont la taille est le nombre de minotaures. Il sert à retourner par pointeur les mouvements demandés pour les minotaures.
);

/**
 * @brief Dans cette stratégie, les minotaures suivant toujours le chemin le plus court qui les mène au joueur.
 */
void minotaurs_bfs(maze* //!< Le labyrinthe.
    , move //!< Le dernier mouvement effectué par le joueur.
    , move* //!< Un tableau dont la taille est le nombre de minotaures. Il sert à retourner par pointeur les mouvements demandés pour les minotaures.
);

/**
 * @brief Dans cette stratégie, les minotaures suivant toujours le chemin le plus court qui les mène au centre
 */
void minotaurs_centre(maze* //!< Le labyrinthe.
    , move //!< Le dernier mouvement effectué par le joueur.
    , move* //!< Un tableau dont la taille est le nombre de minotaures. Il sert à retourner par pointeur les mouvements demandés pour les minotaures.
);

#endif
