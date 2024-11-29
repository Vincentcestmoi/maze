#include "game.h"



/****************************/
/*+ Création et libération +*/
/****************************/

// Génération d'un nouveau jeu
game* create_newgame(const int sh, const int sv, mask *m, const generator f, const objgenerator fo, const int nb_minotaure, int tressage)
{
  maze *p_maze;
  if(m != NULL)
  {
    resize_mask(m, sh, sv);
    p_maze = create_proto_maze(m);
  }
  else
  {
    p_maze = create_proto_maze_nomask(sh, sv);
  }
  gen_minotaurs_maze(p_maze, nb_minotaure);
  (*gen_funs[f]) (p_maze);
  (*obj_funs[fo]) (p_maze);
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

void free_game(game *g) {
  free(g->log);
  free(g->minotaurs_alive);
  free(g->minotaurs_dirs);
  free_maze(g->m);
  free(g);
}

/***********************************************/
/*+ Implémentation des attaques de minotaures +*/
/***********************************************/

int game_try_kill_player(game *g, cardinal *card) {
  const int cell = g->m->player;
  int neighbour = get_adj_maze(g->m, cell, NORTH);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1) {
      *card = SOUTH;
      return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, EAST);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1) {
      *card = WEST;
      return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, SOUTH);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1) {
      *card = NORTH;
      return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, WEST);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1) {
      *card = EAST;
      return mino;
    }
  }
  //on n'a pas trouvé de minotaure
  return -1;
}




/***************************/
/*+ Traitement des objets +*/
/***************************/

void game_consume_object(game *g, const int inter, const object obj) {
  switch (obj) {
    case SMALLT:
      g->score += inter * VALSMALL;
      break;
    case MEDT:
      g->score += inter * VALMED;
      break;
    case LARGET:
      g->score += inter * VALLARGE;
      break;
    case BOMB:
      g->nbombs += inter;
      break;
    case POLY:
      g->npolys += inter;
      break;
    default: //inclue EXIT et NONE
      break;
  }
}

object game_treat_object(game *g) {
  const int cell = g->m->player;
  const object obj = get_object_maze(g->m, cell);
  game_consume_object(g, 1, obj);
  if(obj != EXIT)
  {
    add_object_maze(g->m, cell, NONE);
  }
  return obj;
}

/**********************************************************/
/*+ Implémentation d'une demande de mouvement du joueur  +*/
/**********************************************************/

bool implement_game_move(game *, move , strategy ) {
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
