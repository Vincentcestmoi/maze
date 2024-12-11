#include "game_history.h"

// La structure pour représenter un historique: à définir.
typedef struct history {
    turn *past; // Les tours passés. (cyclique)
    turn *future; // Les tours annulés.
    int first_past; // L'indice du premier tour passé. (pour le cycle)
    int size_past; // La taille de past.
    int size_future; // La taille de future.
    int capacity_past; // La capacité de past.
    int capacity_future; // La capacité de future.
}
history;

history* create_history(void) {
    history *h = malloc(sizeof(history));
    if (h == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    h->past = malloc(sizeof(turn));
    if (h->past == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    h->future = malloc(sizeof(turn));
    if (h->future == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    h->size_past = 0;
    h->size_future = 0;
    h->capacity_past = 1;
    h->capacity_future = 1;
    return h;
}

//************************
//*+ Méthodes statiques +*
//************************


// * @brief
// * agrandit la taille effective de l'historique principal.
// */
static void grow_past(history *h) {
    h->capacity_past *= 2;
    if (h->capacity_past >= HISTORY_MAX) // Contrainte de taille.
    {
        h->capacity_past = HISTORY_MAX;
    }
    h->past = realloc(h->past, h->capacity_past * sizeof(turn));
    if (h->past == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
}

// * @brief
// * agrandit la taille effective de l'historique secondaire.
// */
static void grow_future(history *h) {
    if (h->capacity_future >= HISTORY_MAX) // Contrainte de taille.
    {
        fprintf(stdout, "Demande d'agrandissement de l'historique secondaire déjà à sa taille maximale.\n");
        return;
    }
    h->capacity_future *= 2;
    h->future = realloc(h->future, h->capacity_future * sizeof(turn));
    if (h->future == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
}

// * @brief
// * réduit la taille effective de l'historique principal.
// */
static void reduice_past(history *h) {
    if (h->capacity_past <= 1) // Sécurité.
    {
        return;
    }
    h->capacity_past /= 2;
    turn *new_past = malloc(h->capacity_past * sizeof(turn));
    if (new_past == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < h->size_past; i++) {
        new_past[i] = h->past[(h->first_past + i) % h->capacity_past];
    }
    free(h->past);
    h->past = new_past;
    h->first_past = 0;
}

// * @brief
// * réduit la taille effective de l'historique secondaire.
// */
static void reduice_future(history *h) {
    if (h->capacity_future <= 1) // Sécurité.
    {
        return;
    }
    h->capacity_future /= 2;
    h->future = realloc(h->future, h->capacity_future * sizeof(turn));
    if (h->future == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
}

// * @brief
// * supprime les tours annulés de l'historique.
// */
static void kill_future(history *h) {
    free(h->future);
    h->future = malloc(sizeof(turn));
    if (h->future == NULL) {
        fprintf(stderr, "Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    h->size_future = 0;
    for (int i = 0; i < h->size_past; i++) {
        if (h->past[(h->first_past + i) % h->capacity_past].type == T_POLY) {
            free(h->past[(h->first_past + i) % h->capacity_past].minokilled); // On libère les tableaux de booléens.
        }
    }
    h->capacity_future = 1;
}

// * @brief
// * renvoie le dernier tour passé.
// * supprime ce tour de l'historique.
// * @return le dernier tour passé.
// */
static turn pop_past(history *h) {
    if (h->size_past == 0) {
        fprintf(stderr, "Demande de récupération d'un tour passé dans un historique vide.\n");
        exit(EXIT_FAILURE);
    }
    h->size_past--;
    const turn t = h->past[(h->size_past + h->first_past) % h->capacity_past];
    if (h->size_past < h->capacity_past / 4) {
        reduice_past(h);
    }
    return t;
}

// * @brief
// * renvoie le dernier tour annulé.
// * supprime ce tour de l'historique.
// * @return le dernier tour annulé.
// */
static turn pop_future(history *h) {
    if (h->size_future == 0) {
        fprintf(stderr, "Demande de récupération d'un tour futur dans un historique vide.\n");
        exit(EXIT_FAILURE);
    }
    h->size_future--;
    const turn t = h->future[h->size_future];
    if (h->size_future < h->capacity_future / 4) {
        reduice_future(h);
    }
    return t;
}

// * @brief
// * ajoute un tour passé à l'historique.
// */
static void add_past(const turn t, history *h) {
    if (h->size_past == HISTORY_MAX) {
        // On remplace le plus ancien tour par le nouveau.
        if (h->past[h->first_past].type == T_POLY) {
            free(h->past[h->first_past].minokilled); // On libère le tableau de booléens.
        }
        h->past[h->first_past] = t;
        h->first_past = (h->first_past + 1) % h->capacity_past;
    }
    else {
        if (h->size_past == h->capacity_past) {
            grow_past(h);
        }
        h->past[(h->size_past + h->first_past) % h->capacity_past] = t;
        h->size_past++;
    }
    if (h->size_future > 0) {
        kill_future(h); // On ne peut pas restaurer un tour après en avoir joué un nouveau.
    }
}

// * @brief
// * ajoute un tour annulé à l'historique secondaire.
// */
static void add_future(const turn t, history *h) {
    if (h->size_future == h->capacity_future) {
        grow_future(h);
    }
    h->future[h->size_future] = t;
    h->size_future++;
}

void free_history(history *h) {
    free(h->past);
    free(h->future);
    free(h);
}

void add_entry_history(const turn t, history *h) {
    add_past(t, h); //redondant mais j'avais déjà codé ça :'(
}

int sizenext_history(history *h) {
    return h->size_future;
}

int sizeprev_history(history *h) {
    return h->size_past;
}

void rewind_history(history *h) {
    if (h->size_past == 0) {
        return;
    }
    const turn t = pop_past(h);
    add_future(t, h);
}

void continue_history(history *h) {
    if (h->size_future == 0) {
        return;
    }
    const turn t = pop_future(h);
    add_past(t, h);
}

turn* last_move_history(history *h) {
    if (h->size_past == 0) {
        return NULL;
    }
    return &h->past[(h->size_past + h->first_past - 1) % h->capacity_past];
}

turn* next_move_history(history *h) {
    if (h->size_future == 0) {
        return NULL;
    }
    return  &h->future[h->size_future - 1];
}




