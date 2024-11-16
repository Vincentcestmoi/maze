#include "gui_rendering.h"



static inline int render_min(int a, int b) {
  return a < b ? a : b;
}

/*****************************/
/*+ Fonctions préliminaires **/
/*****************************/

void render_get_cellpos(int i, int j, SDL_Rect* pos) {
  pos->x = (i - var_startx) * 2 * var_tile_size + var_topleftx;
  pos->y = (j - var_starty) * 2 * var_tile_size + var_toplefty;
}



bool render_outofview(int i, int j) {
  if (i + 1 < var_startx || j + 1 < var_starty || i - 1 > var_endx || j - 1 > var_endy) {
    return true;
  }
  return false;
}

/**********************/
/*+ Affichage du sol **/
/**********************/

// Affichage d'un sol
void render_print_floor_tile(SDL_Rect* ppos, int s) {
  switch (s % 20)
  {
  case 2:
    SDL_RenderCopy(renderer, txt_floor, &clp_textures[CLP_FLOOR2], ppos);
    break;
  case 7:
    SDL_RenderCopy(renderer, txt_floor, &clp_textures[CLP_FLOOR3], ppos);
    break;
  case 11:
    SDL_RenderCopy(renderer, txt_floor, &clp_textures[CLP_FLOOR4], ppos);
    break;
  default:
    SDL_RenderCopy(renderer, txt_floor, &clp_textures[CLP_FLOOR1], ppos);
    break;
  }
}

// Affichage d'un sol
void render_print_floor_cell(int i, int j) {
  SDL_Rect ppos;
  ppos.h = var_tile_size;
  ppos.w = var_tile_size;
  render_get_cellpos(i, j, &ppos);
  render_print_floor_tile(&ppos, i + j);
  ppos.x += var_tile_size;
  render_print_floor_tile(&ppos, 3 * (i + j));
  ppos.y += var_tile_size;
  render_print_floor_tile(&ppos, 2 * (i + j) + 1);
  ppos.x -= var_tile_size;
  render_print_floor_tile(&ppos, 5 * (i + j) + 4);
}

void render_print_block_cell(int i, int j) {
  SDL_Rect ppos;
  ppos.h = var_tile_size;
  ppos.w = var_tile_size;
  render_get_cellpos(i, j, &ppos);
  SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_FILLS], &ppos);
  ppos.x += var_tile_size;
  SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_FILLS], &ppos);
  ppos.y += var_tile_size;
  SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_FILLS], &ppos);
  ppos.x -= var_tile_size;
  SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_FILLS], &ppos);
}


// Affichage de tout le sol
void render_whole_floor(game* thegame) {
  // Protection
  if (thegame == NULL || thegame->m == NULL) {
    return;
  }
  int endx = render_min(var_endx, thegame->m->hsize);
  int endy = render_min(var_endy, thegame->m->vsize);
  for (int i = var_startx; i < endx; i++) {
    for (int j = var_starty; j < endy;j++) {
      if (is_reach_maze(thegame->m, thegame->m->hsize * j + i)) {
        render_print_floor_cell(i, j);

      }
      else if (valid_maze(thegame->m, thegame->m->hsize * j + i)) {
        render_print_block_cell(i, j);
      }
    }
  }
}

/**************************/
/*+ Affichage des restes **/
/**************************/

void render_print_remains(game* thegame, bool player_alive, bool* minotaurs_alive) {
  // Protection
  if (thegame == NULL || thegame->m == NULL || thegame->m->minotaurs == NULL) {
    return;
  }
  SDL_Rect ppos;
  ppos.h = 64 * var_render_zoom;
  ppos.w = 64 * var_render_zoom;
  if (!player_alive) {
    render_get_cellpos(thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize, &ppos);
    ppos.x += 6 * var_render_zoom;
    ppos.y += 6 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_BLOOD], &ppos);
  }
  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    if (!minotaurs_alive[i]) {
      render_get_cellpos(thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize, &ppos);
      ppos.x += 6 * var_render_zoom;
      ppos.y += 14 * var_render_zoom;
      SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_ASHES], &ppos);
    }
  }
}

/************************/
/*+ Affichage des murs **/
/************************/


// Affichage d'un mur nord
void render_printhwall(SDL_Rect* ppos, int s) {
  switch (s % 17)
  {
  case 14:
    SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_HWALL2], ppos);
    break;
  case 5:
    SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_HWALL3], ppos);
    break;
  default:
    SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_HWALL1], ppos);
    break;
  }
}

bool render_should_print_wall(maze* m, int cell, cardinal dir) {
  int acell = get_adj_maze(m, cell, dir);
  if (!(is_reach_maze(m, cell) || is_reach_maze(m, acell))) {
    return false;
  }
  return has_wall_maze(m, cell, dir);
}





