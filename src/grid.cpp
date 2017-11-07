#include "greedy.h"

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

// Devuelve la cantidad de partidas no perdidas del input
int evaluate_parameters(Game &g, std::vector<int>& input_genome, std::vector< std::vector<int> >& validation_set) {


//    std::cout << "Por evaluar: " << std::endl;
//    for(unsigned int j = 0; j < input_genome.size(); j++){
//        std::cout << input_genome[j] << " ";
//    }
//    std::cout << std::endl;

    int games_not_lost = 80;
    for (int i = 0; i < validation_set.size(); i ++) {
        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);

        int result = fight(g_home, input_genome, validation_set[i]);

        if (result == PLAYER_2)
            games_not_lost--;

        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        result = fight(g_away, input_genome, validation_set[i]);

        if (result == PLAYER_2)
            games_not_lost--;
    }

    return games_not_lost;
}

int main() {

    int c = 7;
    // Juego base
    Game g(10, 10, c, 100, PLAYER_1);

    // TODO: modificar si se remueven o agregan parametros
    int parameters_lenght = (g.c-2) + (g.c-1) + (g.c-3)*2;

    std::vector<int> parameters(parameters_lenght);
    std::uniform_int_distribution<int> random_weight(0, 100);

    // Hay dos maneras de hacer grid-search, mediante cross-validation o evualuando contra un validation set independiente
    // Optamos por simplicidad por la segunda

    std::vector< std::vector<int> > validation_set(40);
    for (unsigned int i = 0; i < 40; i++) {
        std::vector<int> genome(parameters_lenght);
        for(unsigned int j = 0; j < genome.size(); j++){
            genome[j] = random_weight(generator);
        }
        validation_set[i] = genome;
    }

    std::cout << "cantidad de parametros: " << parameters_lenght << std::endl;
    // Voy tomando de a 2 parametros, analizar en el futuro si el orden importa
    for (int i = 0; i < parameters_lenght; i+=2 ) {
        std::cout << "-------------------------" << std::endl;
        std::cout << "parametro: " << i << std::endl;
        // Siempre hay i parámetros ya fijados


        // Al resto de los parámetros le asigno valores aleatorios (los que no estoy buscando fijar)
        for (int l = i + 2; l < parameters_lenght; l++) {
            parameters[l] = random_weight(generator);
        }


        // Step: por ahora todos los parametros tienen step 10, ver si conviene que tengan diferente granularidad
        int best_values[2] = { 0, 0 };
        int best_score = 0;

        for (int j = 0; j < 100; j += 10) {
            for (int k = 0; k < 100; k += 10) {
                // Quiero probar cada combinación posible de los dos parámetros que busco fijar
                parameters[i] = j;
                parameters[i+1] = k;

                int games_not_lost = evaluate_parameters(g, parameters, validation_set);

                if (games_not_lost > best_score) {
                    best_score = games_not_lost;
                    best_values[0] = j;
                    best_values[1] = k;
//                    std::cout << "Nuevo mejor: " << best_score << std::endl;
                }
            }
        }

        // Fijo los mejores parámetros encontrados
        parameters[i] = best_values[0];
        parameters[i+1] = best_values[1];
        std::cout << "Mejores parámetros encontrados: " << best_values[0] << best_values[1] << std::endl;
    }

    std::cout << "El mejor arreglo es: ";
    for(unsigned int j = 0; j < parameters.size(); j++){
        std::cout << parameters[j] << " ";
    }
    std::cout << std::endl;

   return 0;
}
