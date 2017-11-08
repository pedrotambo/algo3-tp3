#include "opt_players.h"

void set_arguments(Game &g, int &cols, int &last_col, int &h, int &last_row){
    last_col = g.last_move;
    h = g.heights[last_col];
    last_row = h-1;
    cols = g.board[0].size();
}

void max_move(Move& m1, Move m2, int move){
    if(m2.value > m1.value){
        m1.value = m2.value;
        m1.move = move; 
    }
}

void min_move(Move& m1, Move m2, int move){
    if(m2.value < m1.value){
        m1.value = m2.value;
        m1.move = move; 
    }
}

Move minimax_optimal_move(Game &g, char player){
    int cols, last_col, h, last_row;
    set_arguments(g, cols, last_col, h, last_row);

    Move res;
    
    if (finished(g)){
        char prev_player = g.board[last_row][last_col];
        res.move = g.last_move;
        res.value = (prev_player == PLAYER_1)? 10:-10;
        return res; 
    }
    // si no hubo ganador, chequeo si hay empate primero. No debería ser nunca >.
    if (g.p >= g.max_p){
        res.value = 0;
        return res;  
    } 

    // si llego aca, nadie ganó ni empató aun
    res.value = (player == PLAYER_1)? -INFINITE:INFINITE;

    for(int move = 0; move < cols; move++){
        if (possible_move(g, move)){
            if(player == PLAYER_1) {
                // Soy el player 1
                do_move(g, move);
                Move m = minimax_optimal_move(g, PLAYER_2);
                max_move(res, m, move);
                undo_move(g, move);
            } else {
                // Es el player 2, oponente
                do_move(g, move);
                Move m = minimax_optimal_move(g, PLAYER_1);
                min_move(res, m, move);
                undo_move(g, move);
            }
        }
    }

    // Luego de revisar todo el árbol de recursión, si no hay jugada ganadora ni de empate
    // Reviso si puedo taparle una línea próxima a llenar del rival.
    if(res.value == -10 && player == PLAYER_1){
        for(int move = 0; move < cols; move++){
            if (possible_move(g, move)){
                // Es el player 2, oponente
                do_move(g, move);
                if(finished(g)){
                    res.move = move;
                    undo_move(g, move);
                    break;
                }
                undo_move(g, move);
            }
        }
    }


    return res;
}

Move alfabeta_optimal_move(Game &g, char player){
    // FIXME!
    int i = 0;
    while(not valid_move(g, i)) i++;
    return i;
}

int minimax_move(Game &g){
    return minimax_optimal_move(g, g.current_player).move;  
}

int alfabeta_move(Game &g){
    return alfabeta_optimal_move(g, g.current_player).move;  
}