/**
 * @file gui_controller.h
 * @brief
 * Interface: gestion de la boucle de jeu. Ne PAS modifier.
 *
 * @attention
 * Cette partie est déjà implémentée. Il ne faut PAS la modifier.
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <time.h>
#include "gui_rendering.h"
#include "gui_menus.h"

 /*  ____                   _            _            _             */
 /* | __ )  ___  _   _  ___| | ___    __| | ___      | | ___ _   _  */
 /* |  _ \ / _ \| | | |/ __| |/ _ \  / _` |/ _ \  _  | |/ _ \ | | | */
 /* | |_) | (_) | |_| | (__| |  __/ | (_| |  __/ | |_| |  __/ |_| | */
 /* |____/ \___/ \__,_|\___|_|\___|  \__,_|\___|  \___/ \___|\__,_| */

 /*********************************/
 /*+ Flags pour la boucle de jeu **/
 /*********************************/


/**
 * @brief Flags pour la boucle de jeu.
 */
typedef enum {
    QUIT, HELP, PUSHED, ANIMATED, MOUSEDOWN, KEYDOWN, WIN, GENERATE, NB_FLAGS
} ctl_flags;

/**
 * @brief Tableau des flags.
 */
extern bool flags[NB_FLAGS];

/**
 * @brief Activation d'un flag.
 */
void set_flag(ctl_flags //!< Le flag
);

/**
 * @brief Désactivation d'un flag.
 */
void remove_flag(ctl_flags //!< Le flag
);

/**
 * @brief Désactivation de tous les flags.
 */
void reset_flags(void);

/**************************/
/*+ Intialisation du jeu **/
/**************************/

/**
 * @brief Teste si un jeu est bien construit (autant que faire se peut)
 */
bool ctl_game_ok(game*);

/**
 * @brief Libération des objets alloués faisant partie de l'interface.
 */
void ctl_free_gui(void);


/**
 * @brief Intialisation du jeu.
 */
game* ctl_init_game(bool //! Indique si il s'agit de la première génération ou non.
);


/*********************************/
/*+ Gestion du buffer d'édition **/
/*********************************/

/**
 * @brief Mise à jour des paramètre à partir du buffer après écriture.
 */
void ctl_update_from_buffer(game*);


/*********************************/
/*+ Prise en compte des actions **/
/*********************************/

/**
 * @brief Actions appliquées dès qu'on entre dans un nouveau menu.
 */
void ctl_enter_menu(game*);

/**
 * @brief Actions appliquées dès qu'on sort d'un menu.
 */
void ctl_exit_menu(void);


/**
 * @brief Préparation d'une action classique avant la modification du jeu.
 */
void ctl_init_action(game* //!< Le jeu.
    , move //!< Déplacement.
);

/**
 * @brief Préparation d'une action de restauration avant la modification du jeu.
 */
void ctl_init_redo(game* //!< Le jeu.
);

/**
 * @brief Préparation d'une action d'annulation avant la modification du jeu.
 */
void ctl_init_undo(game* //!< Le jeu.
);

/**
 * @brief Fin de la préparation d'une action après la modification du jeu.
 */
void ctl_update_action(game* //!< Le jeu.
);

/**
 * @brief Mise en place d'une action de déplacement.
 */
void ctl_move_action(game* //!< Le jeu.
    , move // Mouvement effectué
);

/**
 * @brief Mise en place d'une action de bombe.
 */
void ctl_bomb_action(game* //!< Le jeu.
);

/**
 * @brief Mise en place d'une action de poly d'algo.
 */
void ctl_poly_action(game* //!< Le jeu.
);

/**
 * @brief Mise en place d'une action de restauration.
 */
void ctl_redo_action(game* //!< Le jeu.
);

/**
 * @brief Mise en place d'une action d'annulation.
 */
void ctl_undo_action(game* //!< Le jeu.
);

/**
 * @brief Passage au masque suivant.
 */
void ctl_next_mask_action(void);

/**
 * @brief Passage au masque précédent.
 */
void ctl_previous_mask_action(void);

/**
 * @brief Teste si la souris pointe sur le labyrinthe.
 */
bool ctl_mouse_on_maze(SDL_Event* //!< L'Event pour avoir la position de la souris.
);

/**
 * @brief Teste si la souris pointe sur le menu.
 */
bool ctl_mouse_on_menu(SDL_Event* //!< L'Event pour avoir la position de la souris.
);

/**
 * @brief Récupération d'un bouton à partir de coordonnées sur l'écran.
 */
buttons ctl_get_button(int //!< Coordonnée horizontale.
    , int //!< Coordonnée verticale.
);

