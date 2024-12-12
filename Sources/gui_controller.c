#include "gui_controller.h"



/*********************************/
/*+ Flags pour la boucle de jeu **/
/*********************************/



/* Tableau des flags */
bool flags[NB_FLAGS];

void reset_flags(void) {
  for (int i = 0; i < NB_FLAGS; i++)
    flags[i] = false;
}



void set_flag(ctl_flags f) {
  flags[f] = true;
}

void remove_flag(ctl_flags f) {
  flags[f] = false;
}

clock_t start, end;

/**************************************/
/*+ Intialisation/Suppression du jeu **/
/**************************************/

bool ctl_game_ok(game* thegame) {

  if (!thegame || !thegame->m) {
    return false;
  }

  if (!thegame->m->objects || !thegame->m->props) {
    return false;
  }

  if (thegame->m->nb_minotaurs && (!thegame->minotaurs_alive || !thegame->minotaurs_dirs || !thegame->m->minotaurs)) {
    return false;
  }

  if (thegame->m->hsize <= 0 || thegame->m->vsize <= 0) {
    return false;
  }

  if (thegame->m->nb_reachable <= 0) {
    return false;
  }

  return true;
}

void ctl_free_gui(void) {
  // Libération des tableaux qui sauvegardent les paramètres des minotaures pour une animation. 
  free(an_minotaurs_pos);
  an_minotaurs_pos = NULL;
  free(an_minotaurs_move);
  an_minotaurs_move = NULL;
  free(an_minotaurs_alive);
  an_minotaurs_alive = NULL;

  // Libération des chemins
  ctl_clear_paths();

  // Libération des parcours enregistrés par les solveurs
  ctl_sim_delete();
  ctl_delete_complex();

}

game* ctl_init_game(bool firstgen) {

  // Libération des éléments de l'interface spécifiques à l'éventuel ancien jeu
  ctl_free_gui();

  // Réinitialisation des flags
  reset_flags();

  // Réinitialisation des boutons
  var_reset_buttons();
  if (!firstgen) {
    set_flag(PUSHED);
  }

  // Calcul du masque à utiliser
  mask* themask;
  if (firstgen) {
    // Cas de la première génération (on utilise le masque par défaut).
    if (gen_initialmask == NULL) {
      themask = NULL;
    }
    else {
      char thepath[10 + strlen(gen_initialmask)];
      sprintf(thepath, "../Masques/%s", gen_initialmask);
      themask = read_mask(thepath);
    }

  }
  else {
    // Cas d'une nouvelle génération (on utilise le masque donné par l'interface).
    if (var_maskindex == -1) {
      themask = NULL;
    }
    else {
      char thepath[10 + strlen(var_maskstab[var_maskindex]->d_name)];
      sprintf(thepath, "../Masques/%s", var_maskstab[var_maskindex]->d_name);
      themask = read_mask(thepath);
    }
  }

  // Génération du jeu.
  game* newgame = create_newgame(gen_hsize, gen_vsize, themask, gen_mode, obj_mode, gen_nbminotaurs, gen_braiding);

  // On n'a plus besoin du masque
  free_mask(themask);


  if (ctl_game_ok(newgame)) {
    // Si le jeu a été généré correctement, on passe dans le mode animation par défaut
    var_crender = AN_IDLE;
  }
  else {
    // Sinon, on passe dans le mode spécial qui gère le cas où la génération a échoué.
    var_crender = AN_NULL;
    free_game(newgame);
    return NULL;
  }



  MALLOC(an_minotaurs_pos, newgame->m->nb_minotaurs);
  MALLOC(an_minotaurs_move, newgame->m->nb_minotaurs);
  MALLOC(an_minotaurs_alive, newgame->m->nb_minotaurs);

  scroll_to_player(newgame);
  game_treat_object(newgame);
  an_killer = game_try_kill_player(newgame, &an_killdir);
  if (!newgame->player_alive) {
    var_frame = 0;
    var_crender = AN_KILL;
  }
  newgame->nbombs = 10;
  newgame->npolys = 10;


  return newgame;
}

/*********************************/
/*+ Gestion du buffer d'édition **/
/*********************************/


void ctl_update_from_buffer(game* thegame) {
  char* end_str;
  int num = strtol(var_editbuffer->content, &end_str, 10);
  if (*end_str != '\0') {
    fprintf(stderr, "Error: malformed string\n");
    return;
  }
  switch (var_edit_mode)
  {
  case ED_BRAID:
    if (num < 0 || num > 100) {
      fprintf(stderr, "Error: invalid percentage\n");
      return;
    }
    else {
      gen_braiding = num;
    }
    break;
  case ED_MINO:
    gen_nbminotaurs = num;
    break;
  case ED_X:
    if (num > 0) {
      gen_hsize = num;
    }
    else {
      gen_hsize = 1;
    }
    break;
  case ED_Y:
    if (num > 0) {
      gen_vsize = num;
    }
    else {
      gen_vsize = 1;
    }
    break;
  case ED_SEG:
    ctl_update_seg_num(thegame, num);
    break;
  default:
    break;
  }
}

/*********************************/
/*+ Prise en compte des actions **/
/*********************************/


void ctl_enter_menu(game* thegame) {
  if (thegame && thegame->m) {
    switch (var_menu) {
    default:
      break;
    }
  }
}




void ctl_exit_menu(void) {
  switch (var_menu) {
  case MENU_PLAY:
    break;
  default:
    break;
  }
}


void ctl_init_action(game* thegame, move themove) {
  an_player_pos = thegame->m->player;
  an_player_move = M_WAIT;
  for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
    an_minotaurs_pos[i] = thegame->m->minotaurs[i];
    an_minotaurs_move[i] = M_WAIT;
  }
  if (themove != M_WAIT) {
    an_obj_taken = get_object_maze(thegame->m, get_adj_maze(thegame->m, thegame->m->player, (cardinal)themove));
  }
  else {
    an_obj_taken = get_object_maze(thegame->m, thegame->m->player);
  }

  for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
    an_minotaurs_alive[i] = thegame->minotaurs_alive[i];
  }

  an_bombdir = thegame->player_dir;
  an_hadwall = has_wall_maze(thegame->m, an_player_pos, an_bombdir);
}



