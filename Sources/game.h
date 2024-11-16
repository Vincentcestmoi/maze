/**
 * @file game.h
 * @brief Définition et manipulation du plateau de jeu.
 *
 * @details Contient le type utilisé pour représenter un plateau jeu ainsi que les fonctions
 * dont on se servira pour le manipuler: création, implémentation d'un tour et libération.
 */

#ifndef GAME_H_
#define GAME_H_


#include <string.h>
#include "alloc.h"
#include "error.h"
#include "maze.h"
#include "maze_gen.h"
#include "maze_objects.h"
#include "maze_braiding.h"
#include "game_strategies.h"
#include "game_history.h"


 /*   ____           _   _                   _                        */
 /*  / ___| ___  ___| |_(_) ___  _ __     __| |_   _                  */
 /* | |  _ / _ \/ __| __| |/ _ \| '_ \   / _` | | | |                 */
 /* | |_| |  __/\__ \ |_| | (_) | | | | | (_| | |_| |                 */
 /*  \____|\___||___/\__|_|\___/|_| |_|  \__,_|\__,_|   _             */
 /*  _ __ | | __ _| |_ ___  __ _ _   _    __| | ___    (_) ___ _   _  */
 /* | '_ \| |/ _` | __/ _ \/ _` | | | |  / _` |/ _ \   | |/ _ \ | | | */
 /* | |_) | | (_| | ||  __/ (_| | |_| | | (_| |  __/   | |  __/ |_| | */
 /* | .__/|_|\__,_|\__\___|\__,_|\__,_|  \__,_|\___|  _/ |\___|\__,_| */
 /* |_|                                              |__/             */


 /*************/
 /*+ Le type +*/
 /*************/

/**
 * @brief Le type utilisé pour représenter le plateau jeu.
 */
typedef struct {

  maze* m; //!< Le labyrinthe.

  int score; //!< Score du joueur (initialisé à 0).

  int nbombs; //!< Nombre de bombes du joueur (initialisé à 0).

  int npolys; //!< Nombre de polys d'algo du joueur (initialisé à 0).

  bool player_alive; //!< Booléen indiquant si le joueur est vivant.

  cardinal player_dir; //!< La direction vers laquelle est tourné le joueur. 

  bool* minotaurs_alive; //!< Tableau de Booléens indiquant si chaque minotaure est vivant ou non (le nombre de minotaures est indiqué dans le labyrinthe).

  cardinal* minotaurs_dirs; //!< Tableau indiquant pour chaque minotaures la direction vers laquelle celui-ci est tourné.

  int nb_deadends; //!< Nombre de culs-de-sac dans le labyrinthe (doit être calculé avec la fonction prévue dans maze_braiding.h)

  int exits; //!< Le nombre de sorties sur le plateau.

  int turns; //!< Nombre de tours joués.

  history* log; //!< L'historique de la partie. Si l'historique n'est pas encore implémenté, ce pointeur doit être initialisé à NULL.
} game;


/****************************/
/*+ Création et libération +*/
/****************************/


/**
 * @brief Génération d'un nouveau jeu.
 *
 * @remark
 * La fonction prend à la fois un masque et des dimensions pour le plateau de jeu à construire.
 * Si les dimensions prises en entrée sont différentes de celles du masque, on se servira des
 * premières: le masque doit être redimensionné.
 *
 * @attention
 * Tous les champs de la structure doivent être initialisés, y compris ceux qui correspondent
 * à des parties du projet qui ne sont pas (encore) implémentées. En particulier, l'historique
 * DOIT être initialisé à NULL si cette fonctionnalité n'a pas (encore) été implémentée. Cela
 * permet à l'interface de détecter qu'il n'y pas d'historique (et donc de ne pas crasher).
 *
 * @return
 * Pointeur vers le plateau de jeu généré.
 */
game* create_newgame(int //!< Taille horizontale.
  , int //!< Taille verticale.
  , mask* //!< Masque à utiliser (si ce paramètre est NULL, on n'utilisera pas de masque).
  , generator //!< Le générateur de labyrinthe à utiliser.
  , objgenerator //!< Le générateur d'objets à utiliser.
  , int //!< Le nombre de minotaures à générer.
  , int //!< Le pourcentage de tressage à utiliser (élimination des culs-de-sac).
);

/**
 * @brief Libération d'un jeu.
 */
void free_game(game* //!< Le jeu.
);


/***********************************************/
/*+ Implémentation des attaques de minotaures +*/
/***********************************************/

/**
 * @brief
 * Teste si un minotaure se trouve à côté du joueur et tue le joueur si c'est le cas.
 *
 * @remark
 * Le second paramètre permet de retourner (par pointeur) la direction dans laquelle
 * se trouve le joueur par rapport au minotaure adjacent (si il y en a un).
 *
 * @return
 * Le numéro d'un minotaure à côté du joueur si il y en a un. Renvoie -1 sinon.
 */
int game_try_kill_player(game* //!< Le jeu.
  , cardinal* //!< Sert à retourner (par pointeur) la direction dans laquelle se trouve le joueur par rapport au minotaure quand on a trouvé un minotaure adjacent.
);


/***************************/
/*+ Traitement des objets +*/
/***************************/

/**
 * @brief Met à jour le plateau jeu en retirant/ajoutant (suivant le signe du
 * paramètre) plusieurs copies d'un objet donné au joueur.
 *
 * @remark Ne fait rien pour les sorties (qui ne sont pas des objets ramassables).
 * Pour les trésors, il faut ajouter au score du joueur le montant correspondant au
 * type de trésor (petit/moyen/grand).
 */
