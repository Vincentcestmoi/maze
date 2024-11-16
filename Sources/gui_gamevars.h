/**
 * @file gui_gamevars.h
 * @brief
 * Interface: variables globales. Ne PAS modifier.
 *
 * @attention
 * Cette partie est déjà implémentée. Il ne faut PAS la modifier.
 */

#ifndef GAMEVARS_H_
#define GAMEVARS_H_

 /* __     __         _       _     _                                         */
 /* \ \   / /_ _ _ __(_) __ _| |__ | | ___  ___                               */
 /*  \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|                              */
 /*   \ V / (_| | |  | | (_| | |_) | |  __/\__ \                              */
 /*    \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/                              */
 /*   ____ _       _           _                 _             _              */
 /*  / ___| | ___ | |__   __ _| | ___  ___    __| |_   _      | | ___ _   _   */
 /* | |  _| |/ _ \| '_ \ / _` | |/ _ \/ __|  / _` | | | |  _  | |/ _ \ | | |  */
 /* | |_| | | (_) | |_) | (_| | |  __/\__ \ | (_| | |_| | | |_| |  __/ |_| |  */
 /*  \____|_|\___/|_.__/ \__,_|_|\___||___/  \__,_|\__,_|  \___/ \___|\__,_|  */


#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <SDL2/SDL_timer.h>
#include "game.h"
#include "solve_adv.h"
#include "gui_textbuf.h"



/******************************/
/*+ Dimensions de la fenêtre **/
/******************************/

/**
 * @brief Taille horizontale de l'écran.
 */
extern int var_screen_width;

/**
 * @brief Taille verticale de l'écran.
 */
extern int var_screen_height;

/**
 * @brief Taille horizontale de la partie labyrinthe
 */
extern int var_maze_width;

/**
 * @brief Taille verticale de la partie labyrinthe
 */
extern int var_maze_height;

/**
 * @brief Taille horizontale de la partie menu.
 */
extern int var_info_width;
/**
 * @brief Taille verticale de la partie menu.
 */
extern int var_info_height;

/**
 * @brief Point médian de la partie menu.
 */
extern int var_info_mid;

/***************************/
/*+ Contrôle de la Souris **/
/***************************/

/**
 * @brief Sauvegarde de la coordonnée horizontale de la souris.
 */
extern int var_mousePosx;

/**
 * @brief Sauvegarde de la coordonnée verticale de la souris.
 */
extern int var_mousePosy;

/**
 * @brief Mémorise si le bouton gauche de la souris en enfoncé.
 */
extern bool var_leftMouseButtonDown;


/**************************************/
/*+ Contrôle du zoom et du Scrolling **/
/**************************************/

/**
 * @brief Multiplicateur actuel du zoom.
 */
extern double var_render_zoom;

/**
 * @brief Niveau de zoom actuel.
 */
extern int var_render_zoom_level;

/**
 * @brief La taille d'une tile au niveau de zoom actuel.
 */
extern int var_tile_size;

/**
 * @brief Décalage horizontal du scrolling.
 */
extern int var_scrollOffsetx;

/**
 * @brief Décalage vertical du scrolling.
 */
extern int var_scrollOffsety;

/**
 * @brief Coordonnée horizontale de la première cellule dans la zone d'affichage (dans le labyrinthe).
 */
extern int var_startx;

/**
 * @brief Coordonnée verticale de la première cellule dans la zone d'affichage (dans le labyrinthe).
 */
extern int var_starty;

/**
 * @brief Coordonnée horizontale de la dernière cellule dans la zone d'affichage (dans le labyrinthe).
 */
extern int var_endx;

/**
 * @brief Coordonnée verticale de la dernière cellule dans la zone d'affichage (dans le labyrinthe).
 */
extern int var_endy;

/**
 * @brief Coordonnée horizontale de la première cellule dans la zone d'affichage (sur l'écran).
 */
extern int var_topleftx;

/**
 * @brief Coordonnée verticale de la première cellule dans la zone d'affichage (sur l'écran).
 */
extern int var_toplefty;

/**
 * @brief Mise à jour des variables qui délimitent la zone à afficher.
 */
void scroll_update_borders(void);

/**
 * @brief Diminution du zoom.
 */
void decrease_zoom(void);

/**
 * @brief Augmentation du zoom.
 */
void increase_zoom(void);

/**
 * @brief Scrolling manuel.
 */
void scroll_manual(int, int);

/**
 * @brief Scrolling à la souris.
 */
void scroll_with_mouse(int, int);

/**
 * @brief Centrage de l'écran sur une cellule.
 */
void scroll_to_cell(game*, int);

/**
 * @brief Centrage de l'écran sur le joueur.
 */
void scroll_to_player(game*);



/****************************/
/*+ Contrôle de la vitesse **/
/****************************/


/**
 * @brief Vitesse actuelle.
 */
extern int var_render_speed;

/**
 * @brief Niveau de la vitesse actuelle.
 */
