#include "maze_gen.h"

void (*gen_funs[GEN_SIZE])(maze *) = {
    &maze_test,          &random_maze_ab,
    &random_maze_wilson, &random_maze_hklinear,
    &random_maze_hkdfs,  &random_maze_hkrandom,
    &random_maze_prim,   &random_maze_kruskal,
    &random_maze_rec};
const char *gen_names[GEN_SIZE] = {"Test (pas de génération)",
                                   "Aldous-Broder",
                                   "Wilson",
                                   "Hunt & Kill: Linéaire",
                                   "Hunt & Kill: DFS",
                                   "Hunt & Kill: Aléatoire",
                                   "Prim",
                                   "Kruskal",
                                   "Récursif"};

void maze_test(maze *) {
  // ne fait rien
}

void random_maze_ab(maze *p_maze) {
  bool
      visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir
                                              // si une case a été visitée
  for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++) {
    visited[i] = false; // aucune case n'a été visitée
  }
  int visited_count =
      p_maze->hsize * p_maze->vsize -
      1; // nombre de case à visiter (la case de départ est déjà visitée)
  int cell =
      rand() % (p_maze->hsize * p_maze->vsize); // case de départ aléatoire
  visited[cell] = true;                         // on visite la case de départ
  while (visited_count > 0) {
    const cardinal card = rand() % 4; // direction aléatoire
    const int neigbour = get_adj_maze(p_maze, cell, card); // case voisine
    if (neigbour != -1 &&
        is_reach_maze(p_maze,
                      neigbour)) // si la case voisine existe et est accessible
    {
      if (!visited[neigbour]) // si la case voisine n'a pas été visitée, on la
                              // visite
      {
        const int diff = cell - neigbour;
        if (diff == 1) {
          del_wall_maze(p_maze, cell, WEST);
        } else if (diff == -1) {
          del_wall_maze(p_maze, cell, EAST);
        } else if (diff == p_maze->hsize) {
          del_wall_maze(p_maze, cell, NORTH);
        } else if (diff == -p_maze->hsize) {
          del_wall_maze(p_maze, cell, SOUTH);
        }

        else {
          fprintf(stderr, "Erreur rma: les cases ne sont pas voisines\n");
          free_maze(p_maze);
          exit(EXIT_FAILURE);
        }
        visited[neigbour] = true;
        visited_count--;
      }
      cell = neigbour; // on se déplace
    }
  }
}

void random_maze_wilson(maze *p_maze) {
  bool
      visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir
                                              // si une case a été visitée
  int visited_count =
      p_maze->hsize * p_maze->vsize -
      1; // nombre de case à visiter (la case de départ est déjà visitée)
  for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++) {
    if (is_reach_maze(p_maze, i)) // si la case est accessible
    {
      visited[i] = false; // on doit la visiter
    } else {
      visited_count--; // on décrémente le nombre de cases à visiter
    }
  }
  int cell =
      rand() % (p_maze->hsize * p_maze->vsize); // case de départ aléatoire
  visited[cell] = true;                         // on visite la case de départ
  while (visited_count > 0) {

    // recherche d'une case non visitée
    do {
      cell = rand() % (p_maze->hsize * p_maze->vsize); // case aléatoire
    } while (visited[cell]);

    bool path_visited[p_maze->hsize *
                      p_maze->vsize]; // Tableau de booléens pour savoir si une
                                      // case a été visitée dans le chemin
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++) {
      path_visited[i] = false; // aucune case n'a été visitée
    }

    // hérence jusqu'à une case visitée
    dynarray *path = create_dyn(); // liste des cases du chemin
    push_dyn(cell, path);          // on initialise le chemin
    path_visited[cell] = true;     // la case est visitée
    int neighbour;                 // case voisine
    while (!visited[cell])         // tant que la case n'a pas été visitée
    {
      push_dyn(cell, path); // on ajoute la case au chemin

      do // on cherche un voisin
      {
        const cardinal card = rand() % 4;             // direction aléatoire
        neighbour = get_adj_maze(p_maze, cell, card); // case voisine
      } while (neighbour == -1 ||
               !is_reach_maze(p_maze, neighbour)); // si le voisin existe

      cell = neighbour; // on se déplace

      // résolution des cycles
      while (path_visited[cell] &&
             !is_empty_dyn(path)) // si la case a déjà été visitée, c'est un
                                  // cycle (qui potentiellement vient du départ)
      {
        path_visited[cell] = false; // on efface le chemin en remontant le cycle
        cell = pop_dyn(path);
      }
      path_visited[cell] = true; // on marque la case dans le chemin
    }

    int cell1 = pop_dyn(
        path); // cell1 est la cellule sur le chemin (pas encore visitée)
    // on casse les murs
    while (!is_empty_dyn(path)) // tant qu'il reste des cases sur le chemin
    {
      const int diff = cell - cell1;
      if (diff == 1) {
        del_wall_maze(p_maze, cell, WEST);
      } else if (diff == -1) {
        del_wall_maze(p_maze, cell, EAST);
      } else if (diff == p_maze->hsize) {
        del_wall_maze(p_maze, cell, NORTH);
      } else if (diff == -p_maze->hsize) {
        del_wall_maze(p_maze, cell, SOUTH);
      }

      else {
        fprintf(stderr, "Erreur rma: les cases ne sont pas voisines\n");
        free_maze(p_maze);
        exit(EXIT_FAILURE);
      }
      visited[cell1] = true; // la case est visitée
      visited_count--;       // on décrémente le nombre de cases à visiter
      cell =
          cell1; // la cellule actuelle devient la cellule précédente (visitée)
      cell1 = pop_dyn(path); // la cellule précédente devient la cellule sur le
                             // chemin (pas encore visitée)
    }
  }
}