// Affichage d'un cellule complète
void render_print_walls_cell(maze* m, int i, int j) {
  SDL_Rect ppos;
  ppos.h = var_tile_size;
  ppos.w = var_tile_size;
  render_get_cellpos(i, j, &ppos);

  // Plusieurs cas

  // Cellules accessibles
  int cell = j * m->hsize + i;
  int lcell = m->hsize * j + i - 1;
  int ucell = m->hsize * (j - 1) + i;
  int lucell = m->hsize * (j - 1) + i - 1;
  if ((i < m->hsize && j < m->vsize) && (is_reach_maze(m, cell) || (is_reach_maze(m, lcell) && is_reach_maze(m, ucell)))) {



    // Tile Nord-Ouest
    if (render_should_print_wall(m, cell, NORTH)) {
      // Si il y a un mur Nord et un Ouest à afficher.
      if (render_should_print_wall(m, cell, WEST)) {
        // Cellule 1: Tile contenant un coin Nord-Ouest
        SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_LANGLE], &ppos);
      }
      // Si il y a juste un mur Nord à afficher.
      else {
        // Cellule 1: Tile contenant un mur Nord
        render_printhwall(&ppos, i + j);
      }
    }
    else {
      // Si il y a un mur Ouest à afficher où que la cellule au-dessus à un mur Ouest à afficher
      if (render_should_print_wall(m, cell, WEST) || (j > 0 && render_should_print_wall(m, get_adj_maze(m, cell, NORTH), WEST))) {
        // Cellule 1: Tile contenant un mur Ouest
        SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_LWALL], &ppos);
      }
    }
    // Tile Nord-Est
    ppos.x += var_tile_size;
    if (render_should_print_wall(m, cell, NORTH)) {
      // Tile contenant un mur Nord
      render_printhwall(&ppos, i + 2 * j);
    }

    // Pas de tile Sud-Est

    // Tile Sud_Ouest
    ppos.y += var_tile_size;
    ppos.x -= var_tile_size;
    if (render_should_print_wall(m, cell, WEST)) {
      SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_LWALL], &ppos);
    }
    return;
  }


  // Cellules à droite d'une cellule accessible
  if (i > 0 && j < m->vsize && is_reach_maze(m, lcell)) {
    if (render_should_print_wall(m, lcell, NORTH) || render_should_print_wall(m, lcell, EAST)) {
      SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_LWALL], &ppos);
    }
    ppos.y += var_tile_size;
    if (render_should_print_wall(m, lcell, EAST)) {
      SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_LWALL], &ppos);
    }
    return;
  }

  // Cellules en dessous d'une cellule accessible
  if (i < m->hsize && is_reach_maze(m, ucell)) {
    if (render_should_print_wall(m, ucell, SOUTH) || render_should_print_wall(m, ucell, WEST)) {
      render_printhwall(&ppos, i + 2 * j);
    }
    ppos.x += var_tile_size;
    if (render_should_print_wall(m, ucell, SOUTH)) {
      render_printhwall(&ppos, 3 * i + j);
    }
    return;
  }

  // Cellules à droite d'une cellule accessible
  if (i > 0 && j <= m->vsize && i <= m->hsize && is_reach_maze(m, lucell)) {
    if (render_should_print_wall(m, lucell, SOUTH) || render_should_print_wall(m, lucell, EAST)) {
      SDL_RenderCopy(renderer, txt_walls, &clp_textures[CLP_LWALL], &ppos);
    }
    return;
  }





}


// Affichage tous les murs
void render_all_walls(maze* m) {
  int endx = render_min(var_endx, m->hsize + 1);
  int endy = render_min(var_endy, m->vsize + 1);
  for (int i = var_startx; i < endx; i++) {
    for (int j = var_starty; j < endy;j++) {
      render_print_walls_cell(m, i, j);
    }
  }
}

/***************************************/
/*+ Affichage des chemins et parcours **/
/***************************************/


// Affichage d'un maillon
void render_print_link(game* thegame, int start, cardinal dir) {

  SDL_Rect pos;
  render_get_cellpos(start % thegame->m->hsize, start / thegame->m->hsize, &pos);



  pos.x += var_tile_size / 2 + var_tile_size / 8;
  pos.y += var_tile_size;
  switch (dir)
  {
  case NORTH:
    pos.y -= 2 * var_tile_size;
    pos.w = var_tile_size;
    pos.h = 2 * var_tile_size;
    break;
  case EAST:
    pos.x += var_tile_size;
    pos.w = 2 * var_tile_size;
    pos.h = var_tile_size;
    break;
  case SOUTH:
    pos.y += var_tile_size;
    pos.w = var_tile_size;
    pos.h = 2 * var_tile_size;
    break;
  case WEST:
    pos.x -= 2 * var_tile_size;
    pos.w = 2 * var_tile_size;
    pos.h = var_tile_size;
    break;

  default:
    break;
  }


  SDL_RenderFillRect(renderer, &pos);

}

