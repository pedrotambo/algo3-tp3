#include "opt_players.h"
#include "greedy.h"
// tiene que ser positivo
#define MAX_RECURSION_DEPTH 2

movement_score max(std::vector<movement_score>& movements){
    movement_score res = movements[0];
    for(unsigned int i = 1; i < movements.size(); ++i){
        if(res < movements[i]){
            res = movements[i];
        }
    }
    return res;
}

movement_score min(std::vector<movement_score>& movements){
    movement_score res = movements[0];
    for(unsigned int i = 1; i < movements.size(); ++i){
        if(res > movements[i]){
            res = movements[i];
        }
    }
    return res;
}

// NO ANDA
movement_score minimax_optimal_move(Game &g, char player){
    if(finished(g)){
        movement_score ms;
        ms.movement = g.last_move;
        if(winner(g) == player) ms.score = INFINITE;
        else if(winner(g) == next_player(player)) ms.score = -INFINITE;
        else if(winner(g) == TIED) ms.score = 0;
        return ms; 
    }

    // si llego aca, nadie ganó ni empató aun
    std::vector<movement_score> res;

    for(int move = 0; move < g.cols; move++){
        if(valid_move(g, move)){
            do_move(g, move);
            movement_score ms = minimax_optimal_move(g, player);
            // ms.movement = move;
            res.push_back(ms);
            undo_move(g, move);
        }
    }
    return (g.current_player == player)? max(res) : min(res);
}

// NO ANDA
movement_score alfabeta_optimal_move(Game &g, char player){
    // FIXME!
    movement_score res;
    int i = 0;
    while(not valid_move(g, i)) i++;
    res.movement = i;
    return res;
}

// NO ANDA
movement_score minimax_greedy_optimal_move(Game &g, char player, int depth, std::vector<int>& parameters){
    if(finished(g)){
        movement_score ms;
        ms.movement = g.last_move;
        if(winner(g) == player) ms.score = INFINITE;
        else if(winner(g) == next_player(player)) ms.score = -INFINITE;
        else if(winner(g) == TIED) ms.score = 0;
        return ms; 
    }

    if(depth >= MAX_RECURSION_DEPTH){
        movement_score ms;
        ms.movement = g.last_move;
        ms.score = 0;// FRUTA, FIXME
        return ms;
    }

    // si llego aca, nadie ganó ni empató aun
    std::vector<movement_score> res;

    for(int move = 0; move < g.cols; move++){
        if(valid_move(g, move)){
            do_move(g, move);
            movement_score ms = minimax_greedy_optimal_move(g, player, depth+1, parameters);
            // ms.movement = move;
            res.push_back(ms);
            undo_move(g, move);
        }
    }
    return (g.current_player == player)? max(res) : min(res);
}


// Wrappers
int minimax_move(Game &g){
    return minimax_optimal_move(g, g.current_player).movement;  
}

int alfabeta_move(Game &g){
    return alfabeta_optimal_move(g, g.current_player).movement;  
}

int minimax_greedy_move(Game &g, std::vector<int>& parameters){
    return minimax_greedy_optimal_move(g, g.current_player, 0, parameters).movement;
}