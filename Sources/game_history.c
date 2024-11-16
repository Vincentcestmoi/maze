#include "game_history.h"

// La straucture pour représenter un historique: à définir.
typedef struct history {
    int dummy;
    // Ce champ est à supprimer (il est là car C n'accepte pas de définir un struct sans champ).
}
history;

history* create_history(void) {
    return NULL;
}

void free_history(history*) {
    return;
}

void add_entry_history(turn, history*) {
    return;
}

int sizenext_history(history*) {
    return 0;
}

int sizeprev_history(history*) {
    return 0;
}

void rewind_history(history*) {
    return;
}

void continue_history(history*) {
    return;
}

turn* last_move_history(history*) {
    return NULL;
}

turn* next_move_history(history*) {
    return NULL;
}