/**
 * @brief Traitement d'un déplacement de la souris.
 */
void ctl_mouse_motion(game* //!< Le jeu.
    , SDL_Event* //!< L'Event pour avoir la position de la souris
);

/**
 * @brief Traitement d'un clic de la souris.
 */
void ctl_mouse_click(game* //!< Le jeu.
    , SDL_Event* //!< L'Event pour avoir la position de la souris
);

/**
 * @brief Traitement du relachement d'un clic.
 */
void ctl_mouse_unclick(game* //!< Le jeu.
    , SDL_Event* //!< L'Event pour avoir la position de la souris
);

/**
 * @brief Traitement du clavier.
 */
void ctl_keyboard(game* //!< Le jeu.
    , SDL_Event* //!< L'Event pour avoir la touche utilisée.
);

/************************/
/*+ Gestion du solveur **/
/************************/

/**
 * @brief
 * Calcul d'un nouveau parcours pour rechercher un chemin simple.
 */
void ctl_sim_compute(game* //!< Le jeu.
);

/**
 * @brief
 * Suppression du parcours enregistré.
 */
void ctl_sim_delete(void);

/**
 * @brief Mise à jour du chemin rouge.
 */
void ctl_update_redpath(game* //!< Le jeu.
);

/**
 * @brief Mise à jour du chemin vert.
 */
void ctl_update_greenpath(game* //!< Le jeu.
);

/**
 * @brief Mise à jour du chemin bleu.
 */
void ctl_update_bluepath(game* //!< Le jeu.
);

/**
 * @brief Suppression du chemin rouge.
 */
void ctl_delete_redpath(void);

/**
 * @brief Suppression du chemin vert.
 */
void ctl_delete_greenpath(void);

/**
 * @brief Suppression du chemin bleu.
 */
void ctl_delete_bluepath(void);

/**
 * @brief Suppression de tous les chemins.
 */
void ctl_clear_paths(void);


/**
 * @brief Lancement de l'animation de parcours.
 */
void ctl_launch_searchpath(void);

/**
 * @brief Affichage d'un parcours complet.
 */
void ctl_print_searchpath(void);

/**
 * @brief Suppression de l'affichage d'un parcours.
 */
void ctl_hide_searchpath(void);


/**
 * @brief Calcul d'un chemin complexe.
 */
void ctl_compute_complex(game* //!< Le jeu.
);

/**
 * @brief Efface le chemin complexe.
 */
void ctl_delete_complex(void);

/**
 * @brief Changement de mode de l'affichage d'un chemin complexe
 */
void ctl_togglel_complex(game*);

/**
 * @brief Changement de mode de l'affichage d'un chemin complexe
 */
void ctl_toggler_complex(game*);

/**
 * @brief Changement du segment affiché dans un chemin complexe
 */
void ctl_next_complex(game*);

/**
 * @brief Changement du segment affiché dans un chemin complexe
 */
void ctl_previous_complex(game*);

/**
 * @brief Mise à jour du numéro de segment affiché
 */
void ctl_update_seg_num(game*, int);


/****************************/
/*+ Gestion des animations **/
/****************************/



/**
 * @brief Réinitialisation des animations.
 */
void ctl_anim_reset(void);

/**
 * @brief Animation des toons immobile (défaut).
 */
void ctl_anim_idle(game*);

/**
 * @brief Animation de déplacement
 */
void ctl_anim_walk(game*);

/**
 * @brief Animation d'une attaque de minotaure.
 */
void ctl_anim_kill(game*);

/**
 * @brief Animation d'une bombe.
 */
void ctl_anim_bomb(game*);

/**
 * @brief Animation d'un poly d'algo.
 */
void ctl_anim_poly(game*);

/**
 * @brief Animation d'une annulation de déplacement.
 */
void ctl_anim_uwalk(game*);

/**
 * @brief Animation d'une annulation de bombe.
 */
void ctl_anim_ubomb(game*);

/**
 * @brief Animation d'une annulation de poly d'algo.
 */
void ctl_anim_upoly(game*);

/**
 * @brief Animation d'une annulation d'attaque de minotaure (qui suit un déplacement).
 */
void ctl_anim_ukillwalk(game*);

/**
 * @brief Animation d'une annulation d'attaque de minotaure (qui suit une bombe).
 */
void ctl_anim_ukillbomb(game*);




/*******************/
/*+ Boucle de jeu **/
/*******************/

/**
 * @brief La boucle de jeu.
 */
void event_loop(void);






#endif // CONTROLLER_H_
