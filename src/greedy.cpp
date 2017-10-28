#include <iostream>
#include <fstream>
#include <random>
#include "greedy.h"

void send(const std::string& msg) {
    std::cout << msg << std::endl;
}

void send(int msg) {
    std::cout << msg << std::endl;
}

int read_int() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return std::stoi(msg);
}

std::string read_str() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        std::exit(0);
    }
    return msg;
}

void do_move(Game &g, int movement) {
    int height = g.heights[movement]++;
    g.board[height][movement] = g.current_player;
    g.p++;
}

void undo_move(Game &g, int movement){
    g.heights[movement]--;
    g.board[g.heights[movement]][movement] = EMPTY;
    g.p--;
}

void show_board(std::vector< std::vector<int > > &board){

    for(int i = (int)board.size()-1; i > -1; i--){
        for(int j = 0; j < (int)board[i].size(); j++){
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


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

void show_lines(Game &g){
	std::vector<int> result = lines_lenghts(g);

	for (int i = 2; i < (int)result.size(); i++) {
        std::cerr << "i: " << i << " #: " << result[i] << std::endl;
    }

}

// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game &g, int movement, std::vector<int> &weights) {
	do_move(g, movement);
    std::vector<int> lines = lines_lenghts(g);

    int score = 0;
    for(int i = 0; i < (int)weights.size(); i++){
    	score += lines[i]*weights[i];
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
int greedy_move(Game &g, std::vector<int> &weights) {
    // Las posibles jugadas son las alturas de cada columna en tanto ninguna de ellas haya alcanzado el máximo

    int best_movement = -1;
    int best_movement_score = -1;
    for (int i = 0; i < g.cols; i++) {
        if (g.heights[i] < g.rows) {
            // Es una jugada posible

            int possible_move = i;
            int possible_move_score = calculate_move_score(g, i, weights);

            if (best_movement_score < possible_move_score) {
                best_movement = possible_move;
                best_movement_score = possible_move_score;
            }
        }
    }

    //best_movement = rand() % g.cols-1;
    return best_movement;
}

void play(Game &g, int movement) {
    do_move(g, movement);
    if(g.current_player == PLAYER_1)
        send(movement);
}

int main() {
    //std::default_random_engine generator;
    std::string msg, color, oponent_color, go_first;
    int columns, rows, c, p;

    while (true) {
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();



        go_first = read_str();

        int current_player = PLAYER_2;
        if (go_first == "vos") {
            current_player = PLAYER_1;
        }

        Game g(rows, columns, c, p, current_player);
        // Nota: Según esta implementación soy siempre PLAYER_1

        std::vector<int> weights(g.cols, 0);
        for(int i = 2; i < weights.size(); i++) weights[i] = i;

        if (current_player == PLAYER_1) {
            int best_move = greedy_move(g, weights);
            play(g, best_move);
        }

        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }

            g.current_player = PLAYER_2;
            play(g, std::stoi(msg));

            g.current_player = PLAYER_1;

            int best_move = greedy_move(g, weights);
            lines_lenghts(g);
//            show_board(g.board);
//            show_lines(g);
            play(g, best_move);
        }
    }

    return 0;
}