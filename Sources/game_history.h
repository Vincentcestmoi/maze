/**
 * @file game_history.h
 * @brief
 * Gestion de l'historique.
 *
 * @details
 * Contient toutes les fonctions liées à la gestion de l'historique d'une partie.
 *
 * Le type utilisé pour coder une entrée de l'historique et les primitives utilisées pour
 * le manipuler sont imposés. En revanche la définition de la structure qu'on va utiliser
 * pour représenter l'historique lui-même est laissée libre. C'est à vous de décider de la
 * façon de l'implémenter (il faut compléter la définition de la structure dans le fichier
 * game_history.c).
 *
 * Votre implémentation de l'historique doit satisfaire la spécification suivante. L'historique
 * contient une séquence d'entrées correspondant chacune à une action effectuée par le joueur
 * (c'est-à-dire que les entrées sont des objet du type "turn" défini dans le fichier). Ces entrées
 * sont stockées dans l'ordre où les actions ont été effectuées par le joueur.
 *
 * L'historique a un nombre maximal d'entrées (codé par le nombre "HISTORY_MAX" défini dans ce
 * fichier). Lorsque ce nombre est atteint, on devra supprimer l'entrée la plus ancienne lorsque
 * l'on veut en insérer une nouvelle pour "faire de la place".
 *
 * Lorsque l'historique est disponible, il y a trois types d'actions disponibles pour le
 * joueur:
 * 1. Effectuer une action classique (déplacement, bombe ou poly). Dans ce cas l'action
 *    effectuée doit être enregistrée dans l'historique en y ajoutant une nouvelle entrée.
 * 2. Annuler la dernière action enregistrée dans l'historique (si il y en a une).
 * 3. Restaurer une action précédemment annulée (si il y en a une).
 *
 * La séquence d'entrées enregistrées dans l'historique est donc divisée en deux parties (toutes
 * les deux possiblement vides):
 * 1. Les entrées correspondant à des tours passés qu'on peut annuler.
 * 2. Les entrées correspondant à des tours futurs. Ce sont des tours précédemment annulés
 *    qu'on peut maintenant restaurer.
 *
 * @remark
 * Lorsqu'un tour est annulé, son entrée est déplacée de la catégorie 1 à la catégorie 2.
 * Réciproquement, lorsqu'un tour est restauré, son entrée est déplacée de la catégorie 2
 * à la catégorie 1.
 *
 * @attention
 * Lorsque le joueur effectue une action classique (indépendante de l'historique) on devra
 * supprimer toutes les entrées correspondant à des tours futurs. Après cette action, il ne
 * restera plus que les entrées correspondant à des tours passés (auxquelles une nouvelle
 * entrée qui enregistre l'action effectuée par le joueur vient d'être ajoutée).
 */

#ifndef GAMEHISTORY_H_
#define GAMEHISTORY_H_

 /*  _   _ _     _             _                        _ _                   */
 /* | | | (_)___| |_ ___  _ __(_) __ _ _   _  ___    __| ( )_   _ _ __   ___  */
 /* | |_| | / __| __/ _ \| '__| |/ _` | | | |/ _ \  / _` |/| | | | '_ \ / _ \ */
 /* |  _  | \__ \ || (_) | |  | | (_| | |_| |  __/ | (_| | | |_| | | | |  __/ */
 /* |_| |_|_|___/\__\___/|_|  |_|\__, |\__,_|\___|  \__,_|  \__,_|_| |_|\___| */
 /*  _ __   __ _ _ __| |_(_) ___    |_|                                       */
 /* | '_ \ / _` | '__| __| |/ _ \                                             */
 /* | |_) | (_| | |  | |_| |  __/                                             */
 /* | .__/ \__,_|_|   \__|_|\___|                                             */
 /* |_|                                                                       */

#include "maze.h"

/**************************************************/
/*+ Type utilisé pour une entrée de l'historique +*/
/**************************************************/

/**
 * @brief Les trois types de tours possibles.
 */
typedef enum {
    T_MOVE, //!< Déplacement.
    T_BOMB, //!< Utilisation d'une bombe.
    T_POLY, //!< Utilisation d'un poly d'algo.
} t_type;

/**
 * @brief Le type utilisé pour coder un tour de déplacement.
 */
