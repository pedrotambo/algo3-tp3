#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <algorithm>  // std::max

#define PLAYER_1 '1'
#define PLAYER_2 '2'
#define EMPTY '-'
#define INFINITE 1000

// #define DEBUG

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

void show_board(std::vector< std::vector<char > > &board){

    for(int i = (int)board.size()-1; i > -1; i--){
        for(int j = 0; j < (int)board[i].size(); j++){
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

struct Game{
    Game(int rows, int cols, int _c, int _max_p, int _p){
        c = _c;
        p = _p;
        max_p = std::min(_max_p, rows*cols);
        last_move = -1; // Si es -1, todavía el juego no empezó

        for(int i = 0; i < rows; i++){
            std::vector<char> c(cols, '-');
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

};

void update_game(Game &g, char player, int move){
    g.board[g.heights[move]][move] = player;
    g.heights[move]++;
    g.p++;
    g.last_move = move;
}


void outdate_game(Game &g, char player, int move){
    g.heights[move]--;
    g.board[g.heights[move]][move] = EMPTY;
    g.p--;
    //g.last_move = move;
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
    return (g.p >= g.max_p)? true:false;

}

bool possible_move(Game &g, int move){
    return (g.heights[move] < (int)g.board.size())? true:false;
}

char next(char player){
    return (player == PLAYER_1)? PLAYER_2:PLAYER_1;
}

void set_arguments(Game &g, int &cols, int &last_col, int &h, int &last_row){
    last_col = g.last_move;
    h = g.heights[last_col];
    last_row = h-1;
    cols = g.board[0].size();
}

struct Move{
    Move(int v, int t): value(v), move(t) {}
    Move(){
        value = 0;
        move = 0;
    }
    int value;
    int move;    
};

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


Move optimal_move(Game &g, char player, int alfa, int beta){
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

    /* alfa-beta pruning:
     *   - alfa y beta: no son globales, son la mejor opción desde la raíz hasta nuestro estado actual.
     *       alfa es la mejor opción para el maximizador, beta es la mejor opción para el minimizador.
     *   - Cuándo: A la primer ocurrencia de conseguir algo mejor respecto a lo mejor que pudo conseguir nuestro rival.
     *   - Razón : El rival va preferir esta primer ocurrencia respecto a otras opciones disponibles que podrían ser mejores. 
     */
    if (player == PLAYER_1) {
        for (int move = 0; move < cols; move++) {
            if (possible_move(g, move)) {
                update_game(g, PLAYER_1, move);
                Move m = optimal_move(g, PLAYER_2, alfa, beta);
                outdate_game(g, PLAYER_1, move);

                max_move(res, m, move);
                alfa = std::max(alfa, res.value);

                if (beta <= alfa) {  // Poda beta
                    res.value = alfa;
                    break;
                }
            }
        }
    } else {
        for (int move = 0; move < cols; move++) {
            if (possible_move(g, move)) {
                // Es el player 2, oponente
                update_game(g, PLAYER_2, move);
                Move m = optimal_move(g, PLAYER_1, alfa, beta);
                outdate_game(g, PLAYER_2, move);

                min_move(res, m, move);
                beta = std::min(beta, res.value);

                if (beta <= alfa) {  // Poda alfa
                    res.value = beta;
                    break;
                }
            }
        }
    }

    // Luego de revisar todo el árbol de recursión, si no hay jugada ganadora ni de empate
    // Reviso si puedo taparle una línea próxima a llenar del rival.
    if(res.value == -10 && player == PLAYER_1){
        for(int move = 0; move < cols; move++){
            if (possible_move(g, move)){
                // Es el player 2, oponente
                update_game(g, PLAYER_2, move);
                if(finished(g)){
                    res.move = move;
                    outdate_game(g, PLAYER_2, move);
                    break;
                }
                outdate_game(g, PLAYER_2, move);
            }
        }
    }


    return res;
}


bool valid_move(Game &g, int move){
    return possible_move(g, move) and move < (int)g.board[0].size() and move >= 0;
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
        
        Game g(rows, columns, c, p, 0);
        
        go_first = read_str();
        if (go_first == "vos") {
            Move m = optimal_move(g, PLAYER_1, -INFINITE, INFINITE);
            move = m.move;
            update_game(g, PLAYER_1, move);
            send(move);
        }

        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }

            
            // Actualizo tablero con la jugada del oponente
            int opponent_move = std::stoi(msg);
            
            #ifdef DEBUG
                while(not valid_move(g, opponent_move)){
                    std::cout << "Please, insert valid move: " << std::endl;
                    msg = read_str();
                    opponent_move = std::stoi(msg);
                }
            #endif

            update_game(g, PLAYER_2, opponent_move);
            
            #ifdef DEBUG
                show_board(g.board);
                if(finished(g)){std::cout << "El juego terminó." << std::endl;return 0;}
            #endif
            
            // calculo la jugada optima
            move = optimal_move(g, PLAYER_1, -INFINITE, INFINITE).move;

            // Actualizo el tablero con mi jugada
            update_game(g, PLAYER_1, move);

            #ifdef DEBUG
                show_board(g.board);
                if(finished(g)){std::cout << "El juego terminó." << std::endl;return 0;}
            #endif

            send(move);
        }
    }

    return 0;
}