void ctl_init_redo(game* thegame) {
  turn* theturn = next_move_history(thegame->log);
  if (!theturn) {
    return;
  }

  an_player_pos = thegame->m->player;
  an_player_move = M_WAIT;
  for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
    an_minotaurs_pos[i] = thegame->m->minotaurs[i];
    an_minotaurs_move[i] = M_WAIT;
  }

  if (theturn->type == T_MOVE && theturn->tmove->playermove != M_WAIT) {
    an_obj_taken = get_object_maze(thegame->m, get_adj_maze(thegame->m, thegame->m->player, (cardinal)theturn->tmove->playermove));
  }
  else {
    an_obj_taken = get_object_maze(thegame->m, thegame->m->player);
  }

  for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
    an_minotaurs_alive[i] = thegame->minotaurs_alive[i];
  }
  if (theturn->type == T_BOMB) {
    an_bombdir = theturn->tbomb->bombdir;
    an_hadwall = has_wall_maze(thegame->m, an_player_pos, an_bombdir);
  }
}



void ctl_init_undo(game* thegame) {
  turn* theturn = last_move_history(thegame->log);
  if (!theturn) {
    return;
  }



  if (theturn->type == T_MOVE) {
    an_player_move = theturn->tmove->playermove;
    an_player_pos = thegame->m->player;

    for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
      an_minotaurs_move[i] = theturn->tmove->minomoves[i];
      an_minotaurs_pos[i] = thegame->m->minotaurs[i];
    }
    if (theturn->tmove->playermove != M_WAIT) {
      an_obj_taken = get_object_maze(thegame->m, get_adj_maze(thegame->m, thegame->m->player, (cardinal)theturn->tmove->playermove));
    }
    else {
      an_obj_taken = get_object_maze(thegame->m, thegame->m->player);
    }
  }

  if (theturn->type == T_POLY) {
    for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
      an_minotaurs_alive[i] = thegame->minotaurs_alive[i];
    }
  }
  if (theturn->type == T_BOMB) {
    an_bombdir = theturn->tbomb->bombdir;
    an_player_pos = thegame->m->player;
    an_hadwall = has_wall_maze(thegame->m, an_player_pos, an_bombdir);
    an_destroyed = theturn->tbomb->destroyed;
  }

  an_killed = !thegame->player_alive;
  if (an_killed) {
    for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
      if (thegame->minotaurs_alive[i]) {
        for (int dir = 0; dir < 4; dir++) {
          if (thegame->m->player == get_adj_maze(thegame->m, thegame->m->minotaurs[i], dir) && !has_wall_maze(thegame->m, thegame->m->minotaurs[i], dir)) {
            an_killer = i;
            an_killdir = dir;
            return;
          }
        }
      }
    }
  }

}

void ctl_update_action(game* thegame) {
  // Mouvement du joueur
  for (int dir = 0; dir < 4; dir++) {
    if (thegame->m->player == get_adj_maze(thegame->m, an_player_pos, dir)) {
      an_player_move = (move)dir;
    }
  }

  // Mouvement des minotaures
  for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
    for (int dir = 0; dir < 4; dir++) {
      if (thegame->m->minotaurs[i] == get_adj_maze(thegame->m, an_minotaurs_pos[i], dir)) {
        an_minotaurs_move[i] = (move)dir;
      }
    }
  }
  an_destroyed = an_hadwall && !has_wall_maze(thegame->m, an_player_pos, an_bombdir);
  an_killed = !thegame->player_alive;

  // Gestion des attaques
  if (an_killed) {

    for (int i = 0; i < thegame->m->nb_minotaurs; i++) {
      if (thegame->minotaurs_alive[i]) {
        for (int dir = 0; dir < 4; dir++) {
          if (thegame->m->player == get_adj_maze(thegame->m, thegame->m->minotaurs[i], dir) && !has_wall_maze(thegame->m, thegame->m->minotaurs[i], dir)) {
            an_killer = i;
            an_killdir = dir;
            return;
          }
        }
      }
    }
  }


}




void ctl_move_action(game* thegame, move themove) {
  if (flags[ANIMATED]) {
    return;
  }
  ctl_init_action(thegame, themove);
  if (!flags[WIN] && implement_game_move(thegame, themove, var_mino_strat)) {
    set_flag(ANIMATED);
    ctl_update_action(thegame);
    var_crender = AN_WALK;
  }
}

void ctl_bomb_action(game* thegame) {
  if (flags[ANIMATED]) {
    return;
  }
  ctl_init_action(thegame, M_WAIT);
  if (!flags[WIN] && game_bomb_wall(thegame)) {
    ctl_update_action(thegame);
    set_flag(ANIMATED);
    var_crender = AN_BOMB;

    // On rétablit temporairement le mur pour l'animation
    if (an_hadwall) {
      build_wall_maze(thegame->m, an_player_pos, an_bombdir);
    }
  }
}


void ctl_poly_action(game* thegame) {
  if (flags[ANIMATED]) {
    return;
  }
  ctl_init_action(thegame, M_WAIT);
  if (!flags[WIN] && game_kill_minotaurs(thegame, 3)) {
    set_flag(ANIMATED);
    var_crender = AN_POLY;
  }
}


void ctl_redo_action(game* thegame) {
  if (!thegame->log || flags[ANIMATED]) {
    return;
  }
  ctl_init_redo(thegame);


  if (game_redo(thegame)) {
    ctl_update_action(thegame);
    set_flag(ANIMATED);
    switch (last_move_history(thegame->log)->type)
    {
    case T_MOVE:
      var_crender = AN_WALK;
      break;
    case T_BOMB:
      var_crender = AN_BOMB;
      // On rétablit temporairement le mur pour l'animation
      if (an_hadwall) {
        build_wall_maze(thegame->m, an_player_pos, an_bombdir);
      }
      break;
    case T_POLY:
      var_crender = AN_POLY;
      break;

    default:
      break;
    }
  }
}

void ctl_undo_action(game* thegame) {
  if (!thegame->log || flags[ANIMATED]) {
    return;
  }


  remove_flag(WIN);
  ctl_init_undo(thegame);
  if (game_undo(thegame)) {
    set_flag(ANIMATED);
    switch (next_move_history(thegame->log)->type)
    {
    case T_MOVE:
      if (an_killed) {
        var_crender = AN_UKILLW;
      }
      else {
        var_crender = AN_UWALK;
      }

      break;
    case T_BOMB:
      if (an_destroyed) {
        del_wall_maze(thegame->m, an_player_pos, an_bombdir);

      }

      if (an_killed) {
        var_crender = AN_UKILLB;
      }
      else {
        var_crender = AN_UBOMB;
      }
      break;
    case T_POLY:
      var_crender = AN_UPOLY;
      break;

    default:
      break;
    }
  }
}

static void update_mask_from_file(void) {

  if (var_maskindex == -1) {
    sprintf(gen_maskname, "Pas de masque");
  }
  else {
    sprintf(gen_maskname, "%s", var_maskstab[var_maskindex]->d_name);
  }
}