typedef struct {
    object obj; //!< L'objet récupéré par le joueur lors de ce déplacement (NONE si aucun objet n'a été récupéré).
    move playermove; //!< Le déplacement effectué par le joueur
    move* minomoves; //!< Un tableau cotenant les déplacements effectués par les minotaures
    int killer; //!< Si le joueur a été tué à l'issue du tour, indique le numéro du meurtrier. Vaut -1 si le joueur a survécu au tour.
    cardinal dirkill; //!< Si le joueur a été tué à l'issue du tour, indique la direction dans laquelle se trouvait le joueur par rapport au meurtrier.
} t_move;

/**
 * @brief Le type utilisé pour coder un tour de bombe.
 */
typedef struct {
    cardinal bombdir; //!< La direction dans laquelle la bombe a été posée
    bool destroyed; //!< Booléen indiquant si un mur a effectivement été détruit par la bombe.
    int killer; //!< Si le joueur a été tué à l'issue du tour, indique le numéro du meurtrier. Vaut -1 si le joueur a survécu au tour.
    cardinal dirkill; //!< Si le joueur a été tué à l'issue du tour, indique la direction dans laquelle se trouvait le joueur par rapport au meurtrier.
} t_bomb;

/**
 * @brief Le type utilisé pour coder un tour quelconque.
 */
typedef struct {
    t_type type; //!< Le type de tour de cette entrée.
    union {
        t_move* tmove; //!< Utilisé si il s'agit d'un tour de type déplacement.
        t_bomb* tbomb; //!< Utilisé si il s'agit d'un tour de type bombe.
        bool* minokilled; //!< Utilisé si il s'agit d'un tour de type poly. Tableau de Booléens (indexé par les minotaures). Indique les minotaures qui ont été tués par le poly lors du tour.
    };
} turn;

/***************************/
/*+ L'historique lui-même +*/
/***************************/

/**
 * @brief
 * Le nombre maximal d'entrées que peut contenir l'historique.
 */
#define HISTORY_MAX 100

 /**
  * @brief
  * Le type utilisé pour représenter l'historique (la vraie définition est à compléter dans le fichier .c)
*/
typedef struct history history;

/**
 * @brief
 * Création d'un historique vide.
 *
 * @return
 * L'historique créé.
 */
history* create_history(void);

/**
 * @brief
 * Libération de tout l'historique.
 */
void free_history(history* //< L'historique.
);

/**
* @brief Insertion d'une nouvelle entrée dans l'historique.
*
* @details
* C'est cette fonction qui est utilisée pour enregistrer une action effectuée
* par le joueur dans l'historique. Elle devra supprimer TOUTES les entrées de
* tours futurs. Après l'application de cette fonction:
* - Il n'y a plus de tours futurs.
* - Le tour passé le plus récent correspond à l'entrée qu'on vient d'ajouter.
* - Les tours passés qui étaient déjà présents ont été décalés d'un cran dans
*   le passé.
*
* @attention
* L'entrée à ajouter n'est pas passée par un pointeur.
*/
void add_entry_history(turn //< L'entrée à ajouter (on remarquera que ce paramètre n'est PAS un pointeur).
    , history* //< L'historique à modifier.
);

/**
 * @brief Calcule le nombre d'entrées futures dans l'historique.
 *
 * @return Le nombre d'entrées futures.
 */
int sizenext_history(history* //!< L'historique.
);

/**
 * @brief Calcule le nombre d'entrées passées dans l'historique.
 *
 * @return Le nombre d'entrées passées.
 */
int sizeprev_history(history* //!< L'historique.
);

/**
 * @brief Remonte l'historique d'un tour (si il y a des entrées passés).
 */
void rewind_history(history* //< L'historique.
);

/**
 * @brief Avance l'historique d'un tour (si il y a des entrée futures).
 */
void continue_history(history* //< L'historique.
);


/**
 * @brief
 * Récupération du plus récent des tours passés (si il y en a un).
 *
 * @return
 * Pointeur vers l'entrée du plus récent des tours passés (NULL si il n'y en a pas).
 *
 * @remark
 * On ne demande pas ici de créér une copie de l'entrée. Il faut simplement retourner
 * un pointeur vers l'endroit où cette entrée est stockée dans l'historique.
 */
turn* last_move_history(history* //!< L'historique.
);


/**
 * @brief
 * Récupération du plus proche des tours futurs (si il y en a un).
 *
 * @return
 * Pointeur vers l'entrée du plus proche des tours futurs (NULL si il n'y en a pas).
 *
 * @remark
 * On ne demande pas ici de créér une copie de l'entrée. Il faut simplement retourner
 * un pointeur vers l'endroit où cette entrée est stockée dans l'historique.
 */
turn* next_move_history(history* //!< L'historique.
);


















#endif
