#include "random_blocker.h"
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));

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
      
        go_first = read_str();
        if (go_first == "vos") {
            g.current_player = PLAYER_1;
            int m = random_blocker_move(g);
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
            move = random_blocker_move(g);

            // Actualizo el tablero con mi jugada
            do_move(g, move);

            send(move);
        }
    }

    return 0;
}
