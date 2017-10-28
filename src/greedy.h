#ifndef SRC_GREEDY_H
#define SRC_GREEDY_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>

const int PLAYER_1 = 1;
const int PLAYER_2 = 2;
const int EMPTY = 0;

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

void do_move(Game &g, int movement);

void undo_move(Game &g, int movement);

void show_board(std::vector< std::vector<int > > &board);

// Devuelve un arreglo donde cada índice (i) indica la cantidad de juegos formados de tamaño i
std::vector<int> lines_lenghts(Game &g);

void show_lines(Game &g);

// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game &g, int movement, std::vector<int> &weights);

int greedy_move(Game &g, std::vector<int> &weights);

void play(Game &g, int movement);

#endif //SRC_GREEDY_H