// Affichage d'un seul chemin
void render_print_path(game* thegame, sim_path* thepath, SDL_Color col) {
  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(col));

  int start = thepath->start;
  int i = start % thegame->m->hsize;
  int j = start / thegame->m->hsize;

  if (!render_outofview(i, j)) {
    SDL_Rect pos;


    render_get_cellpos(start % thegame->m->hsize, start / thegame->m->hsize, &pos);

    pos.x += var_tile_size / 2 + var_tile_size / 8;
    pos.y += var_tile_size;
    pos.h = var_tile_size;
    pos.w = var_tile_size;


    SDL_RenderFillRect(renderer, &pos);

  }

  sim_move_seq* thelist = thepath->moves;



  while (thelist) {
    if (thelist->direction == M_WAIT) {
      thelist = thelist->next;
      continue;
    }
    if (!render_outofview(i, j)) {
      render_print_link(thegame, start, (cardinal)thelist->direction);
    }
    start = get_adj_maze(thegame->m, start, (cardinal)thelist->direction);
    i = start % thegame->m->hsize;
    j = start / thegame->m->hsize;
    thelist = thelist->next;
  }


  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
}


// Affichage d'un parcours complet
void render_search(game* thegame) {
  if (!var_sim_search) {
    return;
  }

  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(ORANGET));
  SDL_Rect pos;
  pos.x = 0;
  pos.y = 0;
  pos.w = var_tile_size;
  pos.h = var_tile_size;


  for (int k = 0;k < var_search_step;k++) {
    int i = var_sim_search->search->array[k] % thegame->m->hsize;
    int j = var_sim_search->search->array[k] / thegame->m->hsize;
    if (render_outofview(i, j)) {
      continue;
    }
    render_get_cellpos(i, j, &pos);
    SDL_RenderFillRect(renderer, &pos);
    pos.x += var_tile_size;
    SDL_RenderFillRect(renderer, &pos);
    pos.y += var_tile_size;
    SDL_RenderFillRect(renderer, &pos);
    pos.x -= var_tile_size;
    SDL_RenderFillRect(renderer, &pos);
  }

  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));

}


static void render_draw_pathline(int cella, int cellb, int hsize) {

  int ia = cella % hsize;
  int ja = cella / hsize;
  int ib = cellb % hsize;
  int jb = cellb / hsize;
  if ((ia > var_endx && ib > var_endx) || (ia < var_startx && ib < var_startx) || (ja > var_endy && jb > var_endy) || (ja < var_starty && jb < var_starty)) {
    return;
  }
  int decx = var_tile_size + var_tile_size / 4;
  int decy = var_tile_size + var_tile_size / 2;
  int posxa = (ia - var_startx) * 2 * var_tile_size + var_topleftx + decx;
  int posya = (ja - var_starty) * 2 * var_tile_size + var_toplefty + decy;
  int posxb = (ib - var_startx) * 2 * var_tile_size + var_topleftx + decx;
  int posyb = (jb - var_starty) * 2 * var_tile_size + var_toplefty + decy;

  thickLineRGBA(renderer, posxa, posya, posxb, posyb, 1 + var_render_zoom_level, COLOR_TUPLE(CARML));
}

void render_print_complex_path(game* thegame, cmp_search* path) {
  if (!thegame || !path || !var_adv_mode) {
    return;
  }

  if (var_adv_mode == 2) {
    render_draw_pathline(path->array[var_adv_num]->start, path->array[var_adv_num]->end, thegame->m->hsize);
    SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return;
  }

  for (int i = 0; i < path->size;i++) {
    render_draw_pathline(path->array[i]->start, path->array[i]->end, thegame->m->hsize);

  }
  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

/**************************/
/*+ Affichage des objets **/
/**************************/

void render_one_object(int i, int j, object obj) {

  // Rectangle pour dessiner les objets à ramasser


  SDL_Rect position;
  position.w = var_tile_size;
  position.h = var_tile_size;
  render_get_cellpos(i, j, &position);

  /*
  position.x = offseth + 22 * var_render_zoom;
  position.y = offsetv + 28 * var_render_zoom;
 */
  switch (obj)
  {
  case SMALLT:
    position.x += 22 * var_render_zoom;
    position.y += 28 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_TREA1], &position);
    break;
  case MEDT:
    position.x += 22 * var_render_zoom;
    position.y += 28 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_TREA2], &position);
    break;
  case LARGET:
    position.x += 22 * var_render_zoom;
    position.y += 28 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_TREA3], &position);
    break;
  case BOMB:
    position.x += 22 * var_render_zoom;
    position.y += 28 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_bomb, &clp_textures[CLP_BOMBS], &position);
    break;
  case POLY:
    position.x += 22 * var_render_zoom;
    position.y += 28 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_POLYS], &position);
    break;
  case EXIT:
    position.x += 22 * var_render_zoom;
    position.y += 28 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_EXITS], &position);
    break;
  default:
    break;
  }
}