void ctl_next_mask_action(void) {

  if (var_masktabsize == -1) {
    return;
  }
  if (var_maskindex < var_masktabsize - 1) {
    var_maskindex++;

  }
  else {
    var_maskindex = -1;
  }
  update_mask_from_file();
}
void ctl_previous_mask_action(void) {
  if (var_masktabsize == -1) {
    return;
  }
  if (var_maskindex >= 0) {
    var_maskindex--;
  }
  else {
    var_maskindex = var_masktabsize - 1;
  }
  update_mask_from_file();
}

bool ctl_mouse_on_maze(SDL_Event* theevent) {
  return (theevent->motion.x >= 0) && (theevent->motion.y >= 0) && (theevent->motion.x < var_maze_width) && (theevent->motion.y < var_maze_height);
}

bool ctl_mouse_on_menu(SDL_Event* theevent) {
  return (theevent->motion.x >= var_maze_width) && (theevent->motion.y >= 0) && (theevent->motion.x < var_screen_width) && (theevent->motion.y < var_screen_height);
}


static void ctl_scalex(int* x) {
  *x -= var_maze_width;
  *x += 970;
}

buttons ctl_get_button(int x, int y) {
  ctl_scalex(&x);


  if ((x >= 1076) && (y >= 190) && (x < 1107) && (y < 221)) {
    var_push_button(BT_LMODE);
    set_flag(PUSHED);
    return BT_LMODE;
  }


  if ((x >= 1357) && (y >= 190) && (x < 1387) && (y < 221)) {
    var_push_button(BT_RMODE);
    set_flag(PUSHED);
    return BT_RMODE;
  }

  if (var_menu == MENU_PLAY) {

    if ((x >= 1209) && (y >= 778) && (x < 1253) && (y < 822)) {
      var_push_button(BT_UP);
      set_flag(PUSHED);
      return BT_UP;
    }

    if ((x >= 1209) && (y >= 828) && (x < 1253) && (y < 872)) {
      var_push_button(BT_WAIT);
      set_flag(PUSHED);
      return BT_WAIT;
    }

    if ((x >= 1209) && (y >= 878) && (x < 1253) && (y < 922)) {
      var_push_button(BT_DOWN);
      set_flag(PUSHED);
      return BT_DOWN;
    }

    if ((x >= 1159) && (y >= 828) && (x < 1203) && (y < 872)) {
      var_push_button(BT_LEFT);
      set_flag(PUSHED);
      return BT_LEFT;
    }


    if ((x >= 1259) && (y >= 828) && (x < 1303) && (y < 872)) {
      var_push_button(BT_RIGHT);
      set_flag(PUSHED);
      return BT_RIGHT;
    }


    if ((x >= 1369) && (y >= 778) && (x < 1413) && (y < 822)) {
      var_push_button(BT_BOMB);
      set_flag(PUSHED);
      return BT_BOMB;
    }

    if ((x >= 1369) && (y >= 828) && (x < 1413) && (y < 872)) {
      var_push_button(BT_POLY);
      set_flag(PUSHED);
      return BT_POLY;
    }

    if ((x >= 1369) && (y >= 878) && (x < 1413) && (y < 922)) {
      var_push_button(BT_EXIT);
      set_flag(PUSHED);
      return BT_EXIT;
    }


    if ((x >= 1049) && (y >= 778) && (x < 1093) && (y < 822)) {
      var_push_button(BT_UNDO);
      set_flag(PUSHED);
      return BT_UNDO;
    }

    if ((x >= 1049) && (y >= 828) && (x < 1093) && (y < 872)) {
      var_push_button(BT_REDO);
      set_flag(PUSHED);
      return BT_REDO;
    }

    if ((x >= 1049) && (y >= 878) && (x < 1093) && (y < 922)) {
      var_push_button(BT_CAMERA);
      set_flag(PUSHED);
      return BT_CAMERA;
    }



    if ((x >= 1000) && (y >= 485) && (x < 1015) && (y < 501)) {
      var_push_button(BT_LSPEED);
      set_flag(PUSHED);
      return BT_LSPEED;
    }

    if ((x >= 1206) && (y >= 485) && (x < 1221) && (y < 501)) {
      var_push_button(BT_RSPEED);
      set_flag(PUSHED);
      return BT_RSPEED;
    }

    if ((x >= 1252) && (y >= 485) && (x < 1267) && (y < 501)) {
      var_push_button(BT_LZOOM);
      set_flag(PUSHED);
      return BT_LZOOM;
    }

    if ((x >= 1458) && (y >= 485) && (x < 1473) && (y < 501)) {
      var_push_button(BT_RZOOM);
      set_flag(PUSHED);
      return BT_RZOOM;
    }





    if ((x >= 1076) && (y >= 556) && (x < 1107) && (y < 587)) {
      var_push_button(BT_LSTRAT);
      set_flag(PUSHED);
      return BT_LSTRAT;
    }


    if ((x >= 1357) && (y >= 556) && (x < 1387) && (y < 587)) {
      var_push_button(BT_RSTRAT);
      set_flag(PUSHED);
      return BT_RSTRAT;
    }
  }


  if (var_menu == MENU_GENE) {

    if ((x >= 999) && (y >= 336) && (x < 1028) && (y < 366)) {
      var_push_button(BT_LALGO);
      set_flag(PUSHED);
      return BT_LALGO;
    }


    if ((x >= 1435) && (y >= 336) && (x < 1465) && (y < 366)) {
      var_push_button(BT_RALGO);
      set_flag(PUSHED);
      return BT_RALGO;
    }



    if ((x >= 999) && (y >= 420) && (x < 1028) && (y < 451)) {
      var_push_button(BT_LMASK);
      set_flag(PUSHED);
      return BT_LMASK;
    }


    if ((x >= 1435) && (y >= 420) && (x < 1465) && (y < 451)) {
      var_push_button(BT_RMASK);
      set_flag(PUSHED);
      return BT_RMASK;
    }


    if ((x >= 999) && (y >= 506) && (x < 1028) && (y < 536)) {
      var_push_button(BT_LOBJ);
      set_flag(PUSHED);
      return BT_LOBJ;
    }


    if ((x >= 1435) && (y >= 506) && (x < 1465) && (y < 536)) {
      var_push_button(BT_ROBJ);
      set_flag(PUSHED);
      return BT_ROBJ;
    }

    if ((x >= 1435) && (y >= 594) && (x < 1465) && (y < 633)) {
      var_push_button(BT_BRAID);
      set_flag(PUSHED);
      return BT_BRAID;
    }

    if ((x >= 1196) && (y >= 679) && (x < 1226) && (y < 709)) {
      var_push_button(BT_X);
      set_flag(PUSHED);
      return BT_X;
    }

    if ((x >= 1435) && (y >= 679) && (x < 1465) && (y < 709)) {
      var_push_button(BT_Y);
      set_flag(PUSHED);
      return BT_Y;
    }

    if ((x >= 1435) && (y >= 764) && (x < 1465) && (y < 795)) {
      var_push_button(BT_MINO);
      set_flag(PUSHED);
      return BT_MINO;
    }

    if ((x >= 1133) && (y >= 852) && (x < 1329) && (y < 900)) {
      var_push_button(BT_GEN);
      set_flag(PUSHED);
      return BT_GEN;
    }

  }




  if (var_menu == MENU_SOSI) {
    //printf("x:%d, y %d\n", x, y);
    if ((x >= 999) && (y >= 336) && (x < 1028) && (y < 366)) {
      var_push_button(BT_SIML);
      set_flag(PUSHED);
      return BT_SIML;
    }


    if ((x >= 1184) && (y >= 336) && (x < 1216) && (y < 366)) {
      var_push_button(BT_SIMR);
      set_flag(PUSHED);
      return BT_SIMR;
    }

    if ((x >= 1246) && (y >= 336) && (x < 1280) && (y < 366)) {
      var_push_button(BT_SIMGL);
      set_flag(PUSHED);
      return BT_SIMGL;
    }


    if ((x >= 1435) && (y >= 336) && (x < 1465) && (y < 366)) {
      var_push_button(BT_SIMGR);
      set_flag(PUSHED);
      return BT_SIMGR;
    }

    if ((x >= 1008) && (y >= 382) && (x < 1204) && (y < 414)) {
      var_push_button(BT_SIMMINOS);
      set_flag(PUSHED);
      return BT_SIMMINOS;
    }

    if ((x >= 999) && (y >= 451) && (x < 1028) && (y < 484)) {
      var_push_button(BT_HEUL);
      set_flag(PUSHED);
      return BT_HEUL;
    }

    if ((x >= 1435) && (y >= 451) && (x < 1465) && (y < 484)) {
      var_push_button(BT_HEUR);
      set_flag(PUSHED);
      return BT_HEUR;
    }


    if ((x >= 1008) && (y >= 501) && (x < 1204) && (y < 532)) {
      var_push_button(BT_SIMCOMP);
      set_flag(PUSHED);
      return BT_SIMCOMP;
    }


    if ((x >= 1257) && (y >= 501) && (x < 1455) && (y < 532)) {
      var_push_button(BT_SIMDELE);
      set_flag(PUSHED);
      return BT_SIMDELE;
    }







    if ((x >= 1065) && (y >= 792) && (x < 1154) && (y < 824)) {
      var_push_button(BT_RED1);
      set_flag(PUSHED);
      return BT_RED1;
    }

    if ((x >= 1215) && (y >= 792) && (x < 1303) && (y < 824)) {
      var_push_button(BT_GREEN1);
      set_flag(PUSHED);
      return BT_GREEN1;
    }

    if ((x >= 1365) && (y >= 792) && (x < 1454) && (y < 824)) {
      var_push_button(BT_BLUE1);
      set_flag(PUSHED);
      return BT_BLUE1;
    }

    if ((x >= 1065) && (y >= 832) && (x < 1154) && (y < 864)) {
      var_push_button(BT_RED2);
      set_flag(PUSHED);
      return BT_RED2;
    }

    if ((x >= 1215) && (y >= 832) && (x < 1303) && (y < 864)) {
      var_push_button(BT_GREEN2);
      set_flag(PUSHED);
      return BT_GREEN2;
    }

    if ((x >= 1365) && (y >= 832) && (x < 1454) && (y < 864)) {
      var_push_button(BT_BLUE2);
      set_flag(PUSHED);
      return BT_BLUE2;
    }



    if ((x >= 1065) && (y >= 897) && (x < 1154) && (y < 929)) {
      var_push_button(BT_DTR);
      set_flag(PUSHED);
      return BT_DTR;
    }

    if ((x >= 1215) && (y >= 897) && (x < 1303) && (y < 929)) {
      var_push_button(BT_ATR);
      set_flag(PUSHED);
      return BT_ATR;
    }

    if ((x >= 1365) && (y >= 897) && (x < 1454) && (y < 929)) {
      var_push_button(BT_RTR);
      set_flag(PUSHED);
      return BT_RTR;
    }

  }


  if (var_menu == MENU_SOAD) {

    //printf("x:%d, y %d\n", x, y);
    if ((x >= 999) && (y >= 336) && (x < 1028) && (y < 366)) {
      var_push_button(BT_ADVL);
      set_flag(PUSHED);
      return BT_ADVL;
    }


    if ((x >= 1435) && (y >= 336) && (x < 1465) && (y < 366)) {
      var_push_button(BT_ADVR);
      set_flag(PUSHED);
      return BT_ADVR;
    }

    if ((x >= 1022) && (y >= 383) && (x < 1211) && (y < 413)) {
      var_push_button(BT_ADVCOMP);
      set_flag(PUSHED);
      return BT_ADVCOMP;
    }

    if ((x >= 1252) && (y >= 383) && (x < 1442) && (y < 413)) {
      var_push_button(BT_ADVDELE);
      set_flag(PUSHED);
      return BT_ADVDELE;
    }


    if ((x >= 999) && (y >= 603) && (x < 1028) && (y < 636)) {
      var_push_button(BT_ADVMODEL);
      set_flag(PUSHED);
      return BT_ADVMODEL;
    }


    if ((x >= 1435) && (y >= 603) && (x < 1465) && (y < 636)) {
      var_push_button(BT_ADVMODER);
      set_flag(PUSHED);
      return BT_ADVMODER;
    }

    if ((x >= 1435) && (y >= 659) && (x < 1465) && (y < 691)) {
      var_push_button(BT_ADVSEG);
      set_flag(PUSHED);
      return BT_ADVSEG;
    }

    if ((x >= 1022) && (y >= 708) && (x < 1211) && (y < 740)) {
      var_push_button(BT_ADVP);
      set_flag(PUSHED);
      return BT_ADVP;
    }

    if ((x >= 1252) && (y >= 708) && (x < 1442) && (y < 740)) {
      var_push_button(BT_ADVN);
      set_flag(PUSHED);
      return BT_ADVN;
    }


  }



  return BT_NONE;
}


