#include "greedy.h"
#include "random_blocker.h"
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#define WIN 10
#define TIE 5
#define LOSE -2

std::random_device rd;
std::mt19937 generator(rd());
#define PRECISION 10000
#define N_OF_GAMES_VS_RANDOM_PLAYERS 100

struct Individual{
	Individual(){
		score = 0;
	}
	Individual(unsigned int genome_lenght){
		for(unsigned int i = 0; i < genome_lenght; i++){
            genome.push_back(0);    
        }
        score = 0;
	}
    
	bool operator<(const Individual& other){
		return this->score < other.score;
	}


	std::vector<int> genome;
	float score;
};

// Devuelve quien gano o si empataron
char fight(Game &g, std::vector<int>& input_genome, std::vector<int>& rival_genome) {
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, input_genome) : greedy_move(g, rival_genome);
        do_move(g, movement);
    }
    return winner(g);
}

// Definida como el procentaje de juegos no perdidos sobre el total, recibe el genoma a valuar y los de sus rivales
float fitness_score(Game &g, Individual &input_genome, std::vector<Individual> &enemies_genomes) {

    // Itero sobre cada genoma rival, y juego dos partidas para cada uno, una donde arranca el input_genome, y otro
    // donde arranca el rival
    int games_played = enemies_genomes.size() * 2;
    int games_lost = 0;
    for (unsigned int i = 0; i < enemies_genomes.size(); i++) {
        // Creo un nuevo juego a partir de los parámetros del original, TODO: crear constructor por copia
        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);

        int result = fight(g_home, input_genome.genome, enemies_genomes[i].genome);

        if (result == PLAYER_2)
            games_lost++;

        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        result = fight(g_away, input_genome.genome, enemies_genomes[i].genome);

        if (result == PLAYER_2)
            games_lost++;
    }
    return (float)(games_played - games_lost) / games_played;
}

// Definida como el procentaje de juegos no perdidos sobre el total, recibe el genoma a valuar y los de sus rivales
float fitness_score_alternative(Game &g, Individual &input_genome, std::vector<Individual> &enemies_genomes) {

    // Itero sobre cada genoma rival, y juego dos partidas para cada uno, una donde arranca el input_genome, y otro
    // donde arranca el rival
    int games_played = enemies_genomes.size() * 2;
    //int games_lost = 0;
    float score = 0;
    for (unsigned int i = 0; i < enemies_genomes.size(); i++) {
        // std::cerr << "    " << i+1 << "/" << enemies_genomes.size() << std::endl;
        // Creo un nuevo juego a partir de los parámetros del original, TODO: crear constructor por copia
        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);

        int result = fight(g_home, input_genome.genome, enemies_genomes[i].genome);

        if (result == PLAYER_1) score+= WIN;
        if (result == PLAYER_2) score+= LOSE;
        if (result == TIED) score+= TIE;

        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        result = fight(g_away, input_genome.genome, enemies_genomes[i].genome);

        if (result == PLAYER_1) score+= 3*WIN;
        if (result == PLAYER_2) score+= LOSE;
        if (result == TIED) score+= 2*TIE;

    }
    return score;
}

void show_population(std::vector<Individual> &population){
	for(unsigned int i = 0; i < population.size(); i++){
		std::cout << "Individual: " << i << ", Score: " << population[i].score;
		std::cout << ", Genome: ";
		for(unsigned int j = 0; j < population[i].genome.size(); j++){
			std::cout << population[i].genome[j] << " ";
		}
		std::cout << std::endl;
	}
}


std::vector<Individual> initial_population(int lenght_population, int parameters_lenght){
	std::vector<Individual> population(lenght_population);

	std::uniform_int_distribution<int> random_weight(0, 100);

    // Creo los genomas de manera aleatoria
    for (unsigned int i = 0; i < population.size(); i++) {
    	std::vector<int> genome(parameters_lenght);
        for(unsigned int j = 0; j < genome.size(); j++){
        	genome[j] = random_weight(generator);
        }
        population[i].genome = genome;
        population[i].score = 0;
    }

	return population;
}

