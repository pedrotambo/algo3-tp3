#ifndef SRC_GREEDY_H
#define SRC_GREEDY_H

#include "game.h"
#include <climits>
#include <algorithm>
#include <random>
#include <assert.h>

struct movement_score { 
    int movement; 
    int score; 
    bool operator<(const movement_score& other){
        return this->score < other.score;
    }
}; 

struct line_info {
    int start;
    int finish;
    int start_i;
    int start_j;
    int finish_i;
    int finish_j;
    int size;
};

int greedy_move(Game &g, std::vector<int>& parameters);

int random_max(std::vector<movement_score>& valid_movements);
void calculate_game_info(Game &g, char player, std::vector<int>& lines_of_length, std::vector<int>& effective_lines_of_length, std::vector<int>& possible_lines_of_length);

#endif //SRC_GREEDY_H
