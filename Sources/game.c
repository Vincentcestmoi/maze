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
    if(mino != -1 && g->minotaurs_alive[mino] && !has_wall_maze(g->m, cell, NORTH)) {
      *card = SOUTH;
      g->player_alive = false;
      return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, EAST);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1 && g->minotaurs_alive[mino] && !has_wall_maze(g->m, cell, EAST)) {
      *card = WEST;
      g->player_alive = false;
      return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, SOUTH);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1 && g->minotaurs_alive[mino] && !has_wall_maze(g->m, cell, SOUTH)) {
      *card = NORTH;
      g->player_alive = false;
      return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, WEST);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1 && g->minotaurs_alive[mino] && !has_wall_maze(g->m, cell, EAST)) {
      *card = EAST;
      g->player_alive = false;
      return mino;
    }
  }
  //on n'a pas trouvé de minotaure
  return -1;
}




/***************************/
/*+ Traitement des objets +*/
/***************************/

void game_consume_object(game *g, const int iter, const object obj) {
  switch (obj) {
    case SMALLT:
      g->score += iter * VALSMALL;
      break;
    case MEDT:
      g->score += iter * VALMED;
      break;
    case LARGET:
      g->score += iter * VALLARGE;
      break;
    case BOMB:
      g->nbombs += iter;
      break;
    case POLY:
      g->npolys += iter;
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

bool implement_game_move(game *g, const move mv, const strategy strat) {
    if (!g->player_alive)
    {
      return false;
    }
    if (mv != M_WAIT)
    {
        g->player_dir = (cardinal)mv;
    }
    if(!valid_move_maze(g->m, g->m->player, mv))
    {
        return false;
    }
    //TODO : mouvement des minotaures
    if (!strat)
    {

    }
    if (mv != M_WAIT)
    {
        g->m->player = get_adj_maze(g->m, g->m->player, g->player_dir);
    }
    cardinal *card = malloc(sizeof(card));
    if (card == NULL)
    {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    game_try_kill_player(g, card);
    free(card);
    game_treat_object(g);
    //TODO : gestion de l'historique
    g->turns++;
    return true;
}



/*******************************/
/*+ Implémentation des bombes +*/
/*******************************/

bool game_bomb_wall(game *g) {
  if (g->nbombs == 0)
  {
    return false;
  }
    const int neighbour = get_adj_maze(g->m, g->m->player, g->player_dir);
    if (!can_be_used(g->m, neighbour) || !has_wall_maze(g->m, g->m->player, g->player_dir))
    {
        return false;
    }
    g->nbombs--;
    g->turns++;
    del_wall_maze(g->m, g->m->player, g->player_dir);
    cardinal *card = malloc(sizeof(card));
    if (card == NULL)
    {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    if (game_try_kill_player(g, card) != -1)
    {
        g->player_alive = false;
    }
    free(card);
    //TODO : gestion de l'historique
    return true;
}

/*************************************/
/*+ Implémentation des polys d'algo +*/
/*************************************/

static bool mino_reachable(maze *p_maze, const int mino, const int cell, const int d)
{
    if (!can_be_used(p_maze, cell))
    {
        return false;
    }
    if (mino == cell)
    {
        return true;
    }
    if (d == 0)
    {
        return false;
    }
    int c = get_adj_maze(p_maze, cell, NORTH);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, EAST);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, WEST);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, cell, EAST);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, cell, SOUTH);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, EAST);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, WEST);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, cell, WEST);
    return mino_reachable(p_maze, mino, c, d - 1);
}

bool game_kill_minotaurs(game *g, const int d) {
  if (g->npolys == 0 || !g->player_alive)
  {
      return false;
  }
    const int cell = g->m->player;
    bool done = false;
    for (int i = 0; i < g->m->nb_minotaurs; i++)
    {
        if (g->minotaurs_alive[i] && mino_reachable(g->m, g->m->minotaurs[i], cell, d))
        {
            g->minotaurs_alive[i] = false;
            free_occupied_maze(g->m, g->m->minotaurs[i]);
            done = true;
            //TODO : gestion de l'historique
        }
    }
    if (!done)
    {
        return false;
    }
    g->npolys--;
    g->turns++;
    return true;
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