void ctl_mouse_motion(game*, SDL_Event* theevent) {
  if (flags[MOUSEDOWN] && ctl_mouse_on_maze(theevent)) {
    scroll_with_mouse(theevent->motion.x, theevent->motion.y);
  }
  var_mousePosx = theevent->motion.x;
  var_mousePosy = theevent->motion.y;
}

void ctl_mouse_click(game* thegame, SDL_Event* theevent) {
  if (!flags[MOUSEDOWN] && theevent->button.button == SDL_BUTTON_LEFT)
  {
    set_flag(MOUSEDOWN);
  }

  if (ctl_mouse_on_menu(theevent)) {

    buttons thebutton = ctl_get_button(theevent->motion.x, theevent->motion.y);

    switch (thebutton)
    {
    case BT_LMODE:
      ctl_exit_menu();
      var_menu = (var_menu + MENU_SIZE - 1) % MENU_SIZE;
      ctl_enter_menu(thegame);
      return;
      break;
    case BT_RMODE:
      ctl_exit_menu();
      var_menu = (var_menu + 1) % MENU_SIZE;
      ctl_enter_menu(thegame);
      return;
      break;
    default:
      break;
    }


    if (var_menu == MENU_PLAY && thegame && thegame->m) {

      switch (thebutton)
      {
      case BT_UP:
        ctl_move_action(thegame, M_NORTH);
        break;
      case BT_RIGHT:
        ctl_move_action(thegame, M_EAST);
        break;
      case BT_DOWN:
        ctl_move_action(thegame, M_SOUTH);
        break;
      case BT_LEFT:
        ctl_move_action(thegame, M_WEST);
        break;
      case BT_WAIT:
        ctl_move_action(thegame, M_WAIT);
        break;
      case BT_BOMB:
        ctl_bomb_action(thegame);
        break;
      case BT_POLY:
        ctl_poly_action(thegame);
        break;
      case BT_EXIT:
        if (thegame->m->objects[thegame->m->player] == EXIT) {
          set_flag(WIN);
        }
        break;
      case BT_UNDO:
        ctl_undo_action(thegame);
        break;
      case BT_REDO:
        ctl_redo_action(thegame);
        break;
      case BT_CAMERA:
        scroll_to_player(thegame);
        break;
      case BT_LSPEED:
        decrease_speed();
        break;

      case BT_RSPEED:
        increase_speed();
        break;
      case BT_LZOOM:
        decrease_zoom();
        break;
      case BT_RZOOM:
        increase_zoom();
        break;
      case BT_LSTRAT:
        var_mino_strat = (var_mino_strat + STR_SIZE - 1) % STR_SIZE;
        break;
      case BT_RSTRAT:
        var_mino_strat = (var_mino_strat + 1) % STR_SIZE;
        break;
      default:
        break;
      }
      return;
    }

    if (var_menu == MENU_GENE) {

      switch (thebutton)
      {
      case BT_GEN:
        set_flag(GENERATE);
        break;
      case BT_LMASK:
        ctl_previous_mask_action();
        break;
      case BT_RMASK:
        ctl_next_mask_action();
        break;
      case BT_LALGO:
        gen_mode = (gen_mode + GEN_SIZE - 1) % GEN_SIZE;
        break;
      case BT_RALGO:
        gen_mode = (gen_mode + 1) % GEN_SIZE;
        break;
      case BT_LOBJ:
        obj_mode = (obj_mode + OBJ_SIZE - 1) % OBJ_SIZE;
        break;
      case BT_ROBJ:
        obj_mode = (obj_mode + 1) % OBJ_SIZE;
        break;
      case BT_X:
        SDL_FlushEvent(SDL_TEXTINPUT);
        var_editbuffer = create_text_buffer();
        var_edit_mode = ED_X;
        break;
      case BT_Y:
        SDL_FlushEvent(SDL_TEXTINPUT);
        var_editbuffer = create_text_buffer();
        var_edit_mode = ED_Y;
        break;
      case BT_MINO:
        SDL_FlushEvent(SDL_TEXTINPUT);
        var_editbuffer = create_text_buffer();
        var_edit_mode = ED_MINO;
        break;
      case BT_BRAID:
        SDL_FlushEvent(SDL_TEXTINPUT);
        var_editbuffer = create_text_buffer();
        var_edit_mode = ED_BRAID;
        break;

      default:
        break;
      }
      return;
    }


    if (var_menu == MENU_SOSI && thegame && thegame->m) {
      switch (ctl_get_button(theevent->motion.x, theevent->motion.y))
      {
      case BT_SIML:
        var_sim_algorithm = (var_sim_algorithm + ALG_SIZE - 1) % ALG_SIZE;
        break;
      case BT_SIMR:
        var_sim_algorithm = (var_sim_algorithm + 1) % ALG_SIZE;
        break;
      case BT_SIMGL:
        var_sim_goal = (var_sim_goal + GOA_SIZE - 1) % GOA_SIZE;
        break;
      case BT_SIMGR:
        var_sim_goal = (var_sim_goal + 1) % GOA_SIZE;
        break;
      case BT_SIMMINOS:
        var_sim_minos = !var_sim_minos;
        break;

      case BT_HEUL:
        if (var_sim_algorithm == ALG_AST) {
          var_heuristic = (var_heuristic + HEU_SIZE - 1) % HEU_SIZE;
        }
        break;
      case BT_HEUR:
        if (var_sim_algorithm == ALG_AST) {
          var_heuristic = (var_heuristic + 1) % HEU_SIZE;
        }
        break;

      case BT_SIMCOMP:
        ctl_sim_compute(thegame);
        break;
      case BT_SIMDELE:
        ctl_sim_delete();
        break;
      case BT_RED1:
        ctl_update_redpath(thegame);
        break;
      case BT_GREEN1:
        ctl_update_greenpath(thegame);
        break;
      case BT_BLUE1:
        ctl_update_bluepath(thegame);
        break;
      case BT_RED2:
        ctl_delete_redpath();
        break;
      case BT_GREEN2:
        ctl_delete_greenpath();
        break;
      case BT_BLUE2:
        ctl_delete_bluepath();
        break;
      case BT_DTR:
        ctl_print_searchpath();
        break;
      case BT_ATR:
        ctl_launch_searchpath();
        break;
      case BT_RTR:
        ctl_hide_searchpath();
        break;
      default:
        break;
      }

    }


  }


  if (var_menu == MENU_SOAD && thegame && thegame->m) {

    switch (ctl_get_button(theevent->motion.x, theevent->motion.y))
    {
    case BT_LMODE:
      ctl_exit_menu();
      var_menu = (var_menu + MENU_SIZE - 1) % MENU_SIZE;
      ctl_enter_menu(thegame);
      break;
    case BT_RMODE:
      ctl_exit_menu();
      var_menu = (var_menu + 1) % MENU_SIZE;
      ctl_enter_menu(thegame);
      break;
    case BT_ADVL:
      var_cmp_algorithm = (var_cmp_algorithm + CMP_SIZE - 1) % CMP_SIZE;
      break;
    case BT_ADVR:
      var_cmp_algorithm = (var_cmp_algorithm + 1) % CMP_SIZE;
      break;
    case BT_ADVCOMP:
      ctl_compute_complex(thegame);
      break;
    case BT_ADVDELE:
      ctl_delete_complex();
      break;
    case BT_ADVMODEL:
      ctl_togglel_complex(thegame);
      break;
    case BT_ADVMODER:
      ctl_toggler_complex(thegame);
      break;
    case BT_ADVSEG:
      if (var_adv_path && var_adv_mode) {
        SDL_FlushEvent(SDL_TEXTINPUT);
        var_editbuffer = create_text_buffer();
        var_edit_mode = ED_SEG;
      }
      break;
    case BT_ADVN:
      ctl_next_complex(thegame);
      break;
    case BT_ADVP:
      ctl_previous_complex(thegame);
      break;
    default:
      break;
    }

    return;
  }

}