// Evalúa la población y acumula los puntajes en scores
void evaluate_population(Game &g, std::vector<Individual> &population){

	for(unsigned int i = 0; i < population.size(); i++){
        // std::cerr << i+1 << "/" << population.size() << std::endl;
		// Evalúo el individuo i contra todos los demás.
		population[i].score = fitness_score_alternative(g, population[i], population);

	}
}

// Primer método de selección: La mitad mejor genera la nueva población (diapos).
void selection(std::vector<Individual> &population){
	// Como los ordeno por puntaje, basta hacer un sort decreciente en base a su score.
	std::sort(population.rbegin(), population.rend());

    // Me quedo con la mejor mitad
    population.resize(population.size() / 2);
}

void alternative_selection(std::vector<Individual> &population) {
    std::sort(population.rbegin(), population.rend());

    std::vector<Individual> selected(population.size());
    int i = 0;
    while (selected.size() < population.size() / 2) {
        float d = (float)(rand() % PRECISION) / (float)PRECISION;

        // Si el score del individuo es mayor a un random (entre 0 y 1) lo mantengo
        if (population[i].score > d) {
            selected.push_back(population[i]);
        }
        i++;
    }

    population = selected;
}


Individual crossover_individual(Individual father, Individual mother, float probability) {
    Individual son(father.genome.size());
    for(unsigned int k = 0; k < father.genome.size(); k++){
        float d = (float)(rand() % PRECISION) / (float)PRECISION;
        son.genome[k] = (d < probability)? father.genome[k]:mother.genome[k];
    }
    return son;

}
// Pre: probability tiene que ser un float entre [0,1]
// Post: Devuelve una nueva poblacion
void crossover(std::vector<Individual> &population, float probability){

    unsigned int pop_size = population.size();
	for(unsigned int i = 0; i < pop_size; i+=2){
		//float d = (float)(rand() % PRECISION) / (float)PRECISION;
		// Cruzo los individuos i e i+1 y genero otros dos individuos a partir de ellos (diapos)
        Individual father = population[i];
        Individual mother = population[i+1];

        population.push_back(crossover_individual(father, mother, probability));
        population.push_back(crossover_individual(father, mother, probability));
	}
}

// Muto solo los descendientes nuevos, la segunda mitad de población
void mutation(std::vector<Individual> &population, float probabilty){

	std::uniform_int_distribution<int> random_weight(0, 100);

	for(unsigned int i = population.size()/2; i < population.size(); i++){
		for(unsigned int k = 0; k < population[i].genome.size(); k++){
			float d = (float)(rand() % PRECISION) / (float)PRECISION;
			population[i].genome[k] = (d < probabilty)? random_weight(generator) : population[i].genome[k];
			//if(d < probabilty) std::cerr << "MUTATING!!!!" << std::endl;
		}


	}

}

/*****************************************************************************/
/*****************************************************************************/

char fight_vs_best_player(Game &g, std::vector<int>& mine_parameters, std::vector<int>& opponent_parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, mine_parameters) : greedy_move(g, opponent_parameters);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_vs_random_blocker(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters) : random_blocker_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_vs_random(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters) : random_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_vs_random_individual_from_population(Game &g, std::vector<int>& mine_parameters, std::vector<int>& opponent_parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, mine_parameters) : greedy_move(g, opponent_parameters);
        do_move(g, movement);
    }
    return winner(g);
}

Individual& get_best_player(std::vector<Individual>& population){
    Individual& mvp = population[0];
    for(unsigned int i = 1; i < population.size(); ++i){
        if(mvp.score < population[i].score)
            mvp = population[i];
    }
    return mvp;
}

