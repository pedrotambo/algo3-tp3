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

void update_max(movement_score& m, movement_score& other){
    if(m.score < other.score){
        m.score = other.score;
        m.movement = other.movement;
    }
}

void update_min(movement_score& m, movement_score& other){
    if(m.score > other.score){
        m.score = other.score;
        m.movement = other.movement;
    }
}

movement_score minimax_optimal_move(Game &g, char player){
    if(finished(g)){
        movement_score ms;
        if(winner(g) == player) ms.score = 10;
        else if(winner(g) == next_player(player)) ms.score = -10;
        else if(winner(g) == TIED) ms.score = 0;
        return ms; 
    }

    // si llego aca, nadie ganó ni empató aun
    std::vector<movement_score> res;

    for(int move = 0; move < g.cols; move++){
        if(valid_move(g, move)){
            do_move(g, move);
            movement_score ms = minimax_optimal_move(g, player);
            ms.movement = move;
            res.push_back(ms);
            undo_move(g, move);
        }
    }
    return (g.current_player == player)? max(res) : min(res);
}

movement_score alfabeta_optimal_move(Game &g, char player, int alfa, int beta){
    /* alfa-beta pruning:
     *   - alfa y beta: no son globales, son la mejor opción desde la raíz hasta nuestro estado actual.
     *       alfa es la mejor opción para el maximizador, beta es la mejor opción para el minimizador.
     *   - Cuándo: A la primer ocurrencia de conseguir algo mejor respecto a lo mejor que pudo conseguir nuestro rival.
     *   - Razón : El rival va preferir esta primer ocurrencia respecto a otras opciones disponibles que podrían ser mejores. 
     */

    if(finished(g)){
        movement_score ms;
        if(winner(g) == player) ms.score = 10;
        else if(winner(g) == next_player(player)) ms.score = -10;
        else if(winner(g) == TIED) ms.score = 0;
        return ms; 
    }

    movement_score res;
    if(g.current_player == player){
        res.score = -INFINITE;
        for(int move = 0; move < g.cols; move++){
            if(valid_move(g, move)){
                do_move(g, move);
                movement_score ms = alfabeta_optimal_move(g, player, alfa, beta);
                ms.movement = move;
                undo_move(g, move);

                update_max(res, ms);
                alfa = std::max(alfa, res.score);

                if(beta <= alfa){  // Poda beta
                    res.score = alfa;
                    break;
                }
            }
        }
    } else {
        res.score = INFINITE;
        for(int move = 0; move < g.cols; move++){
            if(valid_move(g, move)){
                do_move(g, move);
                movement_score ms = alfabeta_optimal_move(g, player, alfa, beta);
                ms.movement = move;
                undo_move(g, move);

                update_min(res, ms);
                beta = std::min(beta, res.score);

                if(beta <= alfa){  // Poda alfa
                    res.score = beta;
                    break;
                }
            }
        }
    }

    return res;
}

// NO ANDA
movement_score minimax_greedy_optimal_move(Game &g, char player, int depth, std::vector<int>& parameters){
    if(finished(g)){
        movement_score ms;
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
            ms.movement = move;
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
    movement_score ms = alfabeta_optimal_move(g, g.current_player, -INFINITE, INFINITE);
    std::cerr << ms.movement << " | " << ms.score << std::endl;
    return ms.movement;  
}

int minimax_greedy_move(Game &g, std::vector<int>& parameters){
    return minimax_greedy_optimal_move(g, g.current_player, 0, parameters).movement;
}