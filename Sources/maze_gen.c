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
        if(neigbour != -1 && is_reach_maze(p_maze, neigbour)) //si la case voisine existe et est accessible
        {
            if(!visited[neigbour]) //si la case voisine n'a pas été visitée, on la visite
            {
                const int diff = cell - neigbour;
                if (diff == 1)
                {
                    del_wall_maze(p_maze, cell, WEST);
                }
                else if (diff == -1)
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
                visited[neigbour] = true;
                visited_count--;
            }
            cell = neigbour; //on se déplace
        }
    }
}

void random_maze_wilson(maze *p_maze) {
    bool visited[p_maze->hsize * p_maze->vsize]; // Tableau de booléens pour savoir si une case a été visitée
    int visited_count = p_maze->hsize * p_maze->vsize - 1; //nombre de case à visiter (la case de départ est déjà visitée)
    for(int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
    {
        if(is_reach_maze(p_maze, i)) //si la case est accessible
        {
            visited[i] = false; //on doit la visiter
        }
        else
        {
            visited[i] = false; //sinon on ne la visite pas
            visited_count--; //on décrémente le nombre de cases à visiter
        }
    }
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
        for(int i = 0; i < p_maze->hsize * p_maze->vsize; i++)
        {
            path_visited[i] = false; //aucune case n'a été visitée
        }

        //hérence jusqu'à une case visitée
        dynarray* path = create_dyn(); //liste des cases du chemin
        push_dyn(cell, path); //on initialise le chemin
        path_visited[cell] = true; //la case est visitée
        int neighbour; //case voisine
        while(!visited[cell]) //tant que la case n'a pas été visitée
        {
            push_dyn(cell, path); //on ajoute la case au chemin

            do //on cherche un voisin
            {
                const cardinal card = rand() % 4; //direction aléatoire
                neighbour = get_adj_maze(p_maze, cell, card); //case voisine
            }
            while(neighbour == -1 || !is_reach_maze(p_maze, neighbour)); //si le voisin existe

            cell = neighbour; //on se déplace

            //résolution des cycles
            while(path_visited[cell] && !is_empty_dyn(path)) //si la case a déjà été visitée, c'est un cycle (qui potentiellement vient du départ)
            {
                path_visited[cell] = false; //on efface le chemin en remontant le cycle
                cell = pop_dyn(path);
            }
            path_visited[cell] = true; //on marque la case dans le chemin
        }

        int cell1 = pop_dyn(path); //cell1 est la cellule sur le chemin (pas encore visitée)
        //on casse les murs
        while(!is_empty_dyn(path)) //tant qu'il reste des cases sur le chemin
        {
            const int diff = cell - cell1;
            if (diff == 1)
            {
                del_wall_maze(p_maze, cell, WEST);
            }
            else if (diff == -1)
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
            visited[cell1] = true; //la case est visitée
            visited_count--; //on décrémente le nombre de cases à visiter
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