void ctl_mouse_unclick(game*, SDL_Event* theevent) {
  if (flags[MOUSEDOWN] && theevent->button.button == SDL_BUTTON_LEFT)
  {
    remove_flag(MOUSEDOWN);
  }
}

void ctl_keyboard(game* thegame, SDL_Event* theevent) {



  switch (theevent->key.keysym.sym)
  {
  case SDLK_m:
    var_push_button(BT_RMODE);
    set_flag(PUSHED);
    ctl_exit_menu();
    var_menu = (var_menu + 1) % MENU_SIZE;
    ctl_enter_menu(thegame);
    break;
  case SDLK_KP_PLUS:
  case SDLK_EQUALS:
  case SDLK_PLUS:
    var_push_button(BT_RZOOM);
    set_flag(PUSHED);
    increase_zoom();
    break;
  case SDLK_KP_MINUS:
  case SDLK_6:
  case SDLK_MINUS:
    var_push_button(BT_LZOOM);
    set_flag(PUSHED);
    decrease_zoom();
    break;
  default:
    break;
  }


  if (var_menu == MENU_PLAY && thegame && thegame->m) {

    switch (theevent->key.keysym.sym) {
    case SDLK_RIGHT:
      var_push_button(BT_RIGHT);
      set_flag(PUSHED);
      ctl_move_action(thegame, M_EAST);
      break;
    case SDLK_LEFT:
      var_push_button(BT_LEFT);
      set_flag(PUSHED);
      ctl_move_action(thegame, M_WEST);
      break;
    case SDLK_DOWN:
      var_push_button(BT_DOWN);
      set_flag(PUSHED);
      ctl_move_action(thegame, M_SOUTH);
      break;
    case SDLK_UP:
      var_push_button(BT_UP);
      set_flag(PUSHED);
      ctl_move_action(thegame, M_NORTH);
      break;
    case SDLK_w:
      var_push_button(BT_WAIT);
      set_flag(PUSHED);
      ctl_move_action(thegame, M_WAIT);
      break;
    case SDLK_LEFTBRACKET:
    case SDLK_CARET:
      var_push_button(BT_LSPEED);
      set_flag(PUSHED);
      decrease_speed();
      break;
    case SDLK_RIGHTBRACKET:
    case SDLK_DOLLAR:
      var_push_button(BT_RSPEED);
      set_flag(PUSHED);
      increase_speed();
      break;
    case SDLK_p:
      var_push_button(BT_CAMERA);
      set_flag(PUSHED);
      scroll_to_player(thegame);
      break;
    case SDLK_b:
      var_push_button(BT_BOMB);
      set_flag(PUSHED);
      ctl_bomb_action(thegame);
      break;
    case SDLK_a:
      var_push_button(BT_POLY);
      set_flag(PUSHED);
      ctl_poly_action(thegame);
      break;
    case SDLK_s:
      var_push_button(BT_RSTRAT);
      set_flag(PUSHED);
      var_mino_strat = (var_mino_strat + 1) % STR_SIZE;
      break;
    case SDLK_e:
      var_push_button(BT_EXIT);
      set_flag(PUSHED);
      if (thegame->m->objects[thegame->m->player] == EXIT) {
        set_flag(WIN);
      }
      break;
    case SDLK_u:
      var_push_button(BT_UNDO);
      set_flag(PUSHED);
      ctl_undo_action(thegame);
      break;
    case SDLK_r:
      var_push_button(BT_REDO);
      set_flag(PUSHED);
      ctl_redo_action(thegame);
      break;
    case SDLK_q:
      set_flag(QUIT);
      break;

    default:
      break;
    }
    return;

  }



  if (var_menu == MENU_GENE) {
    switch (theevent->key.keysym.sym) {
    case SDLK_g:
      var_push_button(BT_GEN);
      set_flag(PUSHED);
      set_flag(GENERATE);
      break;
    case SDLK_a:
      var_push_button(BT_RALGO);
      set_flag(PUSHED);
      gen_mode = (gen_mode + 1) % GEN_SIZE;
      break;
    case SDLK_o:
      var_push_button(BT_ROBJ);
      set_flag(PUSHED);
      obj_mode = (obj_mode + 1) % OBJ_SIZE;
      break;
    case SDLK_x:
      var_push_button(BT_X);
      set_flag(PUSHED);
      SDL_FlushEvent(SDL_TEXTINPUT);
      var_editbuffer = create_text_buffer();
      var_edit_mode = ED_X;
      break;
    case SDLK_y:
      var_push_button(BT_Y);
      set_flag(PUSHED);
      SDL_FlushEvent(SDL_TEXTINPUT);
      var_editbuffer = create_text_buffer();
      var_edit_mode = ED_Y;
      break;
    case SDLK_r:
      var_push_button(BT_MINO);
      set_flag(PUSHED);
      SDL_FlushEvent(SDL_TEXTINPUT);
      var_editbuffer = create_text_buffer();
      var_edit_mode = ED_MINO;
      break;
    case SDLK_b:
      var_push_button(BT_BRAID);
      set_flag(PUSHED);
      SDL_FlushEvent(SDL_TEXTINPUT);
      var_editbuffer = create_text_buffer();
      var_edit_mode = ED_BRAID;
      break;
    case SDLK_k:
      var_push_button(BT_RMASK);
      set_flag(PUSHED);
      ctl_next_mask_action();
      break;

    case SDLK_q:
      set_flag(QUIT);
      break;

    default:
      break;
    }
    return;

  }




  if (var_menu == MENU_SOSI && thegame && thegame->m) {

    switch (theevent->key.keysym.sym) {
    case SDLK_a:
      var_push_button(BT_SIMR);
      set_flag(PUSHED);
      var_sim_algorithm = (var_sim_algorithm + 1) % ALG_SIZE;
      break;
    case SDLK_s:
      var_push_button(BT_SIMGR);
      set_flag(PUSHED);
      var_sim_goal = (var_sim_goal + 1) % GOA_SIZE;
      break;
    case SDLK_t:
      var_push_button(BT_SIMMINOS);
      set_flag(PUSHED);
      var_sim_minos = !var_sim_minos;
      break;
    case SDLK_h:
      if (var_sim_algorithm == ALG_AST) {
        var_push_button(BT_HEUR);
        set_flag(PUSHED);
        var_heuristic = (var_heuristic + 1) % HEU_SIZE;
      }
      break;
    case SDLK_c:
      var_push_button(BT_SIMCOMP);
      set_flag(PUSHED);
      ctl_sim_compute(thegame);
      break;
    case SDLK_e:
      var_push_button(BT_SIMDELE);
      set_flag(PUSHED);
      ctl_sim_delete();
      break;
    case SDLK_r:
      var_push_button(BT_RED1);
      set_flag(PUSHED);
      ctl_update_redpath(thegame);
      break;
    case SDLK_g:
      var_push_button(BT_GREEN1);
      set_flag(PUSHED);
      ctl_update_greenpath(thegame);
      break;
    case SDLK_b:
      var_push_button(BT_BLUE1);
      set_flag(PUSHED);
      ctl_update_bluepath(thegame);
      break;
    default:
      break;

    }
  }

  if (var_menu == MENU_SOAD && thegame && thegame->m) {

    switch (theevent->key.keysym.sym) {
    case SDLK_a:
      var_push_button(BT_ADVR);
      set_flag(PUSHED);
      var_cmp_algorithm = (var_cmp_algorithm + 1) % CMP_SIZE;
      break;
    case SDLK_c:
      var_push_button(BT_ADVCOMP);
      set_flag(PUSHED);
      ctl_compute_complex(thegame);
      break;
    case SDLK_e:
      var_push_button(BT_ADVDELE);
      set_flag(PUSHED);
      ctl_delete_complex();
      break;
    case SDLK_d:
      var_push_button(BT_ADVMODER);
      set_flag(PUSHED);
      ctl_toggler_complex(thegame);
      break;
    case SDLK_p:
      var_push_button(BT_ADVP);
      set_flag(PUSHED);
      ctl_previous_complex(thegame);
      break;
    case SDLK_n:
      var_push_button(BT_ADVN);
      set_flag(PUSHED);
      ctl_next_complex(thegame);
      break;
    default:
      break;

    }
  }
}