extern int var_render_speed_level;

/**
 * @brief Augmentation de la vitesse.
 */
void increase_speed(void);

/**
 * @brief Diminution de la vitesse.
 */
void decrease_speed(void);



/**************************/
/*+ Les différents menus **/
/**************************/

/**
 * @brief Noms internes des différents menus.
 */
typedef enum {
    MENU_PLAY, //!< Jeu.
    MENU_GENE, //!< Génération.
    MENU_SOSI, //!< Solveur simple.
    MENU_SOAD, //!< Solveur avancé.
    MENU_SIZE
} menu_mode;

/**
 * @brief Variable globale qui enregistre le menu actuel.
 */
extern menu_mode var_menu;







/*************************/
/*+ Lecture des masques **/
/*************************/

/**
 * @brief Tableau de la liste des fichiers de masques.
 */
extern struct dirent** var_maskstab;

/**
 * @brief Taille du tableau de la liste des fichiers de masques.
 */
extern int var_masktabsize;

/**
 * @brief Index du masque sélectionné dans le tableau de la liste des fichiers de masques. (-1 si aucun masque n'est utilisé)
 */
extern int var_maskindex;

/**
 * @brief Initialisation du tableau de la liste des fichiers de masques.
 */
void var_init_maskstab(void);

/**
 * @brief Libération du tableau de la liste des fichiers de masques.
 */
void var_free_maskstab(void);


/********************************/
/*+ Entrée de texte au clavier **/
/********************************/

/**
 * @brief Les noms internes des différents textes qu'on peut saisir dans un buffer.
 */
typedef enum {
    ED_X, //!< Taille horizontale.
    ED_Y, //!< Taille verticale.
    ED_MINO, //!< Nombre de minotaures.
    ED_BRAID, //!< Taux de tressage.
    ED_BFS, //!< Numéro d'objet à rechercher dans un parcours BFS/DFS.
    ED_SEG, //!< Segment à afficher dans un parcours complexe
    ED_NONE, //!< Buffer non-utilisé actuellement.
} edit_mode;

/**
 * @brief Variable globale qui enregistre ce pour quoi le buffer est actuellement utilisé.
 */
extern edit_mode var_edit_mode;

/**
 * @brief Variable globale utilisé pour le buffer (NULL si le buffer est inutilisé).
 */
extern text_buffer* var_editbuffer;

/****************/
/*+ Animations **/
/****************/

/**
 * @brief Frame de l'animation à afficher.
 */
extern int var_frame;

/**
 * @brief Les noms internes des différentes animations.
 */
typedef enum {
    AN_IDLE = 0, //!< Immobile (par défaut).
    AN_WALK, //!< Déplacement.
    AN_KILL, //!< Attaque d'un minotaure.
    AN_BOMB, //!< Bombe.
    AN_POLY, //!< Poly d'algo.
    AN_UWALK, //!< Annulation d'un déplacement.
    AN_UKILLB, //!< Annulation d'une attaque de minotaure (après une bombe).
    AN_UKILLW, //!< Annulation d'une attaque de minotaure (après un déplacement).
    AN_UBOMB, //!< Annulation d'une bombe.
    AN_UPOLY, //!< Annulation d'un poly d'algo.
    AN_NULL,
} anim_mode;

/**
 * @brief Variable globale qui enregistre l'animation courante.
 */
extern anim_mode var_crender;

/**
 * @brief Position du joueur pendant un mouvement.
 */
extern int an_player_pos;

/**
 * @brief Positions des minotaures pendant un mouvement.
 */
extern int* an_minotaurs_pos;

/**
 * @brief Direction de déplacement du joueur pendant un mouvement.
 */
extern move an_player_move;

/**
 * @brief Directions de déplacement des minotaures pendant un mouvement.
 */
extern move* an_minotaurs_move;

/**
 * @brief Objet récupéré à la suite d'un mouvement.
 */
extern object an_obj_taken;

/**
 * @brief Booléen indiquant si le joueur a été tué lors d'une action.
 */
extern bool an_killed;

/**
 * @brief Minotaure ayant tué le joueur.
 */
extern int an_killer;

/**
 * @brief Direction du joueur par rapport au minotaure qui l'a tué.
 */
extern cardinal an_killdir;

/**
 * @brief Minotaures encore vivants après l'utilisation d'un poly d'algo.
 */
extern bool* an_minotaurs_alive;

/**
 * @brief Direction vers laquelle une bombe a été posée.
 */
extern cardinal an_bombdir;

/**
 * @brief Est-ce qu'il y avait un mur dans la direction où la bombe a été posée ?
 */
extern bool an_hadwall;

/**
 * @brief Est-ce qu'un mur a été détruit suite à une bombe?
 */
extern bool an_destroyed;

/******************************/
/*+ Stratégie des minotaures **/
/******************************/

/**
 * @brief Stratégie actuelle des minotaures.
 */
extern strategy var_mino_strat;

