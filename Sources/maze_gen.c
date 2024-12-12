#include "maze_gen.h"

#include "data_binheap.h"
#include "maze.h"
#include "maze_2.h"

#include <sys/random.h>

void (*gen_funs[GEN_SIZE])(maze *) = {&maze_test,         &random_maze_ab,       &random_maze_wilson, &random_maze_hklinear,
                                      &random_maze_hkdfs, &random_maze_hkrandom, &random_maze_prim,   &random_maze_kruskal,
                                      &random_maze_rec, &snake_maze};
const char *gen_names[GEN_SIZE] = {"Test (pas de génération)",
                                   "Aldous-Broder",
                                   "Wilson",
                                   "Hunt & Kill: Linéaire",
                                   "Hunt & Kill: DFS",
                                   "Hunt & Kill: Aléatoire",
                                   "Prim",
                                   "Kruskal",
                                   "Récursif",
                                    "serpent"};

void maze_test(maze *)
{
    // ne fait rien
}

void random_maze_ab(maze *p_maze)
{
    bool visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée
    int visited_count = p_maze->nb_reachable; // nombre de case à visiter
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        visited[i] = false; // aucune case n'a été visitée
    }
    uint cell;
    do
    {
        getrandom(&cell, sizeof(cell), 0);
        cell %= p_maze->hsize * p_maze->vsize;
    }
    while (!can_be_used(p_maze, cell)); // on cherche une case utilisable

    visited[cell] = true; // on visite la case de départ
    visited_count--;
    while (visited_count > 0)
    {
        const cardinal card = rand() % 4; // direction aléatoire
        const int neigbour = get_adj_maze(p_maze, cell, card); // case voisine
        if (can_be_used(p_maze, neigbour)) // si le voisin est exploitable
        {
            if (!visited[neigbour])
            // si la case voisine n'a pas été visitée, on la visite
            {
                del_wall_maze(p_maze, cell, card); // on casse le mur
                visited[neigbour] = true;
                visited_count--;
            }
            cell = neigbour; // on se déplace
        }
    }
}

void random_maze_wilson(maze *p_maze)
{
    bool visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée
    int visited_count = p_maze->nb_reachable;
    // nombre de case à visiter (la case de départ est déjà visitée)
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        if (can_be_used(p_maze, i))
        // si la case est accessible
        {
            visited[i] = false; // on doit la visiter
        }
    }
    int cell;
    do
    {
        getrandom(&cell, sizeof(cell), 0);
        cell %= p_maze->hsize * p_maze->vsize;
    }
    while (!can_be_used(p_maze, cell)); // on cherche une case utilisable

    visited[cell] = true; // on visite la case de départ
    while (visited_count > 0) // recherche d'une case non visitée
    {
        do
        {
            cell = rand() % (p_maze->hsize * p_maze->vsize); // case aléatoire
        }
        while (visited[cell] || !can_be_used(p_maze, cell)); // tant que la case a été visitée ou n'est pas utilisable

        bool path_visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée
        for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
        {
            path_visited[i] = false; // aucune case n'a été visitée
        }

        // hérence jusqu'à une case visitée
        dynarray *path = create_dyn(); // liste des cases du chemin
        push_dyn(cell, path); // on initialise le chemin
        path_visited[cell] = true; // la case est visitée
        int neighbour; // case voisine
        cardinal card; // direction aléatoire
        while (!visited[cell]) // tant que la case n'a pas été visitée
        {
            push_dyn(cell, path); // on ajoute la case au chemin

            do // on cherche un voisin
            {
                getrandom(&card, sizeof(card), 0);
                card %= 4;
                neighbour = get_adj_maze(p_maze, cell, card); // case voisine
            }
            while (!can_be_used(p_maze, neighbour)); // jusqu'à trouver un voisin convenable

            cell = neighbour; // on se déplace

            // résolution des cycles
            while (path_visited[cell] && !is_empty_dyn(path)) // si la case a déjà été visitée, c'est un cycle
            {
                path_visited[cell] = false; // on efface le chemin en remontant le cycle
                cell = pop_dyn(path);
            }
            path_visited[cell] = true; // on marque la case dans le chemin
        }

        int cell1 = pop_dyn(path); // cell1 est la cellule sur le chemin (pas encore visitée)
        // on casse les murs
        while (!is_empty_dyn(path)) // tant qu'il reste des cases sur le chemin
        {
            const int diff = cell - cell1;
            if (diff == 1 && p_maze->hsize != 1)
            {
                del_wall_maze(p_maze, cell, WEST);
            }
            else if (diff == -1 && p_maze->hsize != 1)
            {
                del_wall_maze(p_maze, cell, EAST);
            }
            else if (diff == p_maze->hsize)
            {
                del_wall_maze(p_maze, cell, NORTH);
            }
            else if (diff == -p_maze->hsize)
            {
                del_wall_maze(p_maze, cell, SOUTH);
            }

            else
            {
                fprintf(stderr, "Erreur rma: les cases ne sont pas voisines\n");
                free_maze(p_maze);
                exit(EXIT_FAILURE);
            }
            visited[cell1] = true; // la case est visitée
            visited_count--; // on décrémente le nombre de cases à visiter
            cell = cell1; // la cellule actuelle devient la cellule précédente (visitée)
            cell1 = pop_dyn(path); // la cellule précédente devient la cellule chemin (pas encore visitée)
        }
    }
}


