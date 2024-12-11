#include "game.h"
#include "maze_2.h"



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
  p_game->minotaurs_dirs = calloc(nb_minotaure, sizeof(cardinal));
  for(int i = 0; i < nb_minotaure; i++)
  {
    p_game->minotaurs_alive[i] = true;
  }
  p_game->nb_deadends = 0;
  p_game->exits = 0;
  p_game->turns = 0;
  p_game->log = create_history();
  return p_game;
}

void free_game(game *g) {
    free_history(g->log);
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
        g->minotaurs_dirs[mino] = NORTH;
        g->player_alive = false;
        return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, EAST);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1 && g->minotaurs_alive[mino] && !has_wall_maze(g->m, cell, EAST)) {
        *card = WEST;
        g->minotaurs_dirs[mino] = EAST;
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
        g->minotaurs_dirs[mino] = SOUTH;
        return mino;
    }
  }
  neighbour = get_adj_maze(g->m, cell, WEST);
  if(neighbour != -1) {
    const int mino = has_minotaur_maze(g->m, neighbour);
    if(mino != -1 && g->minotaurs_alive[mino] && !has_wall_maze(g->m, cell, WEST)) {
        *card = EAST;
        g->minotaurs_dirs[mino] = WEST;
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
    if (mv != M_WAIT)
    {
        g->player_dir = (cardinal)mv;
    }
    if (!g->player_alive || !valid_move_maze(g->m, g->m->player, mv))
    {
      return false;
    }
    if (mv != M_WAIT)
    {
        free_occupied_maze(g->m, g->m->player);
        g->m->player = get_adj_maze(g->m, g->m->player, g->player_dir);
        make_occupied_maze(g->m, g->m->player);
    }
    move *mino_move = malloc(g->m->nb_minotaurs * sizeof(move));
    str_funs[strat](g->m, mv, mino_move);
    for (int i = 0; i < g->m->nb_minotaurs; i++)
    {
        if (mino_move[i] != M_WAIT)
        {
            g->minotaurs_dirs[i] = (cardinal)mino_move[i];
        }
        if (g->minotaurs_alive[i] && valid_move_maze(g->m, g->m->minotaurs[i], mino_move[i]))
        {
            //si le mino veut et peut bouger
            if (mino_move[i] != M_WAIT)
            {
                free_occupied_maze(g->m, g->m->minotaurs[i]);
                g->m->minotaurs[i] = get_adj_maze(g->m, g->m->minotaurs[i], g->minotaurs_dirs[i]);
                make_occupied_maze(g->m, g->m->minotaurs[i]);
            }
        }
        else
        {
            mino_move[i] = M_WAIT; //on corrige le mouvement pour l'historique
        }
    }
    const t_type typ = T_MOVE;
    t_move *tmove = malloc(sizeof(t_move));
    if (tmove == NULL)
    {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    tmove->obj = game_treat_object(g);
    tmove->playermove = mv;
    tmove->minomoves = mino_move;
    cardinal card = NORTH;
    tmove->killer = game_try_kill_player(g, &card);
    tmove->dirkill = card;
    const turn t = {typ, .tmove = tmove};
    add_entry_history(t, g->log);
    g->turns++;
    return true;
}



/*******************************/
/*+ Implémentation des bombes +*/
/*******************************/

bool game_bomb_wall(game *g) {
  if (g->nbombs == 0 || !g->player_alive)
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
    const t_type typ = T_BOMB;
    t_bomb *tbomb = malloc(sizeof(t_bomb));
    if (tbomb == NULL)
    {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    tbomb->bombdir = g->player_dir;
    tbomb->destroyed = true;
    cardinal card = NORTH;
    tbomb->killer = game_try_kill_player(g, &card);
    tbomb->dirkill = card;
    const turn t = {typ, .tbomb = tbomb};
    add_entry_history(t, g->log);
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
    c = get_adj_maze(p_maze, c, SOUTH);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, SOUTH);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, WEST);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, WEST);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, c, NORTH);
    if (mino_reachable(p_maze, mino, c, d - 1))
    {
        return true;
    }
    c = get_adj_maze(p_maze, cell, NORTH);
    return mino_reachable(p_maze, mino, c, d - 1);
}

bool game_kill_minotaurs(game *g, const int d) {
  if (g->npolys == 0 || !g->player_alive)
  {
      return false;
  }
    const int cell = g->m->player;
    bool done = false;
    bool *minokilled = calloc(g->m->nb_minotaurs, sizeof(bool));
    for (int i = 0; i < g->m->nb_minotaurs; i++)
    {
        if (g->minotaurs_alive[i] && mino_reachable(g->m, g->m->minotaurs[i], cell, d))
        {
            g->minotaurs_alive[i] = false;
            free_occupied_maze(g->m, g->m->minotaurs[i]);
            done = true;
            minokilled[i] = true;
        }
    }
    if (!done)
    {
        free(minokilled);
        return false;
    }
    g->npolys--;
    g->turns++;
    const t_type typ = T_POLY;
    const turn t = {typ, .minokilled = minokilled};
    add_entry_history(t, g->log);
    return true;
}


