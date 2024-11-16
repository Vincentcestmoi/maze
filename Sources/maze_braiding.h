/**
 * @file maze_braiding.h
 * @brief
 * Tressage des labyrinthes.
 *
 * @details
 * Contient les fonctions utilisées pour détecter et supprimer les culs-de-sacs
 * dans un labyrinthe.
 */

#ifndef MAZEBRAID_H_
#define MAZEBRAID_H_

#include "maze.h"

 /*  _____                                         _            */
 /* |_   _| __ ___  ___ ___  __ _  __ _  ___    __| | ___  ___  */
 /*   | || '__/ _ \/ __/ __|/ _` |/ _` |/ _ \  / _` |/ _ \/ __| */
 /*   | || | |  __/\__ \__ \ (_| | (_| |  __/ | (_| |  __/\__ \ */
 /*   |_||_|  \___||___/___/\__,_|\__, |\___|  \__,_|\___||___/ */
 /*  _       _                _   |___/   _                     */
 /* | | __ _| |__  _   _ _ __(_)_ __ | |_| |__   ___  ___       */
 /* | |/ _` | '_ \| | | | '__| | '_ \| __| '_ \ / _ \/ __|      */
 /* | | (_| | |_) | |_| | |  | | | | | |_| | | |  __/\__ \      */
 /* |_|\__,_|_.__/ \__, |_|  |_|_| |_|\__|_| |_|\___||___/      */
 /*                |___/                                        */



 /********************************/
 /*+ Récupération d'information +*/
 /********************************/

/**
 * @brief
 * Teste si une cellule est un cul-de-sac.
 *
 * @remark
 * Un cul-de-sac est une cellule entourée de trois murs.
 *
 * @return
 * Un Booléen indiquant si la cellule est un cul-de-sac.
 */
bool is_dead_end(maze* //!< Le labyrinthe.
    , int//!< La cellule à tester.
);

/**
 * @brief
 * Calcule le nombre de culs-de-sac dans un labyrinthe.
 *
 * @return
 * Le nombre de culs-de-sac.
 */
int count_dead_ends(maze* //!< Le labyrinthe.
);


/*********************************/
/*+ Suppression des culs-de-sac +*/
/*********************************/


/**
 * @brief Si une cellule est un cul-de-sac et que la fonction réussit un jet aléatoire, elle essaye
 * de retirer cette propriété à la cellule en détruisant un de ses trois murs.
 *
 * @attention
 * Certains murs ne peuvent pas tous être détruits. Un mur peut être détruit si il mène à une cellule valide
 * (en particulier non masquée) et satisfait l'une des deux conditions suivantes:
 * - C'est le mur opposé à l'unique côté sans mur de la cellule.
 * - Il est orthogonal à l'unique côté sans mur de la cellule et la cellule adjacente au côté sans mur possède un mur
 *   qui prolonge celui qu'on veut supprimer.
 *
 * @remark
 * Le mur à supprimer (parmi ceux qui sont destructibles) est choisi aléatoirement. On doit néanmoins donner la
 * priorité aux murs qui mènent vers une cellule qui est elle-même un cul-de-sac (si il y en a une).
 */
void remove_one_dead_end(maze*//!< Le labyrinthe
    , int cell //!< La cellule
    , int odds //!< Le pourcentage de chance de réussir le jet aléatoire
);

/**
 * @brief Applique la fonction remove_one_dead_end à toutes les cellules d'un labyrinthe
 */
void braid_maze(maze* m//!< Le labyrinthe
    , int odds//!< Le pourcentage de chance de réussir un jet aléatoire
);


#endif