/************************/
/*+ Gestion du solveur **/
/************************/



void ctl_sim_compute(game* thegame) {
  if (thegame && thegame->m) {
    // Libération de l'ancien parcours
    sim_free_search(var_sim_search);
    var_display_search = false;

    var_sim_search = salgo_funs[var_sim_algorithm](thegame, thegame->m->player, var_sim_goal, heu_funs[var_heuristic], var_sim_minos);
  }

}

void ctl_sim_delete(void) {
  if (var_sim_search) {
    sim_free_search(var_sim_search);
  }
  var_sim_search = NULL;
  var_display_search = false;
}










void ctl_launch_searchpath(void) {

  if (var_menu == MENU_SOSI && var_sim_search) {
    var_display_search = true;
    var_search_step = 0;
    return;
  }


}

void ctl_print_searchpath(void) {
  if (var_menu == MENU_SOSI && var_sim_search) {
    var_display_search = true;
    var_search_step = size_dyn(var_sim_search->search);
    return;
  }



}

void ctl_hide_searchpath(void) {
  var_display_search = false;
}

void ctl_update_redpath(game*) {

  if (var_menu == MENU_SOSI && var_sim_search && var_sim_search->path) {
    sim_freepath(var_path_red);
    var_path_red = NULL;
    var_path_red = sim_copypath(var_sim_search->path);
    return;
  }

}

