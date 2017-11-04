#include "greedy.h"
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
//void evaluate_population(Game &g, std::vector< std::vector<int> > &population, std::vector<int> &scores){
void evaluate_population(Game &g, std::vector<Individual> &population){

	for(unsigned int i = 0; i < population.size(); i++){
		// Evalúo el individuo i contra todos los demás.
		population[i].score = fitness_score_alternative(g, population[i], population);

	}
}

// Primer método de selección: La mitad mejor genera la nueva población (diapos).
void selection(std::vector<Individual> &population){
	// Como los ordeno por puntaje, basta hacer un sort decreciente en base a su score.
	std::sort(population.rbegin(), population.rend());

}


// Pre: probability tiene que ser un float entre [0,1]
// Post: Devuelve una nueva poblacion
void crossover(std::vector<Individual> &population, float probability){
	
	for(unsigned int i = 0; i < population.size()/2; i+=2){
		//float d = (float)(rand() % PRECISION) / (float)PRECISION;
		// Cruzo los individuos i e i+1 y genero otros dos individuos a partir de ellos (diapos)
		Individual son_1(population[i].genome.size());
		for(unsigned int k = 0; k < population[i].genome.size(); k++){
			float d = (float)(rand() % PRECISION) / (float)PRECISION;
			son_1.genome[k] = (d < probability)? population[i].genome[k]:population[i+1].genome[k];
		}

		Individual son_2(population[i].genome.size());
		for(unsigned int k = 0; k < population[i].genome.size(); k++){
			float d = (float)(rand() % PRECISION) / (float)PRECISION;
			son_2.genome[k] = (d < probability)? population[i].genome[k]:population[i+1].genome[k];
		}

		population[population.size()/2 + i] = son_1;
		population[population.size()/2 + i + 1] = son_2;
		

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

int main() {

    srand (time(NULL));
    // Es necesario que sea PAR y que su mitad también lo sea. 
    int lenght_population = 100;
	int parameters_lenght = 7;
    // Juego base
    int c = 7;
    Game g(10, 10, c, 100, PLAYER_1);
    
    
    std::vector<Individual> population = initial_population(lenght_population, parameters_lenght);
    
    int i = 0;
    do {
    	evaluate_population(g, population);
    	selection(population);
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