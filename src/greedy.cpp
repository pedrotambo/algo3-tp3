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


void show_board(std::vector< std::vector<int > > &board){

    for(int i = (int)board.size()-1; i > -1; i--){
        for(int j = 0; j < (int)board[i].size(); j++){
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void show_lines(Game &g){
    std::vector<int> result = lines_lenghts(g);

    for (int i = 2; i < (int)result.size(); i++) {
        std::cerr << "i: " << i << " #: " << result[i] << std::endl;
    }
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