/**
 * @file main.h
 * @brief Paramètres.
 *
 * @details Permet de modifier la taille de l'interface et le jeu initial.
 */

 /**
  * @mainpage Documentation du projet labyrinthe
  *
  * Les fichiers sont divisés en cinq groupes principaux.
  *
  * \subsection I I Les structures de données
  *
  * Ces fichiers ont tous le préfixe "data". Ils implémentent les structures de données
  * utilisées dans les autres fichiers du projet.
  *
  * @remark
  * La structure la plus est importante est l'implémentation des tableaux dynamiques dans
  * le fichier "data_dynarray.h".  Elle utilisée très régulièrement dans tout le projet.
  * Il faudra donc écrire cette implémentation rapidement. Les autres structures (files,
  * tas binaires et union-find) sont utilisées de façon plus spécifique.
  *
  * @subsection II II Gestion des labyrinthes
  *
  * Ces fichiers ont tous le préfixe "maze". Ils contiennent la structure de données qui
  * est utilisée pour représenter un labyrinthe ainsi que toutes les fonctions qui servent
  * à générer de nouveaux labyrinthes.
  *
  * @remark
  * Implémenter les fonctions décrites dans "maze.h" ainsi qu'au moins une des fonctions de
  * génération décrites dans "maze_gen.h" est prioritaire. C'est nécessaire afin d'afficher
  * un premier labyrinthe dans l'interface.
  *
  * @subsection III III Gestion du plateau de jeu.
  *
  * Ces fichiers ont tous le préfixe "game". Ils contiennent toutes les structures et les
  * fonctions qui servent à gérer un jeu.
  *
  * @remark
  * Implémenter la fonction de création d'un jeu dans "game.h" est prioritaire. C'est aussi
  * un prérequis afin d'afficher un premier labyrinthe dans l'interface.
  *
  * @subsection IV IV Solveurs
  *
  * Ces fichiers ont tous le préfixe "solve". Ils contiennent toutes les structures et les
  * fonctions qui servent à parcourir automatiquement les jeux.
  *
  * @subsection V V Interface
  *
  * Ces fichiers ont tous le préfixe "gui". Ils contiennent toutes les structures et les
  * fonctions qui servent à gérer l'interface graphique.
  *
  * @attention
  * Ces fichiers sont déjà écrits. Ils ne doivent PAS être modifiés.
  *
  */

#ifndef MAIN_H_
#define MAIN_H_

#include "game.h"
#include "gui_init_graphic.h"
#include "gui_controller.h"
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>


  /**
  * @brief Dimension horizontale de la fenêtre du labyrinthe (Défaut: 1170).
  */
#define WINDOW_MAZEWIDTH 1170

  /**
  * @brief Dimension verticale de la fenêtre du labyrinthe (Défaut: 970).
  */
#define WINDOW_MAZEHEIGHT 970


  /**
  * @brief Taille horizontale du labyrinthe initial.
  */
#define INITIAL_HSIZE 200

  /**
  * @brief  Taille verticale du labyrinthe initial.
  */
#define INITIAL_VSIZE 200


  /**
  * @brief  Générateur initial.
  */
#define INITIAL_GENERATOR GEN_HKDFS

  /**
  * @brief  Nombre de minotaures initial.
  */
#define INITIAL_MINOTAURS 200

  /**
  * @brief  Taux de tressage initial (en %)
  */
#define INITIAL_BRAIDING 15

  /**
  * @brief  Générateur d'objets initial.
  */
#define INITIAL_OBJECTS OBJ_SIMPLE


  /**
  * @brief  Masque initial: une chaîne de caractères contenant le nom du fichier (NULL si on ne veut pas de masque).
  */
#define INITIAL_MASK "loveC.msk"

#endif // MAIN_H_