static void phase_kill_hkdfs(maze *p_maze, bool *visited, int cell, dynarray *d)
{
    visited[cell] = true; // on visite la case où nous sommes
    // int dir_tab[4] = {0};
    //   tableau de booléens pour savoir si une direction est possible
    int possible_dir = 0; // nombre de directions possibles
    for (int i = 0; i < 4; i++)
    {
        int neighbour = get_adj_maze(p_maze, cell, i); // case voisine
        if (neighbour != -1 && !visited[neighbour] && can_be_used(p_maze, neighbour))
        {
            // si la case voisine existe et n'a pas été visitée
            // dir_tab[i] = true;
            possible_dir++;
        }
    }
    if (possible_dir == 0)
    {
        // si aucune direction n'est possible, on retourne
        return;
    }
    int random_dir = rand() % 4;
    while (get_adj_maze(p_maze, cell, random_dir) == -1 || visited[get_adj_maze(p_maze, cell, random_dir)] || !can_be_used(p_maze, cell))
    {
        random_dir = rand() % 4;
    }
    push_dyn(cell, d);
    del_wall_maze(p_maze, cell, random_dir);
    phase_kill_hkdfs(p_maze, visited, get_adj_maze(p_maze, cell, random_dir), d);
    // on continue la phase de kill
    return;
}


void random_maze_hkdfs(maze *p_maze)
{
    bool visited[p_maze->hsize * p_maze->vsize];
    // tableau de booléens pour savoir si une case a été visitée
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        if (can_be_used(p_maze, i)) // si la case est accessible
        {
            visited[i] = false; // on doit la visiter
        }
        else
        {
            visited[i] = true; // on ne doit pas la visiter
        }
    }
    dynarray *d = create_dyn();
    // on choisit une case aléatoire, on la visite et on lance la phase de kill
    int cell; // case aléatoire
    do
    {
        cell = rand() % (p_maze->hsize * p_maze->vsize);
    }
    while (visited[cell] || !can_be_used(p_maze, cell));
    push_dyn(cell, d);
    phase_kill_hkdfs(p_maze, visited, pop_dyn(d), d);
    // booléen pour savoir si la case a des voisins visités
    while (!is_empty_dyn(d))
    {
        phase_kill_hkdfs(p_maze, visited, pop_dyn(d), d);
    }
    return;
}

static void phase_kill_hkrandom(maze *p_maze, bool *visited, int cell, int *visited_count)
{
    if (!visited[cell])
    {
        (*visited_count)--; // on décrémente le nombre de cases à visiter
    }
    visited[cell] = true; // on visite la case où nous sommes
    int dir_tab[4] = {0};
    // tableau de booléens pour savoir si une direction est possible
    int possible_dir = 0; // nombre de directions possibles
    for (int i = 0; i < 4; i++)
    {
        int neighbour = get_adj_maze(p_maze, cell, i); // case voisine
        if (neighbour != -1 && !visited[neighbour])
        {
            // si la case voisine existe et n'a pas été visitée
            dir_tab[i] = true;
            possible_dir++;
        }
    }
    if (possible_dir == 0)
    {
        // si aucune direction n'est possible, on retourne
        return;
    }
    int random_dir = rand() % possible_dir;
    while (dir_tab[random_dir] == false)
    {
        // on cherche une direction possible
        random_dir = (random_dir + 1) % 4;
    }
    del_wall_maze(p_maze, cell, random_dir);
    phase_kill_hkrandom(p_maze, visited, get_adj_maze(p_maze, cell, random_dir), visited_count);
    // on continue la phase de kill
    return;
}