/*****************************/
/*+ Gestion de l'historique +*/
/*****************************/

bool game_undo(game *g) {
    const turn *t = last_move_history(g->log);
    if (t == NULL)
    {
        return false;
    }
    if (t->type == T_POLY)
    {
        g->npolys++;
        for (int i = 0; i < g->m->nb_minotaurs; i++)
        {
            if (t->minokilled[i])
            {
                g->minotaurs_alive[i] = true;
                make_occupied_maze(g->m, g->m->minotaurs[i]);
            }
        }
        g->turns--;
        rewind_history(g->log); //on note que le tour a été annulé
        return true;
    }
    if (t->type == T_BOMB)
    {
        if (t->tbomb->destroyed)
        {
            build_wall_maze(g->m, g->m->player, t->tbomb->bombdir);
            g->nbombs++;
        }
        if (t->tbomb->killer != -1)
        {
            g->player_alive = true;
            make_occupied_maze(g->m, g->m->player);
        }
        g->turns--;
        rewind_history(g->log); //on note que le tour a été annulé
        return true;
    }
    if (t->type == T_MOVE)
    {
        if (t->tmove->obj != NONE)
        {
            add_object_maze(g->m, g->m->player, t->tmove->obj);
            game_consume_object(g, -1, t->tmove->obj);
        }
        if (t->tmove->killer != -1)
        {
            g->player_alive = true;
        }
        if (t->tmove->playermove != M_WAIT)
        {
            free_occupied_maze(g->m, g->m->player);
            g->m->player = get_adj_maze(g->m, g->m->player, (cardinal)((t->tmove->playermove + 2) % 4));
            make_occupied_maze(g->m, g->m->player);
        }
        g->minotaurs_dirs = (cardinal*)t->tmove->minomoves;
        for (int i = 0; i < g->m->nb_minotaurs; i++)
        {
            if (t->tmove->minomoves[i] != M_WAIT)
            {
                g->minotaurs_dirs[i] = (cardinal)t->tmove->minomoves[i]; //on met à jour les directions des minotaures
                if (g->minotaurs_alive[i])
                {
                    free_occupied_maze(g->m, g->m->minotaurs[i]);
                    g->m->minotaurs[i] = get_adj_maze(g->m, g->m->minotaurs[i], (cardinal)((g->minotaurs_dirs[i] + 2) % 4));
                    make_occupied_maze(g->m, g->m->minotaurs[i]);
                }
            }
        }
        g->turns--;
        rewind_history(g->log); //on note que le tour a été annulé
        return true;
    }
    fprintf(stderr, "Erreur d'historique, type inconnu\n");
    exit(EXIT_FAILURE);
}

bool game_redo(game *g) {
    const turn *t = next_move_history(g->log);
    if (t == NULL)
    {
        return false;
    }
    if (t->type == T_POLY)
    {
        g->npolys--;
        for (int i = 0; i < g->m->nb_minotaurs; i++)
        {
            if (t->minokilled[i])
            {
                g->minotaurs_alive[i] = false;
                free_occupied_maze(g->m, g->m->minotaurs[i]);
            }
        }
        g->turns++;
        continue_history(g->log); //on note que le tour a été rejoué
        return true;
    }
    if (t->type == T_BOMB)
    {
        if (t->tbomb->destroyed)
        {
            del_wall_maze(g->m, g->m->player, t->tbomb->bombdir);
            g->player_dir = t->tbomb->bombdir;
            g->nbombs--;
        }
        if (t->tbomb->killer != -1)
        {
            g->minotaurs_dirs[t->tbomb->killer] = t->tbomb->dirkill;
            g->player_alive = false;
            free_occupied_maze(g->m, g->m->player);
        }
        g->turns++;
        continue_history(g->log); //on note que le tour a été rejoué
        return true;
    }
    if (t->type == T_MOVE)
    {
        if (t->tmove->obj != NONE)
        {
            game_treat_object(g);
        }
        if (t->tmove->killer != -1)
        {
            g->player_alive = false;
        }
        if (t->tmove->playermove != M_WAIT)
        {
            free_occupied_maze(g->m, g->m->player);
            g->m->player = get_adj_maze(g->m, g->m->player, (cardinal)t->tmove->playermove);
            make_occupied_maze(g->m, g->m->player);
        }
        for (int i = 0; i < g->m->nb_minotaurs; i++)
        {
            if (t->tmove->minomoves[i] != M_WAIT)
            {
                g->minotaurs_dirs[i] = (cardinal)t->tmove->minomoves[i]; //on met à jour les directions des minotaures
                if (g->minotaurs_alive[i])
                {
                    free_occupied_maze(g->m, g->m->minotaurs[i]);
                    g->m->minotaurs[i] = get_adj_maze(g->m, g->m->minotaurs[i], (cardinal)t->tmove->minomoves[i]);
                    make_occupied_maze(g->m, g->m->minotaurs[i]);
                }
            }
        }
        g->turns++;
        continue_history(g->log); //on note que le tour a été rejoué
        return true;
    }
    fprintf(stderr, "Erreur d'historique, type inconnu\n");
    exit(EXIT_FAILURE);
}