void render_all_objects(maze* m) {
  int endx = render_min(var_endx, m->hsize);
  int endy = render_min(var_endy, m->vsize);
  for (int i = var_startx; i < endx; i++) {
    for (int j = var_starty; j < endy;j++) {
      if (valid_maze(m, m->hsize * j + i)) {
        render_one_object(i, j, m->objects[m->hsize * j + i]);
      }
    }
  }
}

/*************************************************/
/*+ Affichage d'une scène (tout sauf les toons) **/
/*************************************************/

void render_scene(game* thegame) {

  // Affichage du sol
  render_whole_floor(thegame);
  render_print_remains(thegame, thegame->player_alive, thegame->minotaurs_alive);

  if (var_display_search && var_sim_search) {
    render_search(thegame);
  }
  // Affichage des murs
  render_all_walls(thegame->m);

  if (var_path_red != NULL) {
    render_print_path(thegame, var_path_red, REDT);
  }


  if (var_path_green != NULL) {
    render_print_path(thegame, var_path_green, GREENT);
  }

  if (var_path_blue != NULL) {
    render_print_path(thegame, var_path_blue, BLUET);
  }

  if (var_adv_path && var_adv_mode == 2) {
    render_print_path(thegame, var_adv_path->array[var_adv_num], CARMT);

  }

  // Affichage des objets
  render_all_objects(thegame->m);



}

void render_scene_unkill(game* thegame, bool player_alive, bool* minotaurs_alive) {
  CHECK_NULL(1, "The maze");
  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
  SDL_RenderClear(renderer);

  // Affichage du sol
  render_whole_floor(thegame);
  render_print_remains(thegame, player_alive, minotaurs_alive);

  if (var_display_search && var_sim_search) {
    render_search(thegame);
  }

  // Affichage des murs
  render_all_walls(thegame->m);

  if (var_path_red != NULL) {
    render_print_path(thegame, var_path_red, REDT);
  }


  if (var_path_green != NULL) {
    render_print_path(thegame, var_path_green, GREENT);
  }

  if (var_path_blue != NULL) {
    render_print_path(thegame, var_path_blue, BLUET);
  }

  if (var_adv_path && var_adv_mode == 2) {
    render_print_path(thegame, var_adv_path->array[var_adv_num], CARMT);
  }

  // Affichage des objets
  render_all_objects(thegame->m);




}



/*************************/
/*+ Affichage des toons **/
/*************************/

void render_toon_idle(toon_type thetype, bool alive, cardinal dir, int i, int j) {
  if (render_outofview(i, j)) {
    return;
  }
  SDL_Texture* thetexture;
  // Récupération de la texture du bon toon
  switch (thetype)
  {
  case PLAYER:
    thetexture = txt_player;
    break;
  case MINOTAUR:
    thetexture = txt_minotaur;
    break;
  default:
    return;
    break;
  }

  SDL_Rect position;
  position.h = 64 * var_render_zoom;
  position.w = 64 * var_render_zoom;
  render_get_cellpos(i, j, &position);
  position.x += 6 * var_render_zoom;

  if (alive) {
    int offset;
    switch (dir)
    {
    case NORTH:
      offset = CLP_NWALK1;
      break;
    case EAST:
      offset = CLP_EWALK1;
      break;
    case SOUTH:
      offset = CLP_SWALK1;
      break;
    case WEST:
      offset = CLP_WWALK1;
      break;
    default:
      offset = CLP_SWALK1;
      break;
    }
    SDL_RenderCopy(renderer, thetexture, &clp_textures[offset], &position);
  }
  else if (thetype == PLAYER) {
    SDL_RenderCopy(renderer, thetexture, &clp_textures[CLP_DEAD], &position);
  }
}

