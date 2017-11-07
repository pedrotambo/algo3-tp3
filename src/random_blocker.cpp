#include "random_blocker.h"

int calculate_move_score(Game &g, char player, int movement){
    do_move(g, movement);

    std::vector<int> number_of_lines_of_length;
    std::vector<int> number_of_effective_lines_of_length;
    std::vector<int> number_of_possible_lines_of_length;

    calculate_game_info(g, player, number_of_lines_of_length, number_of_effective_lines_of_length, number_of_possible_lines_of_length);

    std::vector<int> rival_number_of_lines_of_length;
    std::vector<int> rival_number_of_effective_lines_of_length;
    std::vector<int> rival_number_of_possible_lines_of_length;

    calculate_game_info(g, next_player(player), rival_number_of_lines_of_length, rival_number_of_effective_lines_of_length, rival_number_of_possible_lines_of_length);

    int score = 0;

    if(0 < rival_number_of_possible_lines_of_length[g.c]) score = -INFINITE;
    if(number_of_lines_of_length[g.c]) score = INFINITE;

    undo_move(g, movement);
    return score;
}

int random_blocker_move(Game &g){
    std::vector<movement_score> valid_movements;
    for (int i = 0; i < g.cols; i++) {
        if (valid_move(g, i)) {
            movement_score mc;
            mc.movement = i;
            mc.score = calculate_move_score(g, g.current_player, i);
            // std::cerr << "mov: " << mc.movement << " score: " << mc.score << std::endl;
            valid_movements.push_back(mc);
        }
    }

    return random_max(valid_movements);
}

int random_move(Game &g){
    std::vector<movement_score> valid_movements;
    for (int i = 0; i < g.cols; i++) {
        if (valid_move(g, i)) {
            movement_score mc;
            mc.movement = i;
            mc.score = 0;
            valid_movements.push_back(mc);
        }
    }
    return random_max(valid_movements);
}
