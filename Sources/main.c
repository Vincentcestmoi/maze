#include "main.h"

int main(void) {
  atexit(shutdown_SDL);

  // Inialisation du générateur de nombres aléatoires.
  srand((unsigned int)(time(NULL)));

  // Initialisation des paramètres de l'interface.
  var_screen_height = WINDOW_MAZEHEIGHT;
  var_screen_width = WINDOW_MAZEWIDTH + 522;
  var_maze_height = WINDOW_MAZEHEIGHT;
  var_maze_width = WINDOW_MAZEWIDTH;
  var_info_height = WINDOW_MAZEHEIGHT;
  var_info_width = 522;
  var_info_mid = (var_maze_width + var_screen_width) / 2;

  // Initialisation de la fenêtre
  init_SDL(var_screen_width, var_screen_height);

  // Chargement des sprites
  init_load_sprites();

  var_maskindex = -1;
  sprintf(gen_maskname, "No mask");
  gen_hsize = INITIAL_HSIZE;
  gen_vsize = INITIAL_VSIZE;
  gen_mode = INITIAL_GENERATOR;
  gen_nbminotaurs = INITIAL_MINOTAURS;
  gen_braiding = INITIAL_BRAIDING;
  obj_mode = INITIAL_OBJECTS;

  var_mino_strat = STR_AGRE;
  event_loop();
  exit(EXIT_SUCCESS);
}