void render_toon_walking(toon_type thetype, move tdir, move pdir, int i, int j, int frame) {
  if (render_outofview(i, j) || tdir == M_WAIT) {
    return;
  }

  SDL_Texture* thetexture;
  // Récupération de la texture du bon toon
  switch (thetype)
  {
  case PLAYER:
    thetexture = txt_player;
    break;
  case MINOTAUR:
    thetexture = txt_minotaur;
    break;
  default:
    return;
    break;
  }

  int offset = 0;
  int shiftx = 0;
  int shifty = 0;

  switch (tdir)
  {
  case M_NORTH:
    offset = CLP_NWALK1;
    if (pdir == M_NORTH) {
      shifty = 0;
    }
    if (pdir == M_SOUTH) {
      shifty = -4;
    }
    else {
      shifty = -4;
    }
    break;
  case M_EAST:
    if (pdir == M_EAST) {
      shiftx = 0;
    }
    if (pdir == M_WEST) {
      shiftx = 4;
    }
    else {
      shiftx = 4;
    }
    offset = CLP_EWALK1;
    break;
  case M_SOUTH:
    if (pdir == M_SOUTH) {
      shifty = 0;
    }
    if (pdir == M_NORTH) {
      shifty = 4;
    }
    else {
      shifty = 4;
    }
    offset = CLP_SWALK1;
    break;
  case WEST:
    if (pdir == M_WEST) {
      shiftx = 0;
    }
    if (pdir == M_EAST) {
      shiftx = -4;
    }
    else {
      shiftx = -4;
    }
    offset = CLP_WWALK1;
    break;
  default:
    return;
    break;
  }

  SDL_Rect position;
  position.h = 64 * var_render_zoom;
  position.w = 64 * var_render_zoom;
  render_get_cellpos(i, j, &position);
  position.x += (6 + shiftx * frame) * var_render_zoom;
  position.y += frame * shifty * var_render_zoom;
  SDL_RenderCopy(renderer, thetexture, &clp_textures[offset + frame % 9], &position);
}


void render_minotaur_attacking(cardinal dir, int i, int j, int frame) {
  if (render_outofview(i, j)) {
    return;
  }
  int offset;
  int xshift = 0;
  int yshift = 0;
  int pi = i;
  int pj = j;
  switch (dir)
  {
  case NORTH:
    offset = CLP_NSWING1;
    yshift -= 48;
    pj--;
    break;
  case EAST:
    offset = CLP_ESWING1;
    xshift += 40;
    pi++;
    break;
  case SOUTH:
    offset = CLP_SSWING1;
    yshift += 48;
    pj++;
    break;
  case WEST:
    offset = CLP_WSWING1;
    xshift -= 40;
    pi--;
    break;
  default:
    return;
    break;
  }

  SDL_Rect newpos;
  newpos.h = 192 * var_render_zoom;
  newpos.w = 192 * var_render_zoom;
  render_get_cellpos(i, j, &newpos);



  newpos.x += (-64 + xshift) * var_render_zoom;
  newpos.y += (-64 + yshift) * var_render_zoom;

  if (frame % 6 >= 4) {
    SDL_Rect bpos;
    bpos.h = 64 * var_render_zoom;
    bpos.w = 64 * var_render_zoom;
    render_get_cellpos(pi, pj, &bpos);
    bpos.x += 6 * var_render_zoom;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_SLASH], &bpos);
  }


  SDL_RenderCopy(renderer, txt_minotaur, &clp_textures[offset + (frame % 6)], &newpos);






}

/***********************************/
/*+ Affichage des toons immobiles **/
/***********************************/

void render_all_toons_idle(game* thegame) {
  render_toon_idle(PLAYER, thegame->player_alive, thegame->player_dir, thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize);
  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize);
  }
}


void render_all_minotaurs_idle(game* thegame) {
  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize);
  }

}

void render_idle_game(game* thegame, bool player_won) {

  // Affichage de la scene
  render_scene(thegame);
  // Affichage des toons
  if (player_won) {

    render_all_minotaurs_idle(thegame);

  }
  else {
    render_all_toons_idle(thegame);
  }

  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }

  // Affichage des informations
  render_all_informations(thegame);
}

void render_null_game(void) {
  render_all_informations(NULL);
}




/********************************/
/*+ Affichage d'un déplacement **/
/********************************/




void walk_shift_screen(cardinal direc) {
  switch (direc)
  {
  case NORTH:
    scroll_manual(0, 4 * var_render_zoom);
    break;
  case EAST:
    scroll_manual(-4 * var_render_zoom, 0);
    break;
  case SOUTH:
    scroll_manual(0, -4 * var_render_zoom);
    break;
  case WEST:
    scroll_manual(4 * var_render_zoom, 0);
    break;
  default:
    break;
  }

}

