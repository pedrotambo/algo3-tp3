#ifndef SRC_GAME_H
#define SRC_GAME_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <stdio.h>

#define PLAYER_1 '1'
#define PLAYER_2 '2'
#define EMPTY '-'
#define TIED '-'
#define INFINITE 10000

struct Game{
    Game(int _rows, int _cols, int _c, int _max_p, char start_player) {
        cols = _cols;
        rows = _rows;
        c = _c;
        p = 0;
        max_p = std::min(_max_p, rows*cols);
        last_move = -1; // Si es -1, todavía el juego no empezó
        current_player = start_player;
        tied = false;
        
        for(int i = 0; i < rows; i++){
            std::vector<char> c(cols, EMPTY);
            board.push_back(c);
            
        }
        for(int j = 0; j < cols; j++){
            heights.push_back(0);
        }
    }
    std::vector< std::vector<char > > board;
    std::vector<int> heights;
    int c; // c-linea
    int p; // cant fichas puestas
    int max_p; // min(fichas disponibles, filas * columnas)
    int last_move;
    char current_player;
    int cols, rows;
    bool tied;

};

// Game functions
char next_player(char player);
void do_move(Game &g, int move);
void undo_move(Game &g, int move);
bool out_of_index(Game &g, int i, int j);
bool finished(Game &g);
char winner(Game &g);
bool possible_move(Game &g, int move);
bool valid_move(Game &g, int move);
void show_board(std::vector< std::vector<char > > &board);

// IO Control
void send(const std::string& msg);
void send(int msg);
int read_int();
std::string read_str();

#endif //SRC_GAME_H