float fitness_score_vs_pool_of_players(Game &g, std::vector<Individual>& population, Individual& input_genome){
    int games_played = 0;
    int games_won = 0;

    // Vs best player
    Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
    int result = fight_vs_best_player(g_home, input_genome.genome, get_best_player(population).genome);
    games_played++;
    if(result == PLAYER_1) games_won++;

    Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
    games_played++;
    result = fight_vs_best_player(g_away, input_genome.genome, get_best_player(population).genome);
    if(result == PLAYER_1) games_won++;

    // Vs random
    for(unsigned int i = 0; i < N_OF_GAMES_VS_RANDOM_PLAYERS; i++){
        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
        int result = fight_vs_random(g_home, input_genome.genome);
        games_played++;
        if(result == PLAYER_1) games_won++;

        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        games_played++;
        result = fight_vs_random(g_away, input_genome.genome);
        if(result == PLAYER_1) games_won++;
    }

    // Vs random_blocker
    for(unsigned int i = 0; i < N_OF_GAMES_VS_RANDOM_PLAYERS; i++){
        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
        int result = fight_vs_random_blocker(g_home, input_genome.genome);
        games_played++;
        if(result == PLAYER_1) games_won++;
        
        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        games_played++;
        result = fight_vs_random_blocker(g_away, input_genome.genome);
        if(result == PLAYER_1) games_won++;
    }

    // Vs random individuals from population: fight against 10% of the population
    for(unsigned int i = 0; i < population.size() * 0.10 ; i++){
        int r = rand() % population.size();
        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
        int result = fight_vs_random_individual_from_population(g_home, input_genome.genome, population[r].genome);
        games_played++;
        if(result == PLAYER_1) games_won++;
        
        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        games_played++;
        result = fight_vs_random_individual_from_population(g_away, input_genome.genome, population[r].genome);
        if(result == PLAYER_1) games_won++;
    }

    return (float)(games_won) / games_played;
}

void evaluate_population_with_pool_of_players(Game &g, std::vector<Individual>& population){
    for(unsigned int i = 0; i < population.size(); i++){
        std::cerr << " | " << i;
        population[i].score = fitness_score_vs_pool_of_players(g, population, population[i]);
    }
    std::cerr << std::endl;
}

/*****************************************************************************/
/*****************************************************************************/


int main() {

    srand (time(NULL));
    int c = 4;
    // Juego base
    Game g(6, 7, c, 100, PLAYER_1);

    // Es necesario que sea PAR y que su mitad también lo sea. 
    int lenght_population = 100;
	int parameters_lenght = (g.c-2) + (g.c-1) + (g.c-3)*2;
    std::vector<Individual> population = initial_population(lenght_population, parameters_lenght);
    
    // int lenght_population = 100-1;
    // Individual good_player;
    // good_player.genome = {30, 55, 20, 60, 90, 40, 50};
    // population.push_back(good_player);

    int i = 0;
    do {
        // evaluate_population(g, population);
    	evaluate_population_with_pool_of_players(g, population);
    	selection(population);
        // alternative_selection(population);
        crossover(population, 0.5);
    	mutation(population, 0.01);
    	show_population(population);
    	std::cout << i << std::endl;
    	i++;
    } while(i < 100); // acá iría el criterio de terminación

    show_population(population);


    // Test 2
    //int lenght_population = 40;
    //int parameters_lenght = 4;
    //int c = 3;
	//Game g(4, 4, c, 100, PLAYER_1);
    //std::vector<Individual> population = initial_population(lenght_population, parameters_lenght);
    //int i = 0;
    //do {
    //	evaluate_population(g, population);
    //	selection(population);
    //	crossover(population, 0.5);
    //	//population = crossover(population, 0.5);
    //	mutation(population, 0.25);
    //	//show_population(population);
    //	std::cout << i << std::endl;
    //	i++;
    //} while(i < 500); // acá iría el criterio de terminación
    //show_population(population);


}