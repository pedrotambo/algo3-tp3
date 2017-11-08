#ifndef SRC_OPT_PLAYERS_H
#define SRC_OPT_PLAYERS_H

#include "game.h"

struct Move{
    Move(int v, int t): value(v), move(t) {}
    Move(){
        value = 0;
        move = 0;
    }
    int value;
    int move;    
};

int minimax_move(Game &g);
int alfabeta_move(Game &g);

#endif //SRC_OPT_PLAYERS_H
