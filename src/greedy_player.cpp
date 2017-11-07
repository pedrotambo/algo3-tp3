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

        // 7 en linea
        std::vector<int> p = {5, 15, 25, 40, 90};
        std::vector<int> q = {2, 8, 12, 30, 70, 99};
        std::vector<int> s = {1, 2, 10, 25};
        std::vector<int> t = {1, 2, 5, 10};
        std::vector<int> parameters = p;
        parameters.insert(parameters.end(), q.begin(), q.end());
        parameters.insert(parameters.end(), s.begin(), s.end());
        parameters.insert(parameters.end(), t.begin(), t.end());

        // // 4 en linea
        // std::vector<int> p = {5, 15};
        // std::vector<int> q = {3, 20, 50};
        // std::vector<int> s = {10};
        // std::vector<int> t = {5};
        // std::vector<int> parameters = p;
        // parameters.insert(parameters.end(), q.begin(), q.end());
        // parameters.insert(parameters.end(), s.begin(), s.end());
        // parameters.insert(parameters.end(), t.begin(), t.end());
      
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