void render_all_toons_move(game* thegame, int frame) {
  int tx = thegame->m->player % thegame->m->hsize;
  int ty = thegame->m->player / thegame->m->hsize;
  switch (an_player_move)
  {
  case M_WAIT:
    render_toon_idle(PLAYER, true, thegame->player_dir, tx, ty);
    break;
  case M_NORTH:
    ty++;
    render_toon_walking(PLAYER, an_player_move, an_player_move, tx, ty, frame);
    break;
  case M_EAST:
    tx--;
    render_toon_walking(PLAYER, an_player_move, an_player_move, tx, ty, frame);
    break;
  case M_SOUTH:
    ty--;
    render_toon_walking(PLAYER, an_player_move, an_player_move, tx, ty, frame);
    break;
  case M_WEST:
    tx++;
    render_toon_walking(PLAYER, an_player_move, an_player_move, tx, ty, frame);
    break;
  default:
    break;
  }




  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    tx = thegame->m->minotaurs[i] % thegame->m->hsize;
    ty = thegame->m->minotaurs[i] / thegame->m->hsize;
    switch (an_minotaurs_move[i])
    {
    case M_WAIT:
      render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], tx, ty);
      break;
    case M_NORTH:
      ty++;
      render_toon_walking(MINOTAUR, an_minotaurs_move[i], an_player_move, tx, ty, frame);
      break;
    case M_EAST:
      tx--;
      render_toon_walking(MINOTAUR, an_minotaurs_move[i], an_player_move, tx, ty, frame);
      break;
    case M_SOUTH:
      ty--;
      render_toon_walking(MINOTAUR, an_minotaurs_move[i], an_player_move, tx, ty, frame);
      break;
    case M_WEST:
      tx++;
      render_toon_walking(MINOTAUR, an_minotaurs_move[i], an_player_move, tx, ty, frame);
      break;
    default:

      break;
    }
  }
}


void render_walk_animation(game* thegame, int frame, int speed) {
  CHECK_NULL(1, thegame, "The game");
  // Affichage de la scene
  render_scene_unkill(thegame, true, thegame->minotaurs_alive);
  render_one_object(thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize, an_obj_taken);
  render_all_toons_move(thegame, frame / speed);
  if (frame % speed == speed - 1) {
    walk_shift_screen((cardinal)an_player_move);
  }

  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }
  // Affichage du masque
  render_all_informations(thegame);
}



void render_all_toons_move_back(game* thegame, int frame) {
  int tx = thegame->m->player % thegame->m->hsize;
  int ty = thegame->m->player / thegame->m->hsize;

  switch (an_player_move)
  {
  case M_WAIT:
    render_toon_idle(PLAYER, true, thegame->player_dir, tx, ty);
    break;
  default:
    render_toon_walking(PLAYER, an_player_move, an_player_move, tx, ty, frame);
    break;
  }




  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    tx = thegame->m->minotaurs[i] % thegame->m->hsize;
    ty = thegame->m->minotaurs[i] / thegame->m->hsize;
    switch (an_minotaurs_move[i])
    {
    case M_WAIT:
      render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], tx, ty);
      break;
    default:
      render_toon_walking(MINOTAUR, an_minotaurs_move[i], an_player_move, tx, ty, frame);

      break;
    }
  }
}


void render_undo_walk_animation(game* thegame, int frame, int speed) {
  CHECK_NULL(1, thegame, "The game");





  // Affichage de la scene
  render_scene(thegame);

  //render_one_object(thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize, theturn->tmove->obj);
  render_all_toons_move_back(thegame, 15 - frame / speed);
  if (frame % speed == speed - 1 && an_player_move != M_WAIT) {

    walk_shift_screen((((cardinal)an_player_move) + 2) % 4);
  }

  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }
  // Affichage du masque
  render_all_informations(thegame);
  //}
}


/******************************************/
/*+ Affichage d'une attaque de minotaure **/
/******************************************/




void render_kill_animation(game* thegame, int frame, int speed) {

  // Affichage de la scene
  render_scene_unkill(thegame, true, thegame->minotaurs_alive);

  render_toon_idle(PLAYER, true, thegame->player_dir, thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize);
  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    if (i != an_killer) {
      render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize);
    }
  }

  if (frame / speed < 9) {
    render_toon_walking(MINOTAUR, (move)an_killdir, M_WAIT, thegame->m->minotaurs[an_killer] % thegame->m->hsize, thegame->m->minotaurs[an_killer] / thegame->m->hsize, frame / speed);
  }
  else {
    render_minotaur_attacking(an_killdir, thegame->m->minotaurs[an_killer] % thegame->m->hsize, thegame->m->minotaurs[an_killer] / thegame->m->hsize, frame / speed - 9);
  }

  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }

  render_all_informations(thegame);

}