void random_maze_hkrandom(maze *p_maze)
{
    bool visited[p_maze->hsize * p_maze->vsize];
    // tableau de booléens pour savoir si une case a été visitée
    int visited_count = p_maze->hsize * p_maze->vsize;
    // nombre de cases à visiter
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        if (can_be_used(p_maze, i)) // si la case est accessible
        {
            visited[i] = false; // on doit la visiter
        }
        else
        {
            visited[i] = true;
            visited_count--; // on décrémente le nombre de cases à visiter
        }
    }
    // on choisit une case aléatoire et on lance la phase de kill
    int cell;
    do
    {
        cell = rand() % (p_maze->hsize * p_maze->vsize);
    }
    while (visited[cell] || !can_be_used(p_maze, cell));
    phase_kill_hkrandom(p_maze, visited, cell, &visited_count);

    while (visited_count > 0)
    {
        do
        {
            cell = rand() % (p_maze->hsize * p_maze->vsize);
        }
        while (!visited[cell] || !can_be_used(p_maze, cell));
        phase_kill_hkrandom(p_maze, visited, cell, &visited_count);
    }
}


static void phase_kill_hklinear(maze *p_maze, char *visited, int cell, int *cell_min)
{
    visited[cell] = 1; // on visite la case où nous sommes
    int possible_dir = 0; // nombre de directions possibles
    for (int i = 0; i < 4; i++)
    {
        int neighbour = get_adj_maze(p_maze, cell, i); // case voisine
        if (neighbour != -1 && !visited[neighbour] && can_be_used(p_maze, neighbour))
        {
            // si la case voisine existe et n'a pas été visitée
            possible_dir++;
        }
    }
    if (possible_dir == 0)
    {
        visited[cell] = 2;
        return;
    }

    int random_dir = rand() % 4;
    while (get_adj_maze(p_maze, cell, random_dir) == -1 || visited[get_adj_maze(p_maze, cell, random_dir)] == 1 ||
           !can_be_used(p_maze, get_adj_maze(p_maze, cell, random_dir)))
    {
        random_dir = rand() % 4;
    }
    del_wall_maze(p_maze, cell, random_dir);

    if (cell < *cell_min)
        *cell_min = cell;
    phase_kill_hklinear(p_maze, visited, get_adj_maze(p_maze, cell, random_dir), cell_min);
    // on continue la phase de kill
    return;
}

void random_maze_hklinear(maze *p_maze)
{
    char visited[p_maze->hsize * p_maze->vsize];
    // tableau de booléens pour savoir si une case a été visitée
    // nombre de cases à visiter
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        if (can_be_used(p_maze, i)) // si la case est accessible
        {
            visited[i] = 0; // on doit la visiter
        }
        else
        {
            visited[i] = 2; // on ne doit pas la visiter
        }
    }
    // on choisit une case aléatoire, on la visite et on lance la phase de kill
    int cell;
    do
    {
        cell = rand() % (p_maze->hsize * p_maze->vsize); // case aléatoire}
    }
    while (!can_be_used(p_maze, cell));

    phase_kill_hklinear(p_maze, visited, cell, &cell);

    while (cell != p_maze->hsize * p_maze->vsize)
    {
        int n_cell = cell;
        while (visited[n_cell] == 2 || visited[n_cell] == 0)
        {
            n_cell++;
            if (visited[n_cell] == 1 && can_be_used(p_maze, n_cell))
            {
                cell = n_cell;
            }
            if (n_cell == p_maze->hsize * p_maze->vsize)
            {
                return;
            }
        }
        phase_kill_hklinear(p_maze, visited, cell, &cell);
    }
}

//-------------------------Prim-------------------------


