#include "game.h"



/****************************/
/*+ Création et libération +*/
/****************************/

// Génération d'un nouveau jeu
game* create_newgame(int, int, mask*, generator, objgenerator, int, int) {
  return NULL;
}

void free_game(game*) {
  return;
}

/***********************************************/
/*+ Implémentation des attaques de minotaures +*/
/***********************************************/

int game_try_kill_player(game*, cardinal*) {
  return -1;
}




/***************************/
/*+ Traitement des objets +*/
/***************************/

void game_consume_object(game*, int, object) {
  return;
}

object game_treat_object(game*) {
  return NONE;
}

/**********************************************************/
/*+ Implémentation d'une demande de mouvement du joueur  +*/
/**********************************************************/

bool implement_game_move(game*, move, strategy) {
  return false;
}



/*******************************/
/*+ Implémentation des bombes +*/
/*******************************/

bool game_bomb_wall(game*) {
  return false;
}

/*************************************/
/*+ Implémentation des polys d'algo +*/
/*************************************/

bool game_kill_minotaurs(game*, int) {
  return false;
}


/*****************************/
/*+ Gestion de l'historique +*/
/*****************************/

bool game_undo(game*) {
  return false;
}

bool game_redo(game*) {
  return false;
}





