#ifndef SRC_OPT_PLAYERS_H
#define SRC_OPT_PLAYERS_H

#include "game.h"

int minimax_move(Game &g);
int alfabeta_move(Game &g);
int minimax_greedy_move(Game &g, std::vector<int>& parameters);

#endif //SRC_OPT_PLAYERS_H
