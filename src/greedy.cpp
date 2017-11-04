#include <random>
#include "greedy.h"

// Devuelve un arreglo donde cada índice (i) indica la cantidad de juegos formados de tamaño i
std::vector<int> lenght_lines(Game &g){

    std::vector<int> result(g.c + 1, 0);
    //std::vector<int> result(g.cols + 1, 0);

    // cuento lineas horizontales
    for (int i = 0; i < g.rows; i++) {
        for (int j = 0; j < g.cols - 1; j++) {
            int partial_count = 1;

            while (j + 1 < g.cols and g.board[i][j] == g.board[i][j+1] and g.board[i][j] == next_player(g.current_player)) {
                partial_count++;
                j++;
            }

            //Esto puede pasar, si tengo una cadena de X y una de Y separadas por un espacio
            // y pongo la ficha, puedo formar una de X+Y+1 que puede ser mayor que c
            partial_count = (partial_count > g.c)? g.c : partial_count;

            //result[partial_count]++;
            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1) result[partial_count]++;
                
            // Si es una cadena de uno y es mi ficha, sumo uno (esto lo hago unicamente en horizontal, luego no).
            if(partial_count == 1 && g.board[i][j] == next_player(g.current_player)) result[1]++;
        }
    }


    // cuento lineas verticales
    for (int j = 0; j < g.cols; j++) {
        for (int i = 0; i < g.rows - 1; i++) {
            int partial_count = 1;

            while (i + 1 < g.rows and g.board[i][j] == g.board[i+1][j] and g.board[i][j] == next_player(g.current_player)) {
                partial_count++;
                i++;
            }

            partial_count = (partial_count > g.c)? g.c : partial_count;
            
            //result[partial_count]++;
            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1) result[partial_count]++;
        }
    }


    result[0] = 0;
    result[1] = 0;
    return result;
}

// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game &g, int movement, std::vector<int>& parameters) {
    do_move(g, movement);
    std::vector<int> lines = lenght_lines(g);

    int score = 0;
    // Por ahora esto solo se hace con las lineas 
    for(unsigned int i = 0; i < lines.size() - 1; i++){
        score += lines[i]*parameters[i];
    }

    if (lines[g.c] > 0){
        // Si esto pasa, si pongo acá, GANO => MAX SCORE
        score = INT_MAX;
    }

    undo_move(g, movement);
    return score;
}

int greedy_move(Game &g, std::vector<int>& parameters) {
    // Las posibles jugadas son las alturas de cada columna en tanto ninguna de ellas haya alcanzado el máximo

    int best_movement = -1;
    int best_movement_score = -1;
    for (int i = 0; i < g.cols; i++) {
        if (valid_move(g, i)) {

            int possible_move = i;
            int possible_move_score = calculate_move_score(g, i, parameters);

            if (best_movement_score < possible_move_score) {
                best_movement = possible_move;
                best_movement_score = possible_move_score;
            }
        }
    }

    //best_movement = rand() % g.cols-1;
//    std::cerr << best_movement << " " << g.heights[best_movement] << std::endl;
    return best_movement;
}
