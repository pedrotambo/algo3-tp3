#include "greedy.h"
#include <sstream>

// Devuelve quien gano o si empataron
char fight(Game &g, std::vector<int>& input_genome, std::vector<int>& rival_genome) {
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, input_genome) : greedy_move(g, rival_genome);
        do_move(g, movement);
    }
    return winner(g);
}

// Definida como el procentaje de juegos no perdidos sobre el total, recibe el genoma a valuar y los de sus rivales
float fitness_score(Game &g, std::vector<int> input_genome, std::vector< std::vector<int> > enemies_genomes) {

    // Itero sobre cada genoma rival, y juego dos partidas para cada uno, una donde arranca el input_genome, y otro
    // donde arranca el rival
    int games_played = enemies_genomes.size() * 2;
    int games_lost = 0;
    for (int i = 0; i < (int)enemies_genomes.size(); i++) {
        // Creo un nuevo juego a partir de los parámetros del original, TODO: crear constructor por copia
        Game g_home(g.rows, g.cols, g.c, g.p, PLAYER_1);

        int result = fight(g_home, input_genome, enemies_genomes[i]);

        if (result == PLAYER_2)
            games_lost++;

        Game g_away(g.rows, g.cols, g.c, g.p, PLAYER_2);
        result = fight(g_away, input_genome, enemies_genomes[i]);

        if (result == PLAYER_2)
            games_lost++;
    }

    return (games_played - games_lost) / games_played;
}

std::random_device rd;
std::mt19937 generator(rd());

std::vector<int> generate_random_genome(int c) {
    std::uniform_int_distribution<int> random_weight(0, 100);

    std::vector<int> result(c);

    for (int i = 0; i < (int)result.size(); i++) {
        result[i] = random_weight(generator);
    }

    return result;
}


int main() {

    // Juego base
    Game g(7, 6, 4, 42, PLAYER_1);

    // Probablemente tiene sentido para un C mas grande que 4, el genoma es muy chico de esta forma
    std::vector< std::vector<int> > genomes(100);

    // Creo los genomas de manera aleatoria
    for (int i = 0; i < 100; i++) {
        genomes[i] = generate_random_genome(4);
    }

    std::ofstream genomes_scores;
    genomes_scores.open ("genomes_scores.txt");
    // Evaluo a cada uno y printeo su score de fitness
    for (int i = 0; i < 100; i++) {
        std::stringstream result;
        // std::copy(genomes[i].begin(), genomes[i].end(), std::ostream_iterator<int>(result, " "));
        // genomes_scores << i << " " << result.str().c_str() << " " << fitness_score(g, genomes[i], genomes) << "\n";
        for(int j = 0; j < (int)genomes[i].size(); j++){
            genomes_scores << genomes[i][j] << " ";
        }
        genomes_scores << fitness_score(g, genomes[i], genomes) << "\n";
    }
    genomes_scores.close();
}
