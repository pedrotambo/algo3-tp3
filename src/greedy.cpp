#include <random>
#include "greedy.h"


// Devuelve un arreglo donde cada índice (i) indica la cantidad de juegos formados de tamaño i
std::vector<int> lines_lenghts(Game &g){

    std::vector<int> result(g.cols, 0);

    for (int j = 0; j < g.rows; j ++) {
        for (int i = 0; i < g.cols - 1; i++) {
            int partial_count = 1;

            while (i + 1 < g.cols and g.board[j][i] == g.board[j][i+1] and g.board[j][i] == PLAYER_1) {
                partial_count++;
                i++;
            }

            result[partial_count]++;
        }
    }

    for (int j = 0; j < g.cols; j++) {
        for (int i = 0; i < g.rows - 1; i++) {
            int partial_count = 1;

            while (i + 1 < g.rows and g.board[i][j] == g.board[i+1][j] and g.board[i][j] == PLAYER_1) {
                partial_count++;
                i++;
            }

            result[partial_count]++;
        }
    }

    /*for (int i = 0; i < (int)result.size(); i++) {
        std::cerr << "i: " << i << " #: " << result[i] << std::endl;
    }*/

    result[0] = 0;
    result[1] = 0;
    return result;
}

// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game &g, int movement, std::vector<int>& parameters) {
    do_move(g, movement);
    std::vector<int> lines = lines_lenghts(g);

    int score = 0;
    for(int i = 0; i < (int)parameters.size(); i++){
        score += lines[i]*parameters[i];
    }

    undo_move(g, movement);
    return score;
}

/**
 * Parametrización:
 * 1. El primer parámetro indica que estrategia tiene mas peso, de ser 1 significará que se basará estrictamente
 *    en el juego propio (colocar la ficha donde pueda formar el mayor valor), mientras que si es 0, se basará estricta-
 *    mente en el juego del rival buscando donde bloquear su mejor juego, los valores intermedios serán combinaciones.
 * 2. El segundo parámetro indica el peso de colocar una ficha buscando formar un juego por una diagonal vs hacerlo
 *    horizontal o verticalmente (1 será buscar juegos por solo diagonales, 0 solo de modo horizontal o vertical, valores
 *    intermedios seran combinaciones)
 * 3. Dados dos juegos posibles similares indica si es mas importante colocar la ficha en las columnas centrales frente
 *    a alguna de los extremos. Entonces de ser 1 buscará colocar la ficha desde los extremos en primer lugar, y si es 0
 *    desde el centro, los valores intermedios... ni idea.
 */
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
    std::cerr << best_movement << " " << g.heights[best_movement] << std::endl;
    return best_movement;
}


int main() {

    //std::default_random_engine generator;
    std::string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;

    while (true) {
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();
        
        Game g(rows, columns, c, p, PLAYER_2);

        // TODO leer params
        std::vector<int> parameters(c, 5);
        
        go_first = read_str();
        if (go_first == "vos") {
            g.current_player = PLAYER_1;
            int m = greedy_move(g, parameters);
            do_move(g, m);
            send(m);
        }

        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }

            // Actualizo tablero con la jugada del oponente
            int opponent_move = std::stoi(msg);
            
            do_move(g, opponent_move);
                        
            // calculo la jugada optima
            move = greedy_move(g, parameters);

            // Actualizo el tablero con mi jugada
            do_move(g, move);
            
            send(move);
        }
    }

    return 0;
}