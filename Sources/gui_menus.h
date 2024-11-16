/**
 * @file gui_menus.h
 * @brief Interface: affichage des menus. Ne PAS modifier.
 *
 * @attention
 * Cette partie est déjà implémentée. Il ne faut PAS la modifier.
 */

#ifndef MENUS_H_
#define MENUS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_timer.h>
#include <dirent.h>

#include "game.h"
#include "maze.h"
#include "gui_init_graphic.h"
#include "gui_gamevars.h"

 /*  __  __                       */
 /* |  \/  | ___ _ __  _   _ ___  */
 /* | |\/| |/ _ \ '_ \| | | / __| */
 /* | |  | |  __/ | | | |_| \__ \ */
 /* |_|  |_|\___|_| |_|\__,_|___/ */


 /************************/
 /*+ Affichage de texte **/
 /************************/

/**
 * @brief Affichage d'un texte (point d'ancrage Nord-Ouest).
 */
int render_text(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , const char* //!< Chaîne de caractères.
    , TTF_Font* //!< Font à utiliser.
);

/**
 * @brief Affichage d'un texte (point d'ancrage Nord-Centre).
 */
int render_text_centered(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , const char* //!< Chaîne de caractères.
    , TTF_Font* //!< Font à utiliser.
);

/**
 * @brief Affichage d'un texte lors de l'utilisation d'un buffer de saisie.
 */
void print_text_buffer_center(void);

/****************************/
/*+ Affichages des boutons **/
/****************************/

/**
 * @brief Taille d'un bouton.
 */
#define BUTTON_SIZE 44;

 /**
  * @brief Affichage du bouton bombe.
  */
void render_bomb_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage du bouton poly.
 */
void render_poly_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage du bouton sortie.
 */
void render_exit_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage du bouton annuler.
 */
void render_undo_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage du bouton refaire.
 */
void render_redo_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage du bouton aide.
 */
void render_help_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage du bouton centrage de la caméra.
 */
void render_came_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage des boutons de déplacement.
 */
void render_move_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée vertical.
    , bool //!< Le bouton est-il enfoncé?
    , move //!< Le déplacement voulu.
);


/***************************/
/*+ Affichages des cadres **/
/***************************/

/**
 * @brief Affichage d'un tube.
 */
void render_scale_tube(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , int //!< Largeur.
    , int //!< Remplissage actuel.
    , int //!< Remplissage maximal.
    , unsigned char //!< Niveau de zoom.
    , bool //!< Le bouton gauche est-il enfoncé?
    , bool //!< Le bouton droit est-il enfoncé?
);


/**
 * @brief Affichage d'un sélecteur.
 */
void render_selector(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , int //!< Largeur.
    , const char* //!< Le message à afficher.
    , TTF_Font* //!< La font à utiliser.
    , bool //!< Le bouton gauche est-il enfoncé?
    , bool //!< Le bouton droit est-il enfoncé?
);


/**
 * @brief Affichage d'une boite de saisie de texte.
 */
void render_selector_edit(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , int //!< Largeur.
    , const char* //!< Le message à afficher.
    , TTF_Font* //!< La font à utiliser.
    , bool //!< Le bouton droit est-il enfoncé?
);

/**
 * @brief Affichage d'un indicateur coloré.
 */
void render_color_indicator(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , int //!< Largeur.
    , const char* //!< Le message à afficher.
    , int //!< La couleur à utiliser (1: rouge, 2: vert, 3: bleu, autre: gris)
);


/**
 * @brief Affichage d'un gros bouton centré.
 */
void render_bigbutton(int //!< Coordonnée verticale.
    , const char* //!< Label.
    , TTF_Font* //!< La font à utiliser.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage d'un petit bouton.
 */
void render_smallbutton(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , int //!< Largeur.
    , const char* //!< Label.
    , bool //!< Le bouton est-il enfoncé?
);

/**
 * @brief Affichage d'un cadre.
 */
void render_frame(int x //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
    , int //!< Largeur.
    , int //!< Hauteur.
    , const char* //!< Titre.
);

/********************************/
/*+ Affichage des informations **/
/********************************/

/**
 * @brief Message indiquant qu'aucun jeu n'a été généré.
 */
void render_gamenull(void);

/**
 * @brief Message indiquant la victoire.
 */
void render_victory(void);

/**
 * @brief Informations communes à tous les menus.
 */
void render_informations(game* //!< Le jeu.
);

/**
 * @brief Informations spécifiques au menu de jeu.
 */
void render_play_informations(game* //!< Le jeu.
);

/**
 * @brief Informations spécifiques au menu de génération.
 */
void render_gen_informations(game* //!< Le jeu.
);



/**
 * @brief Informations spécifiques au menu du solveur simple.
 */
void render_sim_informations(game* //!< Le jeu.
);

/**
 * @brief Informations spécifiques au menu du solveur avancé.
 */
void render_cmp_searchs(game* //!< Le jeu.
);

/**
 * @brief Affichage du menu complet.
 */
void render_all_informations(game* //!< Le jeu.
);






#endif