void ctl_update_greenpath(game*) {

  if (var_menu == MENU_SOSI && var_sim_search && var_sim_search->path) {
    sim_freepath(var_path_green);
    var_path_green = NULL;
    var_path_green = sim_copypath(var_sim_search->path);
    return;
  }


}

void ctl_update_bluepath(game*) {

  if (var_menu == MENU_SOSI && var_sim_search && var_sim_search->path) {
    sim_freepath(var_path_blue);
    var_path_blue = NULL;
    var_path_blue = sim_copypath(var_sim_search->path);
    return;
  }


}

void ctl_delete_redpath(void) {
  sim_freepath(var_path_red);
  var_path_red = NULL;
}
void ctl_delete_greenpath(void) {
  sim_freepath(var_path_green);
  var_path_green = NULL;
}
void ctl_delete_bluepath(void) {
  sim_freepath(var_path_blue);
  var_path_blue = NULL;
}

void ctl_clear_paths(void) {
  ctl_delete_redpath();
  ctl_delete_greenpath();
  ctl_delete_bluepath();
}


void ctl_compute_complex(game* thegame) {
  cmp_free(var_adv_path);
  var_adv_mode = false;
  var_adv_num = 0;
  var_adv_path = cmp_funs[var_cmp_algorithm](thegame);
}

void ctl_delete_complex(void) {
  if (var_adv_path) {
    cmp_free(var_adv_path);
  }
  var_adv_path = NULL;
  var_adv_mode = 0;
  var_adv_num = 0;
}


void ctl_togglel_complex(game* thegame) {
  if (var_adv_path) {
    var_adv_mode = (var_adv_mode + 2) % 3;
    if (var_adv_mode == 2) {
      scroll_to_cell(thegame, var_adv_path->array[var_adv_num]->start);
    }
  }
}

void ctl_toggler_complex(game* thegame) {
  if (var_adv_path) {
    var_adv_mode = (var_adv_mode + 1) % 3;
    if (var_adv_mode == 2) {
      scroll_to_cell(thegame, var_adv_path->array[var_adv_num]->start);
    }
  }
}



void ctl_next_complex(game* thegame) {
  if (var_adv_path && var_adv_mode && var_adv_num < var_adv_path->size - 1) {
    var_adv_num++;
    scroll_to_cell(thegame, var_adv_path->array[var_adv_num]->start);
  }
}

void ctl_previous_complex(game* thegame) {
  if (var_adv_path && var_adv_mode && var_adv_num > 0) {
    var_adv_num--;
    scroll_to_cell(thegame, var_adv_path->array[var_adv_num]->start);
  }
}

void ctl_update_seg_num(game* thegame, int num) {
  if (var_adv_path && var_adv_mode && num > 0 && num <= var_adv_path->size) {
    var_adv_num = num - 1;
    scroll_to_cell(thegame, var_adv_path->array[var_adv_num]->start);
  }
}



/****************************/
/*+ Gestion des animations **/
/****************************/

void ctl_anim_reset(void) {
  var_frame = 0;
  var_crender = AN_IDLE;
  remove_flag(ANIMATED);
}


