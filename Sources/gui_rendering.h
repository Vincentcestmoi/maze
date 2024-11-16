/**
 * @file gui_rendering.h
 * @brief
 * Interface: affichage du plateau de jeu. Ne PAS modifier.
 *
 * @attention
 * Cette partie est déjà implémentée. Il ne faut PAS la modifier.
 */

#ifndef RENDERING_H_
#define RENDERING_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_timer.h>
#include <dirent.h>

#include "game.h"
#include "solve_adv.h"
#include "maze.h"
#include "gui_init_graphic.h"
#include "gui_gamevars.h"
#include "gui_menus.h"

 /*     _     __  __ _      _                       */
 /*    / \   / _|/ _(_) ___| |__   __ _  __ _  ___  */
 /*   / _ \ | |_| |_| |/ __| '_ \ / _` |/ _` |/ _ \ */
 /*  / ___ \|  _|  _| | (__| | | | (_| | (_| |  __/ */
 /* /_/   \_\_| |_| |_|\___|_| |_|\__,_|\__, |\___| */
 /*                                     |___/       */


/*****************************/
/*+ Fonctions préliminaires **/
/*****************************/

/**
 * @brief Récupération de la position correspondant à une cellule sur l'écran (coin Nord-Ouest).
 */
void render_get_cellpos(int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
    , SDL_Rect* //!< Retour par pointeur de la position.
);

/**
 * @brief Retourne un Booléen indiquant si une cellule est hors de la zone à afficher.
 */
bool render_outofview(int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
);

/**********************/
/*+ Affichage du sol **/
/**********************/

/**
 * @brief Affichage d'une unique tile de sol.
 */
void render_print_floor_tile(SDL_Rect* //!< Position sur l'écran.
    , int //!< Entier utilisé pour sélectionner la tile à afficher parmi les 4 textures disponibles.
);

/**
 * @brief Affichage du sol pour une cellule.
 */
void render_print_floor_cell(int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
);

/**
 * @brief Affichage d'une cellule bloquée (ce sont les cellules non-accessibles à cause d'un masque).
 */
void render_print_block_cell(int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
);

/**
 * @brief Affichage de tout le sol.
 */
void render_whole_floor(game* //!< Le jeu.
);

/************************************/
/*+ Affichage des restes des toons **/
/************************************/

/**
 * @brief Affichage des restes des toons (sang et cendres).
 */
void render_print_remains(game* //!< Le jeu.
    , bool //!< Booléen indiquant si le joueur est vivant.
    , bool* //!< Tableau de Booléens indiquant pour chaque minotaur si celui-ci est vivant.
);

/************************/
/*+ Affichage des murs **/
/************************/

/**
 * @brief Affichage d'un mur Nord.
 */
void render_printhwall(SDL_Rect* //!< Position sur l'écran.
    , int //!< Entier utilisé pour sélectionner la tile à afficher parmi les 3 textures disponibles.
);

/**
 * @brief Retourne un Booléen indiquant si un mur doit être affiché ou non.
 */
bool render_should_print_wall(maze* //!< Le labyrinthe.
    , int //!< La cellule
    , cardinal //!< La direction dans laquelle se trouve le mur.
);

/**
 * @brief Affichage des murs Nord et Ouest dans une cellule.
 */
void render_print_walls_cell(maze* //!< Le labyrinthe.
    , int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
);

/**
 * @brief Affichage de tous les murs d'un labyrinthe.
 */
void render_all_walls(maze*//! Le labyrinthe
);

/***************************************/
/*+ Affichage des chemins et parcours **/
/***************************************/

/**
 * @brief Affichage d'un maillon de chemin.
 */
void render_print_link(game* //!< Le jeu.
    , int //!< La cellule.
    , cardinal //!< La direction.
);

/**
 * @brief Affichage d'un chemin entier.
 */
void render_print_path(game* //!< Le jeu.
    , sim_path* //!< Le chemin.
    , SDL_Color //!< La couleur à utiliser.
);

/**
 * @brief Affichage de tout un parcours.
 */
void render_search(game* //!< Le jeu.
);

/**
 * @brief Affichage du résumé d'un chemin complexe.
 */
void render_print_complex_path(game* //!< Le jeu.
    , cmp_search* //!< Le chemin.
);

/**************************/
/*+ Affichage des objets **/
/**************************/


/**
 * @brief Affichage d'un seul objet.
 */
void render_one_object(int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
    , object //!< L'objet à afficher.
);

/**
 * @brief Affichage de tous les objets d'un labyrinthe.
 */
