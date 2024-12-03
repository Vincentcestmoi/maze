#include "maze_masks.h"



/***********************/
/*+ Fonctions de base +*/
/***********************/

mask* create_empty_mask(const int hsize, const int vsize) {
    mask *m = malloc(sizeof(mask));
    m->hsize = hsize;
    m->vsize = vsize;
    m->nbmasked = 0;
    m->grid = malloc(hsize * vsize * sizeof(bool));
    for(int i = 0; i < hsize * vsize; i++) {
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
    if (m == NULL) {
        return;
    }

    bool *newgrid = malloc(hsize * vsize * sizeof(bool));
    if (newgrid == NULL) {
        fprintf(stderr, "resize_mask: erreur d'allocation d'un malloc\n");
        return;
    }

    for (int i = 0; i < vsize; i++) {
        for (int j = 0; j < hsize; j++) {
            const int old_i = i * m->vsize / vsize;
            const int old_j = j * m->hsize / hsize;
            if (old_i < m->vsize && old_j < m->hsize) {
                newgrid[i * hsize + j] = m->grid[old_i * m->hsize + old_j];
            } else { //en cas de débordement dû à l'imprécision des arrondis
                newgrid[i * hsize + j] = true;
            }
        }
    }

    free(m->grid);
    m->grid = newgrid;
    m->hsize = hsize;
    m->vsize = vsize;
    m->nbmasked = 0;
    for (int i = 0; i < hsize * vsize; i++) {
        if (m->grid[i]) {
            m->nbmasked++;
        }
    }
}



void print_mask(mask *m) {
    printf("mask :\n");
    for(int i = 0; i < m->vsize; i++) {
        for(int j = 0; j < m->hsize; j++) {
            printf("%c", m->grid[i * m->hsize + j] ? 'X' : ' ');
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
    int hsize = 0, vsize = 0;
    char c = fgetc(f);
    while(c != '\n' && c != EOF){
        hsize++;
        c = fgetc(f);
    }
    while(c != EOF) {
        if (c == '\n') {
            vsize++;
        }
        c = fgetc(f);
    }
    mask *m = create_empty_mask(hsize, vsize);
    rewind(f);
    for(int i = 0; i < vsize; i++) {
        for(int j = 0; j < hsize; j++) {
            c = fgetc(f);
            if(c == 'X') {
                m->grid[i * hsize + j] = true;
                m->nbmasked++;
            }
            else {
                m->grid[i * hsize + j] = false;
            }
        }
        fgetc(f);
    }
    fclose(f);
    return m;
}



