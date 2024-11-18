#include "maze_gen.h"

void (*gen_funs[GEN_SIZE]) (maze*) = { &maze_test,&random_maze_ab, &random_maze_wilson,&random_maze_hklinear ,&random_maze_hkdfs, &random_maze_hkrandom, &random_maze_prim ,&random_maze_kruskal ,&random_maze_rec };
const char* gen_names[GEN_SIZE] = { "Test (pas de génération)", "Aldous-Broder", "Wilson", "Hunt & Kill: Linéaire", "Hunt & Kill: DFS","Hunt & Kill: Aléatoire", "Prim" ,"Kruskal" , "Récursif" };

void maze_test(maze*) {
    //ne fait rien
}

void random_maze_ab(maze *p_maze) {
    bool visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée
    for(int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        visited[i] = false; //aucune case n'a été visitée
    }
    int visited_count = p_maze->hsize * p_maze->vsize - 1; //nombre de case à visiter (la case de départ est déjà visitée)
    int cell = rand() % (p_maze->hsize * p_maze->vsize); //case de départ aléatoire
    visited[cell] = true; //on visite la case de départ
    while(visited_count > 0)
    {
        const cardinal card = rand() % 4; //direction aléatoire
        const int neigbour = get_adj_maze(p_maze, cell, card); //case voisine
        if(neigbour !=1)
        {
            if(!visited[neigbour]) //si la case voisine n'a pas été visitée, on la visite
            {
                mask_cell_maze(p_maze, cell); //on casse le mur entre la case actuelle et la case voisine
                visited[neigbour] = true;
                visited_count--;
            }
            cell = neigbour; //on se déplace
        }
    }
}

void random_maze_wilson(maze *p_maze) {
    bool visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée
    for(int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        visited[i] = false; //aucune case n'a été visitée
    }
    int visited_count = p_maze->hsize * p_maze->vsize - 1; //nombre de case à visiter (la case de départ est déjà visitée)
    int cell = rand() % (p_maze->hsize * p_maze->vsize); //case de départ aléatoire
    visited[cell] = true; //on visite la case de départ
    while(visited_count > 0)
    {

        //recherche d'une case non visitée
        do
        {
            cell = rand() % (p_maze->hsize * p_maze->vsize); //case aléatoire
        } while(visited[cell]);

        bool path_visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée dans le chemin
        //hérence jusqu'à une case visitée
        dynarray* path = create_dyn(); //liste des cases visitées
        while(!visited[cell]) //tant que la case n'a pas été visitée
        {
            push_dyn(cell, path); //on ajoute la case à la liste
            const cardinal card = rand() % 4; //direction aléatoire
            const int neigbour = get_adj_maze(p_maze, cell, card); //case voisine
            if(neigbour != 1) //si le voisin existe
            {
                cell = neigbour; //on se déplace

                //résolution des cycles
                if(path_visited[cell]) //si la case a déjà été visitée, c'est un cycle
                {
                    const int cycle = cell;
                    do
                    {
                        cell = pop_dyn(path); //on enlève la dernière case de la liste
                        path_visited[cell] = false; //on marque la case comme non visitée
                    } while(cell != cycle); //tant qu'on n'a pas atteint le début du cycle
                }
                path_visited[cell] = true; //on marque la case comme visitée
            }
        }
        int cell1 = pop_dyn(path); //cell1 est la cellule sur le chemin (pas encore visitée)
        //on casse les murs
        for(int i = 0; i < size_dyn(path) - 1; i++)
        {
            visited[cell1] = true; //la case est visitée
            visited_count--; //on décrémente le nombre de cases à visiter
            int diff = cell - cell1;
            if (diff == 1) //cell1 à l'ouest de cell
                del_wall_maze(p_maze, cell, WEST);
            else if (diff == -1){
                del_wall_maze(p_maze, cell, EAST);}

            else if (diff == p_maze->hsize) //cell1 au nord de cell
                del_wall_maze(p_maze, cell, NORTH);

            else if (diff == -p_maze->hsize)
                del_wall_maze(p_maze, cell, SOUTH);

            else{
                fprintf(stderr, "Erreur: les cases ne sont pas voisines\n");
                free_dyn(path);
                free_maze(p_maze);
                exit(EXIT_FAILURE);
            }

            cell = cell1; //la cellule actuelle devient la cellule précédente (visitée)
            cell1 = pop_dyn(path); //la cellule précédente devient la cellule sur le chemin (pas encore visitée)
        }
    }
}

void random_maze_hkdfs(maze*) {
    return;
}

void random_maze_hkrandom(maze*) {
    return;
}

void random_maze_hklinear(maze*) {
    return;
}

void random_maze_prim(maze*) {
    return;
}

void random_maze_kruskal(maze*) {
    return;
}

void random_maze_rec(maze*) {
    return;
}