typedef struct {
    int* array1;
    cardinal* array2;
    int size;
    int true_size;
    int capacity;
} prim_array;

static void grow_prim_array(prim_array* p_array) {
    p_array->capacity *= 2;
    int* new_array1 = malloc(p_array->capacity * sizeof(int));
    cardinal* new_array2 = malloc(p_array->capacity * sizeof(cardinal));
    if (new_array1 == NULL || new_array2 == NULL) {
        perror("Failed to reallocate memory for prim_array");
        exit(EXIT_FAILURE);
    }
    int j = 0;
    for (int i = 0; i < p_array->size; i++) {
        if (p_array->array1[i] != -1) {
            new_array1[j] = p_array->array1[i];
            new_array2[j] = p_array->array2[i];
            j++;
        }
    }
    p_array->size = j;
    free(p_array->array1);
    free(p_array->array2);
    p_array->array1 = new_array1;
    p_array->array2 = new_array2;
    p_array->true_size = j;
}

static void shrink_prim_array(prim_array* p_array) {
    p_array->capacity /= 2;
    int* new_array1 = malloc(p_array->capacity * sizeof(int));
    cardinal* new_array2 = malloc( p_array->capacity * sizeof(cardinal));
    if (new_array1 == NULL || new_array2 == NULL) {
        perror("Failed to reallocate memory for prim_array");
        exit(EXIT_FAILURE);
    }
    int j = 0;
    for (int i = 0; i < p_array->size; i++){
        if(p_array->array1[i] != -1) {
            new_array1[j] = p_array->array1[i];
            new_array2[j] = p_array->array2[i];
            j++;
        }
    }
    p_array->size = j;
    p_array->true_size = j;
    free(p_array->array1);
    free(p_array->array2);
    p_array->array1 = new_array1;
    p_array->array2 = new_array2;
}

static void clean_prim_array(prim_array* p_array) {
    int j = 0;
    for (int i = 0; i < p_array->size; i++) {
        if (p_array->array1[i] != -1) {
            p_array->array1[j] = p_array->array1[i];
            p_array->array2[j] = p_array->array2[i];
            j++;
        }
    }
    p_array->size = j;
}

static prim_array* create_prim_array(void)
{
    prim_array* p_array = malloc(sizeof(prim_array));
    if (p_array == NULL) {
        perror("Failed to allocate memory for prim_array");
        exit(EXIT_FAILURE);
    }
    p_array->array1 = malloc(sizeof(int));
    p_array->array2 = malloc(sizeof(cardinal));
    if (p_array->array1 == NULL || p_array->array2 == NULL) {
        perror("Failed to allocate memory for prim_array");
        exit(EXIT_FAILURE);
    }
    p_array->array1[0] = -1;
    p_array->array2[0] = -1;
    p_array->size = 0;
    p_array->true_size = 0;
    p_array->capacity = 1;
    return p_array;
}

static void free_prim_array(prim_array* p_array)
{
    free(p_array->array1);
    free(p_array->array2);
    free(p_array);
}

static bool is_empty_prim_array(const prim_array* p_array)
{
    return p_array->true_size == 0;
}

static void push_prim_array(prim_array* p_array, const int value1, const cardinal value2)
{
    if (p_array->array1 == NULL || p_array->array2 == NULL) {
        fprintf(stderr, "push_prim_array: array is NULL\n");
        exit(EXIT_FAILURE);
    }
    //on s'assure d'avoir la place
    if (p_array->size == p_array->capacity) {
        if (p_array->size == p_array->true_size) {
            grow_prim_array(p_array);
        } else {
            clean_prim_array(p_array);
        }
    }
    p_array->array1[p_array->size] = value1;
    p_array->array2[p_array->size] = value2;
    p_array->size++;
    p_array->true_size++;
}

