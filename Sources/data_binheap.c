#include "data_binheap.h"

/*************************/
/* Fonctions auxiliaires */
/*************************/

/* Fonctions de navigation dans un arbre représenté par un tableau */
int left_binheap(int i) {
  return i * 2 + 1;
}
int right_binheap(int i) {
  return i * 2 + 2;
}
int parent_binheap(int i) {
  return (i - 1) / 2;
}
bool isvalid_binheap(binheap *p, int i) {
  return !(i < 0 || i >= p->size_heap);
}

/* Modification de la taille du tableau */
void grow_binheap(binheap *p) {
  p -> size_array *= 2;
  REALLOC(p -> array, p->size_array);
}

void shrink_binheap(binheap *p) {
  if(p->size_array > 1) {
    p -> size_array /= 2;
    REALLOC(p -> array, p->size_array);
  }
}

/************************/
/* Fonctions primitives */
/************************/

/* Création d'un tas vide */
binheap *create_binheap(int (*fc) (void*, void*)) {
  binheap *p = malloc(sizeof(binheap));
  if (p == NULL) {
    fprintf(stderr, "Erreur d'allocation de mémoire\n");
    exit(EXIT_FAILURE);
  }
  p->size_array = 1;
  p->size_heap = 0;
  p->array = malloc(sizeof(void *) * p->size_array);
  if (p->array == NULL) {
    free(p);
    fprintf(stderr, "Erreur d'allocation de mémoire\n");
    exit(EXIT_FAILURE);
  }
  p->fc = fc;
  return p;
}

/* Suppression */
void delete_binheap(binheap *p) {
  free(p->array);
  free(p);
}

/* Test du vide */
bool isempty_binheap(binheap *p) {
  return p -> size_heap == 0;
}

/* Récupération de la taille */
int getsize_binheap(binheap *p) {
  return p -> size_heap;
}

/* Insertion d'une valeur */
void push_binheap(binheap *p, void *val) {
  int i = p -> size_heap;
  if(i == p -> size_array) {
    grow_binheap(p);
  }
  while(i != 0 && p -> fc(val, p -> array[parent_binheap(i)]))
  {
    p -> array[i] = p -> array[parent_binheap(i)];
    i = parent_binheap(i);
  }
  p -> array[i] = val;
  p -> size_heap++;
}

/* Récupération du minimum sans le retirer */
void *peekmin_binheap(binheap *p) {
  if(isempty_binheap(p))
  {
    fprintf(stderr, "Erreur : tas vide\n");
    exit(EXIT_FAILURE);
  }
  return p -> array[0];
}

/* Récupération du minimum en le retirant */
void *popmin_binheap(binheap *p) {
  void *min = peekmin_binheap(p);
  p -> size_heap--;
  int i = 0;
  p->array[i] = p->array[p->size_heap]; // On remplace la racine par la dernière feuille
  bool do_run = true;
  while(do_run)
  {
    const int left = left_binheap(i);
    const int right = right_binheap(i);
    if(isvalid_binheap(p, right))
    {
      int min_i;
      if(p->fc(p->array[left], p->array[right]))
      {
        min_i = left;
      }
      else
      {
        min_i = right;
      }
      if(p->fc(p->array[i], p->array[min_i]))
      {
        do_run = false;
      }
      else
      {
        void *tmp = p->array[i];
        p->array[i] = p->array[min_i];
        i = min_i;
        p->array[i] = tmp;
      }
    }
    else if(isvalid_binheap(p, left))
    {
      if(p->fc(p->array[i], p->array[left]))
      {
        do_run = false;
      }
      else
      {
        void *tmp = p->array[i];
        p -> array[i] = p -> array[left];
        i = left;
        p -> array[i] = tmp;
      }
    }
    else
    {
      do_run = false;
    }
  }
  if(p -> size_heap < p -> size_array / 4)
  {
    shrink_binheap(p);
  }
  return min;
}
