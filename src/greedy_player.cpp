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
        // std::vector<int> parameters = {15, 18, 1, 3, 16, 33, 49 ,  INT_MAX}; // Parametros del genetico 10 10 7


        // Parametros 4 en linea: 6 rows, 7 cols, c == 4
        // p=[20, 40], q=[10, 30], r=[15, 65], s=[20], m=[[1, 2],[3, 4],[5, 6],[7, 8],[9, 10],[11, 12]] 
        std::vector<int> parameters = {20, 90, 10, 30, 15, 65, 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; 

        // Parametros 7 en linea: 10 rows, 10 cols, c == 7
        // p=[2, 10, 15, 25, 40], q=[2, 10, 15, 25, 40], r=[2, 10, 15, 25, 40], s=[2, 10, 15, 25], m=[[1, 1, 1, 2, 2],[2, 2, 2, 3, 3],[3, 3, 3, 4, 4],[4, 4, 4, 5, 5],[5, 5, 5, 6, 6],[6, 6, 6, 7, 7],[6, 6, 6, 7, 7],[7, 7, 7, 8, 8],[8, 8, 8, 9, 9],[9, 9, 9, 10, 10]] 
        // std::vector<int> parameters = {2, 10, 15, 25, 40, 2, 10, 15, 25, 40, 2, 10, 15, 25, 40, 2, 10, 15, 25, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10}; 
      
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
