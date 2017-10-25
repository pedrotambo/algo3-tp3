#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>

std::random_device rd;
std::mt19937 generator(rd());

const int PLAYER_1 = 1;
const int PLAYER_2 = 2;


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



struct Game{
    Game(int _rows, int _cols, int _c, int _p, int _current_player){
        c = _c;
        rows = _rows;
        cols = _cols;
        current_player = _current_player;
        p = _p;

        board = std::vector< std::vector<int>>(rows, std::vector<int>(cols));
        heights = std::vector<int>(cols); //cada indice (i) de este vector indica el alto de la columna (i)

    }

    std::vector< std::vector<int>> board;
    std::vector<int> heights;
    int c;
    int rows;
    int cols;
    int p;
    int current_player;
};

// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game g, int move) {
    return 0;
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
int greedy_move(Game g, float strategy_weight, float diagonal_weight, float centered_weight) {
    // Las posibles jugadas son las alturas de cada columna en tanto ninguna de ellas haya alcanzado el máximo

    int best_movement = -1;
    int best_movement_score = -1;
    for (int i = 0; i < g.heights.size(); i++) {
        if (g.heights[i] < g.rows) {
            // Es una jugada posible

            int possible_move = g.heights[i] + 1;
            int possible_move_score = calculate_move_score(g, possible_move);

            if (best_movement_score < possible_move_score) {
                best_movement = possible_move;
                best_movement_score = possible_move_score;
            }
        }
    }

    return best_movement;
}

void play(Game g, int movement) {
    int height = g.heights[movement]++;
    g.board[height][movement] = g.current_player;
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

        if (current_player == PLAYER_1) {
            int best_move = greedy_move(g, 0.0, 0.0, 0.0);
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
            int best_move = greedy_move(g, 0.0, 0.0, 0.0);
            play(g, best_move);
        }
    }

    return 0;
}