void ctl_anim_idle(game* thegame) {
  render_idle_game(thegame, flags[WIN]);
  if (var_menu == MENU_PLAY && flags[WIN]) {
    render_victory();
  }
}



void ctl_anim_walk(game* thegame) {

  render_walk_animation(thegame, var_frame, var_render_speed_level);
  if (var_frame >= 16 * var_render_speed_level - 1) {
    if (an_killed) {
      var_frame = 0;
      var_crender = AN_KILL;
    }
    else {
      ctl_anim_reset();
    }
  }
  else {
    var_frame++;
  }
}


void ctl_anim_kill(game* thegame) {

  render_kill_animation(thegame, var_frame, var_render_speed_level);

  if (var_frame >= 33 * var_render_speed_level - 1) {
    ctl_anim_reset();
  }
  else {
    var_frame++;
  }


}
void ctl_anim_bomb(game* thegame) {

  render_bombing(thegame, var_frame, var_render_speed_level);
  if (var_frame >= 8 * var_render_speed_level - 1) {

    if (an_killed) {
      var_frame = 0;
      var_crender = AN_KILL;
    }
    else {
      ctl_anim_reset();
    }
  }
  else {
    var_frame++;
  }

}
void ctl_anim_poly(game* thegame) {
  render_holy_poly(thegame, var_frame, 2 * var_render_speed_level);
  if (var_frame >= 11 * 2 * var_render_speed_level - 1) {
    ctl_anim_reset();
  }
  else {
    var_frame++;
  }

}






void ctl_anim_uwalk(game* thegame) {

  render_undo_walk_animation(thegame, var_frame, var_render_speed_level);
  if (var_frame >= 16 * var_render_speed_level - 1) {
    ctl_anim_reset();
  }
  else {
    var_frame++;
  }
}



void ctl_anim_ubomb(game* thegame) {

  render_undo_bombing(thegame, var_frame, var_render_speed_level);
  if (var_frame >= 8 * var_render_speed_level - 1) {

    /*     if (!var_checkwall) {
          del_wall_maze(thegame->m, thegame->m->player, var_active_turn->tbomb->bombdir);
        } */
    ctl_anim_reset();
  }
  else {
    var_frame++;
  }

}




void ctl_anim_upoly(game* thegame) {
  render_undo_holy_poly(thegame, var_frame, 2 * var_render_speed_level);
  if (var_frame >= 11 * 2 * var_render_speed_level - 1) {
    ctl_anim_reset();
  }
  else {
    var_frame++;
  }

}


void ctl_anim_ukillbomb(game* thegame) {


  render_undo_kill_animation(thegame, var_frame, var_render_speed_level);
  if (var_frame >= 33 * var_render_speed_level - 1) {
    var_frame = 0;
    var_crender = AN_UBOMB;
  }
  else {
    var_frame++;
  }


}

void ctl_anim_ukillwalk(game* thegame) {


  render_undo_kill_animation(thegame, var_frame, var_render_speed_level);
  if (var_frame >= 33 * var_render_speed_level - 1) {
    var_frame = 0;
    var_crender = AN_UWALK;
  }
  else {
    var_frame++;
  }


}




/*******************/
/*+ Boucle de jeu **/
/*******************/

void event_loop(void) {

  // Lecture du répertoire des masques.


  var_reset_buttons();
  // Déclaration de l'événement
  SDL_Event event;

  // Création du plateau original
  game* thegame = ctl_init_game(true);

  //thegame = NULL;

  // Boucle

  //flags[TEXTINPUT_MODE] = true;

  while (!flags[QUIT]) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        set_flag(QUIT);
        break;
      }

      // Lock du clavier tant qu'une touche est enfoncée
      if ((flags[KEYDOWN] && event.type != SDL_KEYUP)) {
        continue;
      }
      if (flags[KEYDOWN] && event.type == SDL_KEYUP) {
        flags[KEYDOWN] = false;
      }




      if (var_edit_mode != ED_NONE) {
        switch (event.type) {
        case SDL_TEXTINPUT:
          append_to_text_buffer(var_editbuffer, event.text.text);
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
          case SDLK_BACKSPACE:
            if (var_editbuffer->length > 0)
              remove_char(var_editbuffer);
            break;
          case SDLK_RETURN:
          case SDLK_KP_ENTER:
            ctl_update_from_buffer(thegame);
            //update_variables_from_buffer();
            free_text_buffer(var_editbuffer);
            var_editbuffer = NULL;
            var_edit_mode = ED_NONE;
            //reset_flags();
            break;
          default:
            break;
          }
        default:
          break;
        }
        continue;
      }





      // Gestion de la souris
      switch (event.type)
      {
      case SDL_MOUSEMOTION:
        ctl_mouse_motion(thegame, &event);
        break;
      case SDL_MOUSEBUTTONDOWN:
        ctl_mouse_click(thegame, &event);
        break;
      case SDL_MOUSEBUTTONUP:
        ctl_mouse_unclick(thegame, &event);
        break;

      default:
        break;
      }

      // Gestion du clavier
      if (event.type == SDL_KEYDOWN) {
        set_flag(KEYDOWN);
        ctl_keyboard(thegame, &event);
      }

    }


    if (var_display_search && var_sim_search && var_search_step < size_dyn(var_sim_search->search)) {
      var_search_step++;
    }


    // Affichage
    SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(BLACK));
    SDL_RenderClear(renderer);
    switch (var_crender)
    {
    case AN_NULL:
      render_null_game();
      break;
    case AN_IDLE:
      ctl_anim_idle(thegame);
      break;
    case AN_WALK:
      ctl_anim_walk(thegame);
      break;
    case AN_KILL:
      ctl_anim_kill(thegame);
      break;
    case AN_BOMB:
      ctl_anim_bomb(thegame);
      break;
    case AN_POLY:
      ctl_anim_poly(thegame);
      break;
    case AN_UWALK:
      ctl_anim_uwalk(thegame);
      break;
    case AN_UKILLB:
      ctl_anim_ukillbomb(thegame);
      break;
    case AN_UKILLW:
      ctl_anim_ukillwalk(thegame);
      break;
    case AN_UBOMB:
      ctl_anim_ubomb(thegame);
      break;
    case AN_UPOLY:
      ctl_anim_upoly(thegame);
      break;
    default:
      break;
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(5);

    // Réinitialisation des boutons
    if (flags[PUSHED] && SDL_GetTicks() - var_button_countdown >= 100) {
      var_reset_buttons();
      remove_flag(PUSHED);
    }

    // Génération
    if (flags[GENERATE]) {
      ctl_anim_reset();
      free_game(thegame);
      thegame = ctl_init_game(false);
    }
  }

  // Libération.
  ctl_free_gui();
  free_game(thegame);
}





