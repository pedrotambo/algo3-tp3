#include "greedy.h"
#include <string>
#include <sstream>


bool out_of_index(Game &g, int i, int j){
    return (i >= g.rows or j >= g.cols or i < 0 or j < 0);
}


int finished(Game &g, int movement){
    // primero chequeo que el tablero no sea el vacío
    if(g.p == 0) return false;


    int last_col = movement;
    int c = g.c;
    int h = g.heights[last_col];
    int last_row = h-1;
    int cols = g.cols;
    int player = g.board[last_row][last_col];

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

    if(cant >= c) return player;

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
    if(cant >= c) return player;


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
    if(cant >= c) return player;
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

    if(cant >= c) return player;


    //si nadie gano, chequeo que no se hayan terminado las fichas.
    return (g.p >= g.max_p)? 0 : -1;

}

// Play ya estaba en uso je, devuelve 0 si empataron, 1 si gano el input, 2 si ganó el rival
int fight(Game &g, std::vector<int> input_genome, std::vector<int> rival_genome) {
    int movement, result;
    if (g.current_player == 1) {
        movement = greedy_move(g, input_genome);
        play(g, movement);
    }


    while ((result = finished(g, movement)) != -1) {
        movement = greedy_move(g, rival_genome);
        play(g, movement);
        if ((result = finished(g, movement)) != -1)
            break;

        movement = greedy_move(g, input_genome);
        play(g, movement);
    }

    return result;
}

// Definida como el procentaje de juegos no perdidos sobre el total, recibe el genoma a valuar y los de sus rivales
float fitness_score(Game &g, std::vector<int> input_genome, std::vector< std::vector<int>> enemies_genomes) {

    // Itero sobre cada genoma rival, y juego dos partidas para cada uno, una donde arranca el input_genome, y otro
    // donde arranca el rival
    int games_played = enemies_genomes.size() * 2;
    int games_lost = 0;
    for (int i = 0; i < enemies_genomes.size(); i++) {
        // Creo un nuevo juego a partir de los parámetros del original, TODO: crear constructor por copia
        Game g_aux(g.rows, g.cols, g.c, g.p, 1);

        int result = fight(g_aux, input_genome, enemies_genomes[i]);

        if (result == 2)
            games_lost++;

        g_aux = Game(g.rows, g.cols, g.c, g.p, 2);
        result = fight(g_aux, input_genome, enemies_genomes[i]);

        if (result == 2)
            games_lost++;
    }

    return (games_played - games_lost) / games_played;
}

std::random_device rd;
std::mt19937 generator(rd());

std::vector<int> generate_random_genome(int c) {
    std::uniform_int_distribution<int> random_weight(0, 100);

    std::vector<int> result(c);

    for (int i = 0; i < result.size(); i++) {
        result[i] = random_weight(generator);
    }

    return result;
}


int main() {

    // Juego base
    Game g(7, 6, 21, 4, 1);

    // Probablemente tiene sentido para un C mas grande que 4, el genoma es muy chico de esta forma
    std::vector< std::vector<int>> genomes(100);

    // Creo los genomas de manera aleatoria
    for (int i = 0; i < 100; i++) {
        genomes[i] = generate_random_genome(4);
    }

    // Evaluo a cada uno y printeo su score de fitness
    for (int i = 0; i < 100; i++) {
        std::stringstream result;
        std::copy(genomes[i].begin(), genomes[i].end(), std::ostream_iterator<int>(result, " "));

        std::cout << i << " " << result.str().c_str() << " " << fitness_score(g, genomes[i], genomes) << std::endl;
    }
}