void render_undo_kill_animation(game* thegame, int frame, int speed) {
  CHECK_NULL(1, thegame, "The game");


  // Affichage de la scene
  render_scene(thegame);


  render_toon_idle(PLAYER, true, thegame->player_dir, an_player_pos % thegame->m->hsize, an_player_pos / thegame->m->hsize);
  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    if (i != an_killer) {
      render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], (an_minotaurs_pos[i] % thegame->m->hsize), (an_minotaurs_pos[i] / thegame->m->hsize));
    }
  }

  if (frame / speed < 24) {
    render_minotaur_attacking(an_killdir, an_minotaurs_pos[an_killer] % thegame->m->hsize, an_minotaurs_pos[an_killer] / thegame->m->hsize, 23 - frame / speed);
  }
  else {
    render_toon_walking(MINOTAUR, (move)an_killdir, M_WAIT, an_minotaurs_pos[an_killer] % thegame->m->hsize, an_minotaurs_pos[an_killer] / thegame->m->hsize, 8 - (frame / speed - 24));
  }

  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }
  // Affichage du masque
  render_all_informations(thegame);
}

/********************************/
/*+ Affichage d'un poly d'algo **/
/********************************/




void render_holy_poly(game* thegame, int frame, int speed) {
  CHECK_NULL(1, thegame, "The game");


  /*
    bool minos[thegame->m->nb_minotaurs];

    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      minos[i] = thegame->minotaurs_alive[i] || var_active_turn->minokilled[i];
    } */
  SDL_Rect pos;
  pos.w = 100 * var_render_zoom;
  pos.h = 175 * var_render_zoom;
  SDL_Rect impactpos;
  impactpos.w = 192 * var_render_zoom;
  impactpos.h = 192 * var_render_zoom;



  // Affichage de la scene
  render_scene_unkill(thegame, true, an_minotaurs_alive);

  if (frame / speed < 6) {
    render_toon_idle(PLAYER, thegame->player_alive, thegame->player_dir, thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize);
    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      render_toon_idle(MINOTAUR, an_minotaurs_alive[i], thegame->minotaurs_dirs[i], thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize);
    }
    // Affichage des éclairs
    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      if (an_minotaurs_alive[i] != thegame->minotaurs_alive[i]) {
        render_get_cellpos(thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize, &pos);
        pos.x -= 14 * var_render_zoom;
        pos.y -= 130 * var_render_zoom;
        SDL_RenderCopy(renderer, txt_lightning, &clp_textures[CLP_LIGHT1 + frame / speed], &pos);
      }
    }
  }
  else {

    render_all_toons_idle(thegame);
    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      if (an_minotaurs_alive[i] != thegame->minotaurs_alive[i]) {
        if (frame / speed < 9) {
          render_get_cellpos(thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize, &pos);
          pos.x -= 14 * var_render_zoom;
          pos.y -= 130 * var_render_zoom;
          SDL_RenderCopy(renderer, txt_lightning, &clp_textures[CLP_LIGHT1 + 5], &pos);
        }
        render_get_cellpos(thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize, &impactpos);
        impactpos.x -= 58 * var_render_zoom;
        impactpos.y -= 54 * var_render_zoom;
        SDL_RenderCopy(renderer, txt_impact, &clp_textures[CLP_IMPACT1 + frame / speed - 6], &impactpos);
      }

    }

  }
  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }
  render_all_informations(thegame);
}

void render_undo_holy_poly(game* thegame, int frame, int speed) {
  CHECK_NULL(1, thegame, "The game");


  SDL_Rect pos;
  pos.w = 100 * var_render_zoom;
  pos.h = 175 * var_render_zoom;
  SDL_Rect impactpos;
  impactpos.w = 192 * var_render_zoom;
  impactpos.h = 192 * var_render_zoom;




  // Affichage de la scene
  render_scene(thegame);


  render_toon_idle(PLAYER, thegame->player_alive, thegame->player_dir, thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize);
  for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
    render_toon_idle(MINOTAUR, an_minotaurs_alive[i], thegame->minotaurs_dirs[i], thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize);

  }

  if (frame / speed < 6) {
    // Affichage des minotaures
    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      if (an_minotaurs_alive[i] != thegame->minotaurs_alive[i]) {
        if (2 < frame / speed) {
          render_get_cellpos(thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize, &pos);
          pos.x -= 14 * var_render_zoom;
          pos.y -= 130 * var_render_zoom;
          SDL_RenderCopy(renderer, txt_lightning, &clp_textures[CLP_LIGHT1 + 5], &pos);
        }
        render_get_cellpos(thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize, &impactpos);
        impactpos.x -= 58 * var_render_zoom;
        impactpos.y -= 54 * var_render_zoom;
        SDL_RenderCopy(renderer, txt_impact, &clp_textures[CLP_IMPACT1 + 5 - frame / speed], &impactpos);
      }

    }
  }
  else {
    render_all_toons_idle(thegame);


    // Affichage des éclairs
    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      if (an_minotaurs_alive[i] != thegame->minotaurs_alive[i]) {
        render_get_cellpos(thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize, &pos);
        pos.x -= 14 * var_render_zoom;
        pos.y -= 130 * var_render_zoom;
        SDL_RenderCopy(renderer, txt_lightning, &clp_textures[CLP_LIGHT1 + 5 - (frame / speed - 6)], &pos);
      }
    }



  }
  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }
  // Affichage du masque
  render_all_informations(thegame);




















}