void render_all_objects(maze* //!< Le labyrinthe
);


/*************************************************/
/*+ Affichage d'une scène (tout sauf les toons) **/
/*************************************************/

/**
 * @brief Affichage de toute une scène (sans les toons).
 */
void render_scene(game* //!< Le jeu.
);

/**
 * @brief Affichage de toute une scène (sans les toons). Permet de modifier le statut mort/vivant par
 * rapport à celui indiqué dans le jeu (utile lors des animations).
 */
void render_scene_unkill(game* //!< Le jeu.
    , bool //!< Booléen indiquant si le joueur doit être affiché vivant.
    , bool* //!< Tableau de Booléens indiquant pour chaque minotaure si celui-ci doit être affiché vivant.
);



/*************************/
/*+ Affichage d'un toon **/
/*************************/

/**
 * @brief Les deux types de toons.
 */
typedef enum {
    PLAYER,
    MINOTAUR,
} toon_type;

/**
 * @brief Affichage d'un toon immobile.
 */
void render_toon_idle(toon_type //!< Type de toon.
    , bool //!< Le toon est-il vivant?
    , cardinal //!< Dans quelle direction regarde le toon?
    , int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
);

/**
 * @brief Affichage d'un toon en déplacement.
 */
void render_toon_walking(toon_type //!< Type de toon.
    , move //!< Mouvement effectué par le toon.
    , move //!< Mouvement simultané effectué par le joueur.
    , int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
    , int //!< Frame de l'animation à afficher.
);

/**
 * @brief Affichage d'une attaque de minotaure.
 */
void render_minotaur_attacking(cardinal //!< Direction de l'attaque.
    , int //!< Coordonnée horizontale de la cellule dans le labyrinthe.
    , int //!< Coordonnée verticale de la cellule dans le labyrinthe.
    , int //!< Frame de l'animation à afficher.
);

/***********************************/
/*+ Affichage des toons immobiles **/
/***********************************/

/**
 * @brief Affichage de tout les toons immobiles.
 */
void render_all_toons_idle(game* //!< Le jeu.
);

/**
 * @brief Affichage de tout les minotaures immobiles.
 */
void render_all_minotaurs_idle(game* //!< Le jeu.
);

/**
 * @brief Affichage de tout le jeu entre deux tours
 */
void render_idle_game(game* //!< Le jeu.
    , bool //!< Booléen indiquant si le joueur a gagné (auquel cas un message doit être affiché).
);

/**
 * @brief Affichage d'un jeu vide (quand les fonctions de création d'un jeu ne sont pas écrites).
 */
void render_null_game(void);

/********************************/
/*+ Affichage d'un déplacement **/
/********************************/

/**
 * @brief Déplacement de l'écran (simule le mouvement du joueur).
 */
void walk_shift_screen(cardinal //!< La direction dans laquelle on fait le déplacement.
);

/**
 * @brief Affichage de tous les toons en déplacement.
 */
void render_all_toons_move(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
);

/**
 * @brief Affichage complet d'une frame de déplacement (plateau + toons).
 */
void render_walk_animation(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse de déplacement.
);

/**
 * @brief Affichage de tous les toons lors d'une annulation de déplacement.
 */
void render_all_toons_move_back(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
);

/**
 * @brief Affichage complet d'une frame d'annulation de déplacement (plateau + toons).
 */
void render_undo_walk_animation(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse de déplacement.
);

/******************************************/
/*+ Affichage d'une attaque de minotaure **/
/******************************************/

/**
 * @brief Affichage de toute la scène d'une attaque de minotaure.
 */
void render_kill_animation(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse.
);

/**
 * @brief Affichage de toute la scène d'une annulation d'attaque de minotaure.
 */
void render_undo_kill_animation(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse.
);

/********************************/
/*+ Affichage d'un poly d'algo **/
/********************************/

/**
 * @brief Affichage de toute la scène de l'utilisation d'un poly d'algo.
 */
void render_holy_poly(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse.
);

/**
 * @brief Affichage de toute la scène de l'annulation de l'utilisation d'un poly d'algo.
 */
void render_undo_holy_poly(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse.
);


/***************************/
/*+ Affichage d'une bombe **/
/***************************/

/**
 * @brief Affichage de toute la scène de l'utilisation d'une bombe.
 */
void render_bombing(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse.
);

/**
 * @brief Affichage de toute la scène de l'annulation de l'utilisation d'une bombe.
 */
void render_undo_bombing(game* //!< Le jeu.
    , int //!< La frame d'animation à afficher.
    , int //!< Vitesse.
);







#endif // RENDERING_H_