/**********************************/
/*+ Variables pour la génération **/
/**********************************/

/**
 * @brief Générateur de labyrinthe selectionné.
 */
extern generator gen_mode;

/**
 * @brief Générateur d'objets selectionné.
 */
extern objgenerator obj_mode;

/**
 * @brief Taille horizontale selectionnée.
 */
extern int gen_hsize;

/**
 * @brief Taille verticale selectionnée.
 */
extern int gen_vsize;

/**
 * @brief Nombre de minotaures selectionné.
 */
extern int gen_nbminotaurs;

/**
 * @brief Taux de tressage selectionné.
 */
extern int gen_braiding;

/**
 * @brief Nom du masque selectionné.
 */
extern char gen_maskname[];

/**
 * @brief Masque utilisé lors de la toutes première génération.
 */
extern char* gen_initialmask;

/*****************/
/*+ Les boutons **/
/*****************/


/**
 * @brief Noms internes des boutons de l'interface.
 */
typedef enum {
    BT_UP,
    BT_RIGHT,
    BT_DOWN,
    BT_LEFT,
    BT_WAIT,
    BT_BOMB,
    BT_POLY,
    BT_EXIT,
    BT_UNDO,
    BT_REDO,
    BT_CAMERA,
    BT_HELP,
    BT_GEN,

    BT_LSPEED,
    BT_RSPEED,
    BT_LZOOM,
    BT_RZOOM,

    BT_LMODE,
    BT_RMODE,

    BT_LSTRAT,
    BT_RSTRAT,

    BT_LMASK,
    BT_RMASK,
    BT_LALGO,
    BT_RALGO,
    BT_LOBJ,
    BT_ROBJ,
    BT_BRAID,
    BT_X,
    BT_Y,
    BT_MINO,

    BT_SEARCHL,
    BT_SEARCHR,
    BT_BFSL,
    BT_BFSR,
    BT_BFS_NUM,
    BT_BLUE1,
    BT_GREEN1,
    BT_RED1,
    BT_BLUE2,
    BT_GREEN2,
    BT_RED2,
    BT_DTR,
    BT_ATR,
    BT_RTR,



    BT_SIML,
    BT_SIMR,
    BT_SIMGL,
    BT_SIMGR,
    BT_SIMMINOS,
    BT_HEUL,
    BT_HEUR,
    BT_SIMCOMP,
    BT_SIMDELE,

    BT_ADVL,
    BT_ADVR,
    BT_ADVCOMP,
    BT_ADVDELE,
    BT_ADVMODEL,
    BT_ADVMODER,
    BT_ADVSEG,
    BT_ADVN,
    BT_ADVP,






    BT_SIZE,



    BT_NONE,
} buttons;

/**
 * @brief Compteur pour l'animation de pression d'un bouton.
 */
extern int var_button_countdown;

/**
 * @brief Tableau qui indique les boutons actuellement pressé (pour les animations).
 */
extern bool var_buttons_flags[BT_SIZE];

/**
 * @brief Réinitialistion des boutons.
 */
void var_reset_buttons(void);

/**
 * @brief Appuie sur un bouton.
 */
void var_push_button(buttons);

/**
 * @brief Relache un bouton.
 */
void var_release_button(buttons);

/*********************************/
/*+ Variables le solveur simple **/
/*********************************/

/**
 * @brief L'algorithme à utiliser pour le calcul d'un chemin simple.
 */
extern sim_algorithm var_sim_algorithm;

/**
 * @brief Le type d'objet à rechercher pour le calcul d'un chemin simple.
 */
extern sim_goal var_sim_goal;

/**
 * @brief L'algorithme à utiliser pour le calcul soit-il prendre en compte les minotaures?
 */
extern bool var_sim_minos;

/**
 * @brief Enregistrement d'un parcours (BFS, DFS ou A*).
 */
extern sim_search* var_sim_search;

/**
 * @brief L'heuristique selectionnée pour l'algorithme A*.
 */
extern heuristic var_heuristic;

/**
 * @brief Enregistrement des chemins à afficher (jusqu'à trois chemins en même temps, rouge, bleu, vert)
 */
extern sim_path* var_path_red;
extern sim_path* var_path_blue;
extern sim_path* var_path_green;

/**
 * @brief Booléen indiquant si on doit afficher un parcours.
 */
extern bool var_display_search;


/**
* @brief Case du tableau dynamique jusqu'à laquelle le parcours doit temporairement être affiché (spécifique à l'animation).
*/
extern int var_search_step;


/**
 * @brief La fonction selectionnée pour la génération de chemin complexe.
 */
extern cmp_algorithm var_cmp_algorithm;

/**
 * @brief Enregistrement d'un chemin complexe.
 */
extern cmp_search* var_adv_path;

/**
 * @brief Mode d'affichage d'un chemin complexe.
 */
extern unsigned char var_adv_mode;

/**
 * @brief Numéro du segment à afficher dans un chemin complexe.
 */
extern int var_adv_num;

#endif
