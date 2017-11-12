#include "greedy.h"
#include "random_blocker.h"

std::random_device rd;
std::mt19937 generator(rd());


// Devuelve quien gano o si empataron
char fight(Game &g, std::vector<int>& input_genome, std::vector<int>& rival_genome) {
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, input_genome) : greedy_move(g, rival_genome);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_greedy_vs_random_blocker(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters) : random_blocker_move(g);
        do_move(g, movement);
    }
    return winner(g);
}


// Devuelve la cantidad de partidas no perdidas del input
int evaluate_parameters(Game &g, std::vector<int>& input_genome, std::vector< std::vector<int> >& validation_set) {
    std::cout << "Por evaluar: " << std::endl;
    for(unsigned int j = 0; j < input_genome.size(); j++){
        std::cout << input_genome[j] << " ";
    }
    std::cout << std::endl;

    int games_won = 0;
//    for (int i = 0; i < (int)validation_set.size(); i ++) {
//        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
//
//        int result = fight(g_home, input_genome, validation_set[i]);
//
//        if (result == PLAYER_1)
//            games_won++;
//
//        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
//        result = fight(g_away, input_genome, validation_set[i]);
//
//        if (result == PLAYER_1)
//            games_won++;
//    }

    int iterations_against_blocker = 30;
    for (int i = 0; i < iterations_against_blocker; i++) {
        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
        int result = fight_greedy_vs_random_blocker(g_home, input_genome);

        if(result == PLAYER_1) games_won++;

        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        result = fight_greedy_vs_random_blocker(g_away, input_genome);
        if(result == PLAYER_1) games_won++;
    }

    return games_won;
}

void find_parameters(std::vector<int> &parameters, std::vector< std::vector<int> > &validation_set, Game &g, int length, int start_at) {
    std::vector<int> best_results(length);
    int best_score = 0;
    for (int i = 0; i < std::pow(10, length); i++) {

        // Horrible indexing hack
        std::string foo = std::to_string(i);
        int missing_zeros = length - foo.length();
        while (missing_zeros > 0) {
            foo = '0' + foo;
            missing_zeros--;
        }

        for (int j = start_at; j < length + start_at; j++) {
            parameters[j] = (foo[j - start_at] - '0') * 10;
        }

        int games_not_lost = evaluate_parameters(g, parameters, validation_set);

        if (games_not_lost > best_score) {
            best_score = games_not_lost;
            for (int j = start_at; j < length + start_at; j++) {
                best_results[j - start_at] = parameters[j];
            }
        }
        std::cout << i << " de " << std::pow(10, length) << std::endl;
    }

    // Fijo los conseguidos y voy por el próximo segmento
    for (int j = start_at; j < length + start_at; j++) {
        parameters[j] = best_results[j - start_at];
    }


}

int main() {
    // Juego base
    Game g(15, 15, 5, 225, PLAYER_1);

    // TODO: modificar si se remueven o agregan parametros
    int parameters_lenght = (g.c-2) + (g.c-1) + (g.c-3)*2;

    std::vector<int> parameters(parameters_lenght);
    std::uniform_int_distribution<int> random_weight(0, 100);

    // Hay dos maneras de hacer grid-search, mediante cross-validation o evualuando contra un validation set independiente
    // Optamos por simplicidad por la segunda

    std::vector< std::vector<int> > validation_set(30);
    for (unsigned int i = 0; i < validation_set.size(); i++) {
        std::vector<int> genome(parameters_lenght);
        for(unsigned int j = 0; j < genome.size(); j++){
            genome[j] = random_weight(generator);
        }
        validation_set[i] = genome;
    }

    std::cout << "cantidad de parametros: " << parameters_lenght << std::endl;


    // Le asigno valores aleatorios
    for (int l = 0; l < parameters_lenght; l++) {
        parameters[l] = random_weight(generator);
    }

//    std::cout << "Determinando parametros 0, 1 y 2 (p)" << std::endl;
//    find_parameters(parameters, validation_set, g, g.c - 2, 0);
//    std::cout << "Determinando parametros 3, 4, 5 y 6 (q)" << std::endl;
//    find_parameters(parameters, validation_set, g, g.c - 1, 3);
//    std::cout << "Determinando parametro 7 y 8 (s)" << std::endl;
//    find_parameters(parameters, validation_set, g, g.c - 3, 7);
//    std::cout << "Determinando parametro 8 y 9 (t)" << std::endl;
//    find_parameters(parameters, validation_set, g, g.c - 3, 9);


    // villero as fuck
    find_parameters(parameters, validation_set, g, 2, 0);
    find_parameters(parameters, validation_set, g, 2, 2);
    find_parameters(parameters, validation_set, g, 2, 4);
    find_parameters(parameters, validation_set, g, 2, 6);
    find_parameters(parameters, validation_set, g, 2, 8);
    find_parameters(parameters, validation_set, g, 1, 10);

    std::cout << "El mejor arreglo es: ";
    for(unsigned int j = 0; j < parameters.size(); j++){
        std::cout << parameters[j] << " ";
    }
    std::cout << std::endl;

   return 0;
}
