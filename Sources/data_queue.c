#include "data_queue.h"

/*************************/
/* Fonctions auxiliaires */
/*************************/

void grow_queue(queue *p) {
    int *n_array = malloc(p->size_array * 2 * sizeof(int));
    int k = 0;
    do {
        n_array[p->size_array + k] = p->array[p->left % p->size_array];
        p->left++;
        k++;
    }while (p->left % p->size_array != p->right % p->size_array);
    free(p->array);
    p->array = n_array;
    p->left = p->size_array;
    p->size_array *= 2;
    p->right = 0;
}


void shrink_queue(queue *p) {
    int *n_array = malloc(p->size_array / 2 * sizeof(int));
    for (int k = 0; k < p->size_array / 4; k++){
        n_array[k + p->size_array / 4] = p->array[p->left % p->size_array];
        p->left++;
    }
    free(p->array);
    p->array = n_array;
    p->left = p->size_array / 4;
    p->size_array /= 2;
    p->right = 0;
}



/************************/
/* Fonctions primitives */
/************************/



queue* create_queue(void) {
    queue *p = malloc(sizeof(queue));
    p->array = malloc(sizeof(int));
    p->size_array = 1;
    p->left = 0;
    p->right = 0;
    p->empty = true;
  return p;
}


void delete_queue(queue *p) {
    free(p->array);
    free(p);
}


bool is_empty_queue(queue *p) {
    return p->empty;;
}



int getsize_queue(queue *p) {
    if (is_empty_queue(p)) // si vide retourne 0
        return 0;
    const int delta = p->right - p->left;

    if (p->right == p->left){ // si plein
        return p->size_array;
    }
    if (delta < 0)
        return p->size_array + delta;
  return delta;
}


int read_queue(queue *p, const int indice) {
    if (getsize_queue(p) < indice){
        fprintf(stderr, "Ne peut lire un élément qui n'existe pas\n");
        exit(EXIT_FAILURE); // si on dépasse le nombre d'élément, la lecture aurait un comportement aléatoire
    }
    return p->array[(p->left + indice) % p->size_array];
}




void enqueue(const int val, queue *p) {
    if (getsize_queue(p)==p->size_array && !is_empty_queue(p)){
        grow_queue(p); // si la queue est pleine on l'agrandit
    }
    p->empty = false;
    p->left--;
    if (p->left < 0){
      p->left += p->size_array; // on décale left "à gauche"
    }
    p->array[p->left] = val; //on ajoute notre élément en début de queue
}


int dequeue(queue *p) {
    if (is_empty_queue(p)){
        fprintf(stderr, "Ne peut retirer un élément de la file quand elle est vide\n");
        exit(EXIT_FAILURE); // si la queue est vide on ne peut pas supprimer d'éléménts
    }
    p->right--;
    if (p->right < 0) {
        p->right += p->size_array;  // Ajuster à la fin du tableau
    }
    if (p->right == p->left){
        p->empty = true; // si la file est vide on la déclare vide
    }
    if (1 < getsize_queue(p) &&  getsize_queue(p) <= p->size_array / 4){
        shrink_queue(p);
    }

  return  p->array[p->right]; // on récupère la valeur de la cellule que nous voulons renvoyer
}
