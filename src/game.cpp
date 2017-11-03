#include "game.h"

char next_player(char player){
    return (player == PLAYER_1)? PLAYER_2:PLAYER_1;
}

void do_move(Game &g, int move){
    g.board[g.heights[move]][move] = g.current_player;
    g.heights[move]++;
    g.p++;
    g.last_move = move;
    g.current_player = next_player(g.current_player);
}

void undo_move(Game &g, int move){
    g.heights[move]--;
    g.board[g.heights[move]][move] = EMPTY;
    g.p--;
    //g.last_move = move;
    g.current_player = next_player(g.current_player);
}

bool out_of_index(Game &g, int i, int j){
    int cols = g.board[0].size(); int rows = g.board.size();
    return (i >= rows or j >= cols or i < 0 or j < 0)? true:false;
}

/*  PRE := last_col = columna en la que fue puesta la última ficha, luego en g.heights[c] tendremos la altura
      y en g.board[last_row][last_col]  tendremos el jugador que hizo la movida 
    POST := devuelve TRUE si a partir de la última ficha puesta en last_col el jugador correspondiente gano o
            FALSE en caso contrario                                                                          */
bool finished(Game &g){

    // primero chequeo que el tablero no sea el vacío
    if(g.p == 0) return false;


    int last_col = g.last_move;
    int c = g.c;
    int h = g.heights[last_col];
    int last_row = h-1;
    int cols = g.board[0].size();
    char player = g.board[last_row][last_col];

    // voy por la horizontal (izquierda y derecha)
    // ------------  Esta direccion: ---  ---------
    int lim_left = std::max(0,last_col - c + 1);
    int cant = 1; // cuento la ficha puesta
    bool over = false;
    for(int j = last_col - 1; j >= lim_left and not over; j--){
        if (g.board[last_row][j] == player){
            cant++;
        } else {
            over = true;
        }
    }
    // cuanto cantidad de fichas hacia la der
    int lim_right = std::min(cols, last_col + c);
    over = false;
    for(int j = last_col + 1; j < lim_right and not over; j++){
        if (g.board[last_row][j] == player){
            cant++;
        } else {
            over = true;
        }
    }

    if(cant >= c) return true;

    // ahora voy por la vertical (solo para abajo, arriba no puede haber nada)
    cant = 1;
    over = false;
    int lim_down = std::max(0, last_row -c + 1);
    for(int i = last_row - 1; i >= lim_down and not over; i--){
        if (g.board[i][last_col] == player){
            cant++;
        } else {
            over = true;
        }
    }
    if(cant >= c) return true;


    // ahora voy por las diagonales
    // ------------  Esta direccion: / ---------
    cant = 1;
    over = false;
    int i = last_row + 1;
    int j = last_col + 1;
    while (not out_of_index(g, i, j) and not over and cant < c){
        if (g.board[i][j] == player){
            cant++; i++; j++;
        } else {
            over = true;
        }
    }

    i = last_row - 1;
    j = last_col - 1;
    over = false;
    while (not out_of_index(g, i, j) and not over and cant < c){
        if (g.board[i][j] == player){
            cant++; i--; j--;
        } else {
            over = true;
        }
    }    
    if(cant >= c) return true;
    // ------------------------------------------

    // ------------  Esta direccion: \ ---------
    cant = 1;
    over = false;
    i = last_row + 1;
    j = last_col - 1;
    while (not out_of_index(g, i, j) and not over and cant < c){
        if (g.board[i][j] == player){
            cant++; i++; j--;
        } else {
            over = true;
        }
    }

    i = last_row - 1;
    j = last_col + 1;
    over = false;
    while (not out_of_index(g, i, j) and not over and cant < c){
        if (g.board[i][j] == player){
            cant++; i--; j++;
        } else {
            over = true;
        }
    } 

    if(cant >= c) return true;


    //si nadie gano, chequeo que no se hayan terminado las fichas.
    g.tied = g.p >= g.max_p;
    return g.tied;

}

// PRE: finished(g) == True
// POST: PLAYER_i si gano el jugador i sino EMPTY
char winner(Game &g){
    if(g.tied)
        return TIED;
    return g.board[g.heights[g.last_move]-1][g.last_move];
}

bool possible_move(Game &g, int move){
    return g.heights[move] < (int)g.board.size();
}

bool valid_move(Game &g, int move){
    return possible_move(g, move) and move < (int)g.board[0].size() and move >= 0;
}

void show_board(std::vector< std::vector<char > > &board){
    for(int i = (int)board.size()-1; i > -1; i--){
        for(int j = 0; j < (int)board[i].size(); j++){
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// IO Control

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