#include "maze_masks.h"



/***********************/
/*+ Fonctions de base +*/
/***********************/

mask* create_empty_mask(const int hsize, const int vsize) {
    mask *m = malloc(sizeof(mask));
    m->hsize = hsize;
    m->vsize = vsize;
    m->nbmasked = 0;
    m->grid = malloc(hsize*vsize*sizeof(bool));
    for(int i = 0; i < hsize*vsize; i++) {
        m->grid[i] = false;
    }
    return m;
}

void free_mask(mask *m) {
    if(m == NULL) {
        return;
    }
    free(m->grid);
    free(m);
}

void resize_mask(mask *m, const int hsize, const int vsize) {
    if(m == NULL) {
        return;
    }
    int delta = m->hsize - hsize; // la variation de taille horizontale
    bool inter[hsize * m->vsize];
    if(delta > 0) { //on doit ajouter des colonnes
        for(int i = 0; i < m->vsize; i++) {
            for(int j = 0; j < hsize; j++) {
                inter[i * hsize + j] = m->grid[i * m->hsize + j];
            }
        }
    }
    else if (delta < 0) { //on doit supprimer des colonnes
        for(int i = 0; i < m->vsize; i++) {
            for(int j = 0; j < hsize; j++) {
                //TODO
            }
        }
    }
    // delta = 0, on copie simplement
    else {
        for(int i = 0; i < m->vsize; i++) {
            for(int j = 0; j < m->hsize; j++) {
                //TODO
            }
        }
    }
    delta = vsize - m->vsize;
    bool *newgrid = malloc(sizeof(bool) * hsize * vsize);
    if(delta > 0) { //on doit ajouter des lignes
        for(int i = 0; i < m->vsize; i++) {
            for(int j = 0; j < m->hsize; j++) {
                //TODO
            }
        }
    }
    else if (delta < 0) { //on doit supprimer des lignes
        for(int i = 0; i < vsize; i++) {
            for(int j = 0; j < m->hsize; j++) {
                //TODO
            }
        }
    }
    // delta == 0, on copie simplement
    else {
        for(int i = 0; i < m->vsize; i++) {
            for(int j = 0; j < m->hsize; j++) {
                newgrid[i * hsize + j] = inter[i * m->hsize + j];
            }
        }
    }
    free(m->grid);
    m->grid = newgrid;
    m->hsize = hsize;
    m->vsize = vsize;
}



void print_mask(mask *m) {
    printf("mask :\n");
    for(int i = 0; i < m->vsize; i++) {
        for(int j = 0; j < m->hsize; j++) {
            printf("%c", m->grid[i*m->hsize+j] ? 'X' : ' ');
        }
        printf("\n");
    }
}


/**************************************/
/*+ Génération à partir d'un fichier +*/
/**************************************/


mask* read_mask(const char *fillname) {
    FILE *f = fopen(fillname, "r");
    if(!f) {
        return NULL;
    }
    int hsize =0, vsize = 1;
    char c;
    do {
        c = fgetc(f);
        hsize++;
    }while(c != '\n' && c != EOF);
    do {
        c = fgetc(f);
        if(c == '\n') {
            vsize++;
        }
    }while(c != EOF);
    mask *m = create_empty_mask(hsize, vsize);
    rewind(f);
    for(int i = 0; i < vsize; i++) {
        for(int j = 0; j < hsize; j++) {
            c = fgetc(f);
            if(c == 'X') {
                m->grid[i*hsize+j] = true;
                m->nbmasked++;
            }
            else {
                m->grid[i*hsize+j] = false;
            }
        }
        fgetc(f);
    }
    fclose(f);
    return m;
}