/***************************/
/*+ Affichage d'une bombe **/
/***************************/



// Bombs
void render_bombing(game* thegame, int frame, int speed) {
  CHECK_NULL(1, thegame, "The game");


  SDL_Rect pos;
  render_get_cellpos(thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize, &pos);
  pos.w = 64 * var_render_zoom;
  pos.h = 64 * var_render_zoom;

  switch (an_bombdir)
  {
  case NORTH:
    pos.y -= 20 * var_render_zoom;
    pos.x += 8 * var_render_zoom;
    break;
  case EAST:
    pos.x += 40 * var_render_zoom;
    pos.y += 10 * var_render_zoom;
    break;
  case SOUTH:
    pos.y += 44 * var_render_zoom;
    pos.x += 8 * var_render_zoom;
    break;
  case WEST:
    pos.x -= 28 * var_render_zoom;
    pos.y += 10 * var_render_zoom;
    break;

  default:
    break;
  }





  if (frame / speed == 3 && an_destroyed) {
    del_wall_maze(thegame->m, thegame->m->player, an_bombdir);
  }


  // Affichage de la scene

  render_scene_unkill(thegame, true, thegame->minotaurs_alive);

  if (an_bombdir == SOUTH) {

    render_toon_idle(PLAYER, true, thegame->player_dir, thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize);
    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize);

    }

    SDL_RenderCopy(renderer, txt_explosion, &clp_textures[CLP_EXPLO1 + frame / speed], &pos);


  }
  else {

    SDL_RenderCopy(renderer, txt_explosion, &clp_textures[CLP_EXPLO1 + frame / speed], &pos);

    render_toon_idle(PLAYER, true, thegame->player_dir, thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize);
    for (int i = 0; i < thegame->m->nb_minotaurs;i++) {
      render_toon_idle(MINOTAUR, thegame->minotaurs_alive[i], thegame->minotaurs_dirs[i], thegame->m->minotaurs[i] % thegame->m->hsize, thegame->m->minotaurs[i] / thegame->m->hsize);

    }

  }

  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }
  // Affichage du masque
  render_all_informations(thegame);







  //}

}



void render_undo_bombing(game* thegame, int frame, int speed) {
  CHECK_NULL(1, thegame, "The game");

  SDL_Rect pos;
  render_get_cellpos(thegame->m->player % thegame->m->hsize, thegame->m->player / thegame->m->hsize, &pos);
  pos.w = 64 * var_render_zoom;
  pos.h = 64 * var_render_zoom;

  switch (an_bombdir)
  {
  case NORTH:
    pos.y -= 20 * var_render_zoom;
    pos.x += 8 * var_render_zoom;
    break;
  case EAST:
    pos.x += 40 * var_render_zoom;
    pos.y += 10 * var_render_zoom;
    break;
  case SOUTH:
    pos.y += 44 * var_render_zoom;
    pos.x += 8 * var_render_zoom;
    break;
  case WEST:
    pos.x -= 28 * var_render_zoom;
    pos.y += 10 * var_render_zoom;
    break;

  default:
    break;
  }




  if ((7 - frame / speed) == 3 && an_destroyed) {
    build_wall_maze(thegame->m, thegame->m->player, an_bombdir);
  }


  // Affichage de la scene
  render_scene(thegame);
  if (an_bombdir == SOUTH) {

    render_all_toons_idle(thegame);

    SDL_RenderCopy(renderer, txt_explosion, &clp_textures[CLP_EXPLO1 + 7 - frame / speed], &pos);


  }
  else {

    SDL_RenderCopy(renderer, txt_explosion, &clp_textures[CLP_EXPLO1 + 7 - frame / speed], &pos);

    render_all_toons_idle(thegame);

  }

  if (var_adv_path) {
    render_print_complex_path(thegame, var_adv_path);
  }
  // Affichage du masque
  render_all_informations(thegame);







  //}

}






























