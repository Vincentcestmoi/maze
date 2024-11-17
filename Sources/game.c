#include "game.h"



/****************************/
/*+ Création et libération +*/
/****************************/

// Génération d'un nouveau jeu
game* create_newgame(const int sh, const int sv, mask *m, generator f, objgenerator fo, const int nb_minotaure, int tressage)
{
  maze *p_maze;
  if(m->hsize != sh || m->vsize != sv)
  {
    p_maze = create_proto_maze(m);
  }
  else
  {
    p_maze = create_proto_maze_nomask(sh, sv);
  }
  gen_minotaurs_maze(p_maze, nb_minotaure);
  generate_objects_maze(p_maze, fo);
  generate_maze(p_maze, f);
  braid_maze(p_maze, tressage);
  game *p_game = malloc(sizeof(game));
  p_game->m = p_maze;
  p_game->score = 0;
  p_game->nbombs = 0;
  p_game->npolys = 0;
  p_game->player_alive = true;
  p_game->player_dir = NORTH;
  p_game->minotaurs_alive = malloc(nb_minotaure * sizeof(bool));
  p_game->minotaurs_dirs = malloc(nb_minotaure * sizeof(cardinal));
  for(int i = 0; i < nb_minotaure; i++)
  {
    p_game->minotaurs_alive[i] = true;
    p_game->minotaurs_dirs[i] = NORTH;
  }
  p_game->nb_deadends = 0;
  p_game->exits = 0;
  p_game->turns = 0;
  p_game->log = NULL;
  return p_game;
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





