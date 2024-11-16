#include "game_strategies.h"


const char* str_names[STR_SIZE] = { "Immobile","Aléatoire", "Opposée", "Agressive","BFS" };
void(*str_funs[STR_SIZE]) (maze*, move, move*) = { &minotaurs_still,&minotaurs_random,&minotaurs_reverse,&minotaurs_closein,&minotaurs_bfs };


void minotaurs_still(maze*, move, move*) {
    return;
}

void minotaurs_random(maze*, move, move*) {
    return;
}

void minotaurs_reverse(maze*, move, move*) {
    return;
}

void minotaurs_closein(maze*, move, move*) {
    return;
}

void minotaurs_bfs(maze*, move, move*) {
    return;
}