//pop aléatoire
static void pop_prim_array(prim_array* p_array, int* value1, cardinal* value2)
{
    if (is_empty_prim_array(p_array)) {
        fprintf(stderr, "pop_prim_array: array is empty\n");
        exit(EXIT_FAILURE);
    }
    if (p_array->array1 == NULL || p_array->array2 == NULL) {
        fprintf(stderr, "pop_prim_array: array is NULL\n");
        exit(EXIT_FAILURE);
    }
    int r;
    getrandom(&r, sizeof(r), 0);
    r = abs(r) % p_array->true_size;
    int i = 0;
    while (r > 0 || p_array->array1[i] == -1)
    {
        if (p_array->array1[i] != -1)
        {
            r--;
        }
        i++;
    }
    *value1 = p_array->array1[i];
    *value2 = p_array->array2[i];
    p_array->array1[i] = -1;
    p_array->true_size--;
    if (p_array->true_size < p_array->capacity / 4) {
        shrink_prim_array(p_array);
    }
}

//-------------------------Prim-------------------------

void random_maze_prim(maze *p_maze) {
    bool visited[p_maze->hsize * p_maze->vsize];
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        if (can_be_used(p_maze, i))
        {
            visited[i] = false;
        }
    }
    prim_array* edge = create_prim_array();
    int cell;
    do
    {
        getrandom(&cell, sizeof(cell), 0);
        cell %= p_maze->hsize * p_maze->vsize;
    }while (!can_be_used(p_maze, cell));
    visited[cell] = true;
    for (cardinal c = NORTH; c <= WEST; c++)
    {
        push_prim_array(edge, cell, c);
    }
    cardinal card;
    while (!is_empty_prim_array(edge))
    {
        pop_prim_array(edge, &cell, &card);
        const int neighbour = get_adj_maze(p_maze, cell, card);
        if (!can_be_used(p_maze, neighbour) || visited[neighbour])
        {
            continue;
        }
        del_wall_maze(p_maze, cell, card);
        visited[neighbour] = true;
        for (cardinal c = NORTH; c <= WEST; c++)
        {
            push_prim_array(edge, neighbour, c);
        }
    }
    free_prim_array(edge);
}

void random_maze_kruskal(maze *){
}

static void maze_rec(maze *m, const int x, const int y, const int w, const int h)
{
    if (w == 1 && h == 1)
    {
        return;
    }
    unsigned int wall;
    if (w > h)
    {
        const int middle = x + w / 2; //la coordonnée x du mur à casser
        getrandom(&wall, sizeof(wall), 0);
        wall = y + wall % h; //la coordonné y du mur à casser
        del_wall_maze(m, wall * m->hsize + middle, WEST);
        maze_rec(m, x, y, middle - x, h);
        maze_rec(m, middle, y, x + w - middle, h);
    }
    else
    {
        const int middle = y + h / 2; //la coordonnée y du mur à casser
        getrandom(&wall, sizeof(wall), 0);
        wall = x + wall % w; //la coordonné x du mur à casser
        del_wall_maze(m, middle * m->hsize + wall, NORTH);
        maze_rec(m, x, y, w, middle - y);
        maze_rec(m, x, middle, w, y + h - middle);
    }
}

void random_maze_rec(maze *p_maze){
    maze_rec(p_maze, 0, 0, p_maze->hsize, p_maze->vsize);
}

void snake_maze(maze *p_maze)
{
    //c'est pas optimisé mais ça fait partie de son charme
    bool visited[p_maze->hsize * p_maze->vsize];
    for (int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        if (can_be_used(p_maze, i))
        {
            visited[i] = false;
        }
    }
    dynarray *cell_list = create_dyn();
    dynarray *card_list = create_dyn();
    int cell;
    do
    {
        getrandom(&cell, sizeof(cell), 0);
        cell %= p_maze->hsize * p_maze->vsize;
    }while (!can_be_used(p_maze, cell));
    visited[cell] = true;
    for (cardinal c = NORTH; c <= WEST; c++)
    {
        push_dyn(cell, cell_list);
        push_dyn(c, card_list);
    }
    while (!is_empty_dyn(card_list))
    {
        const cardinal card = pop_dyn(card_list);
        cell = pop_dyn(cell_list);
        const int neighbour = get_adj_maze(p_maze, cell, card);
        if (!can_be_used(p_maze, neighbour) || visited[neighbour])
        {
            continue;
        }
        del_wall_maze(p_maze, cell, card);
        visited[neighbour] = true;
        for (cardinal c = NORTH; c <= WEST; c++)
        {
            push_dyn(neighbour, cell_list);
            push_dyn(c, card_list);
        }
    }
}