static void phase_kill_hkrandom(maze *p_maze, bool *visited, int cell,
                                int *visited_count) {
  visited[cell] = true; // on visite la case où nous sommes
  (*visited_count)--;   // on décrémente le nombre de cases à visiter
  int dir_tab[4] = {0};
  // tableau de booléens pour savoir si une direction est possible
  int possible_dir = 0; // nombre de directions possibles
  for (int i = 0; i < 4; i++) {
    int neighbour = get_adj_maze(p_maze, cell, i); // case voisine
    if (neighbour != -1 && !visited[neighbour]) {
      // si la case voisine existe et n'a pas été visitée
      dir_tab[i] = true;
      possible_dir++;
    }
  }
  if (possible_dir == 0) {
    // si aucune direction n'est possible, on retourne
    return;
  }
  int random_dir = rand() % possible_dir;
  while (dir_tab[random_dir] == false) {
    // on cherche une direction possible
    random_dir = (random_dir + 1) % 4;
  }
  del_wall_maze(p_maze, cell, random_dir);
  phase_kill_hkrandom(p_maze, visited, get_adj_maze(p_maze, cell, random_dir),
                      visited_count);
  // on continue la phase de kill
  return;
}

void random_maze_hkdfs(maze *) { return; }

void random_maze_hkrandom(maze *p_maze) {
  bool visited[p_maze->hsize * p_maze->vsize];
  // tableau de booléens pour savoir si une case a été visitée
  int visited_count = p_maze->hsize * p_maze->vsize;
  // nombre de cases à visiter
  for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++) {
    if (is_reach_maze(p_maze, i)) // si la case est accessible
    {
      visited[i] = false; // on doit la visiter
    } else {
      visited_count--; // on décrémente le nombre de cases à visiter
    }
  }
  // on choisit une case aléatoire, on la visite et on lance la phase de kill
  int cell = rand() % (p_maze->hsize * p_maze->vsize); // case aléatoire
  bool has_adjacent_visited_;
  // booléen pour savoir si la case a des voisins visités
  int dir_tab[4] = {0}; // tableau de booléens
  int possible_dir = 0; // nombre de directions possibles

  phase_kill_hkrandom(p_maze, visited, cell, &visited_count);

  while (visited_count > 0) {
    do {
      cell = rand() % (p_maze->hsize * p_maze->vsize); // case aléatoire
      has_adjacent_visited_ = false; // on réinitialise le booléen
      for (int i = 0; i < 4; i++) {
        dir_tab[i] = false; // on réinitialise le tableau de booléens
      }
      possible_dir = 0; // on réinitialise le nombre de directions possibles
      for (int i = 0; i < 4; i++) {
        int neighbour = get_adj_maze(p_maze, cell, i); // case voisine
        if (neighbour != -1 && visited[neighbour]) {
          // si la case voisine existe et a été visitée
          dir_tab[i] = true;
          possible_dir++;
          has_adjacent_visited_ = true;
        }
      }
    } while (visited[cell] || !has_adjacent_visited_);
    // tant que la case a été visitée ou n'a pas de voisins visités

    if (has_adjacent_visited_) { // si la case a des voisins visités
      int random_dir = rand() % possible_dir;
      while (dir_tab[random_dir] == false) {
        random_dir = (random_dir + 1) % 4;
      }
      del_wall_maze(p_maze, cell, random_dir);
      // on casse un mur
    }
    phase_kill_hkrandom(p_maze, visited, cell, &visited_count);
    // on lance la phase de kill
  }
}

void random_maze_hklinear(maze *) { return; }

void random_maze_prim(maze *) { return; }

void random_maze_kruskal(maze *) { return; }

void random_maze_rec(maze *) { return; }
