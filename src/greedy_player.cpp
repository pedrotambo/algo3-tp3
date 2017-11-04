#include "greedy.h"
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));

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
        //std::vector<int> parameters = {30, 47, 8, 57}; // Parametros del genetico para 4 4 3
        std::vector<int> parameters = {15, 18, 1, 3, 16, 33, 49, INT_MAX}; // Parametros del genetico para 10 10 7 BEST
        //std::vector<int> parameters = {68, 58, 4, 17, 29, 36, 71,  INT_MAX}; // Parametros del genetico 10 10 7
        //std::vector<int> parameters = {49, 37, 0, 51, 54, 58, 98,  INT_MAX}; // Parametros del genetico 10 10 7
        
        //std::vector<int> parameters = {1000, 10, 15, 20, 30, 40, 50,  INT_MAX}; // MANUAL
        //std::vector<int> parameters = {0, 10, 25, 50, 80, 140, 280, INT_MAX}; // MANUAL 

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