void game_consume_object(game* //!< Le jeu.
  , int//!< Le nombre de copies à ajouter/retirer.
  , object //!< L'objet à ajouter/retirer
);



/**
 * @brief Teste si un objet se trouve sur la cellule du joueur et modifie le plateau
 * en consommant cet objet (si cet objet est consommable).
 *
 * @remark Si l'objet est un trésor, une bombe ou un poly, il est retiré de la cellule
 * et les paramètres du joueur doivent être mis à jour avec "game_consume_object". Si
 * l'objet est une sortie, on ne modifie pas le jeu.
 *
 * @return L'objet présent sur la cellule du joueur.
 */
object game_treat_object(game* //!< Le jeu.
);


/**********************************************************/
/*+ Implémentation d'une demande de mouvement du joueur  +*/
/**********************************************************/

/**
 * @brief
 * Teste si un mouvement demandé par le joueur est réalisable et met à jour le plateau de jeu
 * en conséquence. Sont modifiés: la position du joueur, les positions des minotaures, les
 * trésors, le statut du joueur (a-t-il été tué par un minotaure ?), le nombre de tours joués et
 * l'historique.
 *
 * @attention
 * Un mouvement échoué ne compte pas comme un tour. Il y a deux cas dans lesquels le mouvement peut échouer:
 * - Le joueur est mort.
 * - Le joueur est bloqué par un mur. Dans ce second cas on tournera la joueur vers
 * la direction demandée sans le déplacer (le minotaures ne bougent pas non plus).
 *
 *
 * @remark
 * Si le le mouvement a pu être effectué, alors les paramètres suivants doivent être mis à jour:
 * - Les positions des minotaures. Leurs mouvements sont donnés par la stratégie passée en entrée.
 *   En particulier, il faudra vérfier pour chaque minotaure si le mouvement donné par la stratégie
 *   est possible (le minotaure n'est ni mort ni bloqué). Si le minotaure est bloqué, on le tourne
 *   vers la direction demandée sans le déplacer.
 * - Les trésors (en utilisant "game_treat_object").
 * - Le statut du joueur: on le tue si il y a un minotaure adjacent (propriété qu'on testera avec "game_try_kill_player").
 * - L'historique: on ajoute une entrée qui enregistre le tour.
 * - Le nombre de tours joués: on l'incrémente.
 *
 * @return
 * Un Booléen indiquant si la mouvement a pu être effectué.
 */
bool implement_game_move(game* //!< Le jeu.
  , move //!< Le mouvement demandé par le joueur.
  , strategy //!< La stratégie à utiliser pour déterminer les mouvements demandés par les minotaures.
);

/*******************************/
/*+ Implémentation des bombes +*/
/*******************************/

/**
 * @brief Utilise une bombe (si il y a une dans l'inventaire du joueur) et met à jour le plateau de jeu
 * en conséquence.
 *
 * @attention
 * Si l'action échoue, elle ne compte pas comme un tour.
 *
 * @remark Si il y a une bombe dans l'inventaire, on mettra à jour les paramètres suivants du jeu:
 * - Les murs: si il y a un mur qui mène à une cellule valide dans la direction du joueur, alors on le détruit.
 * - Le nombre de bombes du joueur: on le décrémente.
 * - Le statut du joueur: on le tue si le mur détruit mène à un minotaure (propriété qu'on testera avec "game_try_kill_player").
 * - L'historique: on ajoute une entrée qui enregistre le tour.
 * - Le nombre de tours joués: on l'incrémente.
 *
 * @return
 * Un Booléen indiquant si l'action a pu être effectuée.
 */
bool game_bomb_wall(game* //!< Le jeu.
);

/*************************************/
/*+ Implémentation des polys d'algo +*/
/*************************************/

/**
 * @brief
 * Utilise un poly d'algo (si c'est possible) et met à jour le plateau de jeu en conséquence.
 *
 * @attention
 * Si l'action échoue, elle ne compte pas comme un tour. Il y a deux façons d'échouer:
 * - Il n'y a pas de poly d'algo dans l'inventaire du joueur.
 * - Il n'y a aucun minotaure à portée.
 *
 * @remark Si l'action peut être effectuée, on mettra à jour les paramètres suivants du jeu:
 * - Les minotaures: on tue tous ceux qui sont à portée (la portée est passée en paramêtre).
 * - L'historique: on ajoute une entrée qui enregistre le tour.
 * - Le nombre de tours joués: on l'incrémente.
 *
 * @return
 * Un Booléen indiquant si l'action a pu être effectuée.
 */
bool game_kill_minotaurs(game* //!< Le jeu.
  , int d //!< La portée: un minotaure est tué si sa distance horizontale ou sa distance verticale est inférieure ou égale à d.
);


/*****************************/
/*+ Gestion de l'historique +*/
/*****************************/

/**
 * @brief Annulation d'une action à partir de l'historique.
 *
 * @attention
 * Cette action ne peut pas être effectuée si il n'y a pas d'entrée passée enregistrée dans
 * l'historique).
 *
 * @remark
 * Si l'action est effectuée, il faut décrémenter le nombre de tours joués.
 *
 * @return
 * Un Booléen indiquant si l'action a pu être effectuée.
 */
bool game_undo(game* //!< Le jeu.
);

/**
 * @brief Restauration d'une action à partir de l'historique.
 *
 * @attention
 * Cette action ne peut pas être effectuée si il n'y a pas d'entrée future enregistrée dans
 * l'historique).
 *
 * @remark
 * Si l'action est effectuée, il faut incrémenter le nombre de tours joués.
 *
 * @return
 * Un Booléen indiquant si l'action a pu être effectuée.
 */
bool game_redo(game* //!< Le jeu.
);






#endif // GAME_H_
