#ifndef MAZE_2_H
#define MAZE_2_H
#include "maze.h"

//fonctions utilisées mais non définies dans maze.h

/**
 *
 * @return
 * si la cellule est usable ie. si elle est valide, accessible et non masquée
 */
bool can_be_used(maze* //!< Le labyrinthe.
    , int //!< Le numéro de la cellule.
);
#endif //MAZE_2_H
