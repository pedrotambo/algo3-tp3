#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <chrono>

#include "greedy.h"
#include "random_blocker.h"
#include "opt_players.h"

// #define DEBUG 1

struct stat info;

using namespace std::chrono;

high_resolution_clock::time_point start_clock () { return high_resolution_clock::now (); }

long stop_clock (high_resolution_clock::time_point &T_START, int iterations) {
    return (duration_cast<microseconds> (high_resolution_clock::now () - T_START).count ()) / iterations;
}

char fight_minimax_vs_alfabeta(Game &g){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? minimax_move(g) : alfabeta_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_greedy_vs_minimax(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters) : minimax_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_greedy_vs_alfabeta(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters) : alfabeta_move(g);
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

char fight_greedy_vs_random(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters) : random_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_greedy_vs_greedy(Game &g, std::vector<int>& parameters_p1, std::vector<int>& parameters_p2){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters_p1) : greedy_move(g, parameters_p2);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_greedy_vs_minimax_greedy(Game &g, std::vector<int>& parameters_p1, std::vector<int>& parameters_p2){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters_p1) : minimax_greedy_move(g, parameters_p2);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_minimax_greedy_vs_minimax_greedy(Game &g, std::vector<int>& parameters_p1, std::vector<int>& parameters_p2){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? minimax_greedy_move(g, parameters_p1) : minimax_greedy_move(g, parameters_p2);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_minimax_greedy_vs_random_blocker(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? minimax_greedy_move(g, parameters) : random_blocker_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_minimax_greedy_vs_random(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? minimax_greedy_move(g, parameters) : random_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_random_vs_random_blocker(Game &g){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? random_move(g) : random_blocker_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_random_vs_random(Game &g){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? random_move(g) : random_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

char fight_random_blocker_vs_random_blocker(Game &g){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? random_blocker_move(g) : random_blocker_move(g);
        do_move(g, movement);
    }
    return winner(g);
}

// Pelea de jugadores.
void do_fight(Game &g_run, int fight, int &result, std::vector<int> parameters = {}) {
    if (parameters.empty()) {
        // Experimentacion A)
        
        #ifdef DEBUG
            std::cout << "  ENTRO PELEA EXPERIMENTACION A -> " << fight << std::endl;
        #endif

        if (fight == 0) result = fight_minimax_vs_alfabeta(g_run);
        else if (fight == 8) result = fight_random_vs_random_blocker(g_run);
        else if (fight == 9) result = fight_random_vs_random(g_run);
        else if (fight == 10) result = fight_random_blocker_vs_random_blocker(g_run);
    } else {

        #ifdef DEBUG
            std::cout << "  ENTRO PELEA EXPERIMENTACION B -> " << fight << std::endl;
            std::cout << "      JUEGO -> FILAS: " << g_run.rows << ", COLS:  " << g_run.cols << ", C: " << g_run.c << ", P: " << g_run.p << std::endl;
            std::cout << "      PARAMETROS: ";
            for (auto i = parameters.begin(); i != parameters.end(); ++i)
                std::cout << *i << ' ';
            std::cout << std::endl;
        #endif

        // Experimentacion B), C), D)
        if (fight == 1) result = fight_greedy_vs_random(g_run, parameters);
        else if (fight == 2) result = fight_greedy_vs_random_blocker(g_run, parameters);
        else if (fight == 3) result = fight_greedy_vs_greedy(g_run, parameters, parameters);
        else if (fight == 4) result = fight_greedy_vs_minimax_greedy(g_run, parameters, parameters);
        else if (fight == 5) result = fight_minimax_greedy_vs_minimax_greedy(g_run, parameters, parameters);
        else if (fight == 6) result = fight_minimax_greedy_vs_random_blocker(g_run, parameters);
        else if (fight == 7) result = fight_minimax_greedy_vs_random(g_run, parameters);
    }
}

// Medicion de tiempo de pelea.
void measure_time(Game g,
    high_resolution_clock::time_point &T_START, high_resolution_clock::time_point &T_END,
    long &T_DURATION,
    int iterations, int &result, int fight, int &games_played, int &games_won, int &games_lost, int &games_tied,
    char PLAYER,
    std::vector<int> parameters = {}) {

    T_START = start_clock();
    for (int i = 0; i < iterations; ++i) {
        Game g_run(g.rows, g.cols, g.c, g.max_p, PLAYER);

        do_fight(g_run, fight, result, parameters);

        games_played++;
        if (result == PLAYER_1) games_won++;
        else if (result == PLAYER_2) games_lost++;
        else games_tied++;
    }
    T_DURATION = stop_clock(T_START, iterations);        
}

// fight y parameters opcionales, son utilizados para experimentaciones que contienen a greedy, etc.
void run(int _N, int _M, int _c, std::ofstream &of, int fight = 0, const std::vector<int> parameters = {}) {
    assert(of.is_open());
    assert(fight >= 0 && fight <= 10);
    if (!parameters.empty()) assert((int)parameters.size() == (_c-2) + (_c-1) + (_c-3)*2); // 4-linea == 2 + 3 + 2 == 7 == cantidad de parametros

    // Variables de medicion de tiempos.
    high_resolution_clock::time_point T_START, T_END;
    long T_DURATION;    

    // Variable resultado de pelea.
    int result;

    // Inicializacion
    int N = _N, M = _M, c = _c, max_p = N*M, games_played = 0, games_won = 0, games_lost = 0, games_tied = 0, iterations = 1;
    max_p = N*M;

    // Juego base.
    Game g(N, M, c, max_p, PLAYER_1);

    // Si es experimentacion A) -> fight = 0 y sin parameters.
    // Si es IDA -> PLAYER_1, sino si VUELTA -> PLAYER_2.

    // IDA.
    if (parameters.empty() and fight == 0) measure_time(g, T_START, T_END, T_DURATION, iterations, result, fight, games_played, games_won, games_lost, games_tied, PLAYER_1);
    else measure_time(g, T_START, T_END, T_DURATION, iterations, result, fight, games_played, games_won, games_lost, games_tied, PLAYER_1, parameters);    

    of << "ida," << N << "," << M << "," << c << "," << g.max_p << "," << result - '0' << "," << T_DURATION << "\n";
    
    #ifdef DEBUG
        std::cout << "      IDA time: " << T_DURATION << std::endl;
    #endif

    // VUELTA.
    if (parameters.empty() and fight == 0) measure_time(g, T_START, T_END, T_DURATION, iterations, result, fight, games_played, games_won, games_lost, games_tied, PLAYER_2);
    else measure_time(g, T_START, T_END, T_DURATION, iterations, result, fight, games_played, games_won, games_lost, games_tied, PLAYER_2, parameters);

    of << "vuelta," << N << "," << M << "," << c << "," << g.max_p << "," << result - '0' << "," << T_DURATION << "\n";
    
    #ifdef DEBUG
        std::cout << "      VUELTA time: " << T_DURATION << std::endl;
        std::cout << "  PLAYER_1 [won|lost|tied]: " << "[" << (float)games_won / games_played << "|" << (float)games_lost / games_played << "|" << (float)games_tied / games_played << "]" << std::endl << std::endl;
    #endif
}

void exp_a() {
    std::ofstream of_a;

    /* PLAYER_1 == minimax  VS  PLAYER_2 == alfabeta */
    #ifdef DEBUG
        std::cout << "EXPERIMENTO A: Tiempos minimax_vs_alfabeta" << std::endl;
    #endif

    of_a.open("csv/exp_a_minimax_vs_alfa.csv", std::ofstream::out);
    of_a << "home_away,filas,columnas,c,max_p,ganador,tiempo\n";
    run(4, 4, 3, of_a);
    of_a.close();
    
    of_a.open("csv/exp_a_minimax_vs_alfa.csv", std::ofstream::out | std::ofstream::app);
    run(4, 5, 3, of_a);
    of_a.close();

    of_a.open("csv/exp_a_minimax_vs_alfa.csv", std::ofstream::out | std::ofstream::app);
    run(5, 5, 3, of_a);
    of_a.close();    
}

void exp_b(std::vector<int> &parameters) {
    std::ofstream of_b;

    /* 1: greedy_vs_random
     * 2: greedy_vs_random_blocker
     * 3: greedy_vs_greedy
     * 4: greedy vs min_greedy
     * 5: min_greedy vs min_greedy
     * 6: min_greedy vs random_blocker.
     * 7: minimax_greedy vs random.
     * 8: random vs random_blocker.
     * 9: random vs random. 
     * 10: random_blocker vs random_blocker.      
     */

    #ifdef DEBUG
        std::cout << "EXPERIMENTO B: incremento de columnas." << std::endl;
    #endif

    for (int j = 1; j <= 10; ++j) {
        std::string fight_name;

        // Abro el archivo específico.
        if (j == 1) {
            fight_name = "csv/exp_b_greedy_vs_random.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == random */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 2) {
            fight_name = "csv/exp_b_greedy_vs_random_blocker.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == random_blocker */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 3) {
            fight_name = "csv/exp_b_greedy_vs_greedy.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == greedy */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 4) {
            fight_name = "csv/exp_b_greedy_vs_minimax_greedy.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == minimax_greedy */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 5) {
            fight_name = "csv/exp_b_minimax_greedy_vs_minimax_greedy.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == minimax_greedy */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 6) {
            fight_name = "csv/exp_b_minimax_greedy_vs_random_blocker.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == random_blocker */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 7) {
            fight_name = "csv/exp_b_minimax_greedy_vs_random.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == random */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 8) {
            fight_name = "csv/exp_b_random_vs_random_blocker.csv"; /* PLAYER_1 == random  VS  PLAYER_2 == random_blocker */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 9) {
            fight_name = "csv/exp_b_random_vs_random_csv"; /* PLAYER_1 == random  VS  PLAYER_2 == random */
            of_b.open(fight_name, std::ofstream::out);
        } else if (j == 10) {
            fight_name = "csv/exp_b_random_blocker_vs_random_blocker.csv"; /* PLAYER_1 == random_blocker  VS  PLAYER_2 == random_blocker */
            of_b.open(fight_name, std::ofstream::out);
        }

        // Experimentación. 3 <= c <= 7.
        for (int i = 3; i <= 10; ++i) {
            // Si es el primero, agrego headers, sino, abro en modo append pues cerrado por iteración anterior.
            if (i == 3) {
                of_b << "home_away,filas,columnas,c,max_p,ganador,tiempo\n";
            } else {
                of_b.open(fight_name, std::ofstream::out | std::ofstream::app);
            }

            // run(6, i, 4, of_b, j, parameters);
            run(10, i, 4, of_b, j, parameters);
            of_b.close();
        }
    }
}

void exp_c(std::vector<int> &parameters) {
    std::ofstream of_c;

    /* 1: greedy_vs_random
     * 2: greedy_vs_random_blocker
     * 3: greedy_vs_greedy
     * 4: greedy vs min_greedy
     * 5: min_greedy vs min_greedy
     * 6: min_greedy vs random_blocker.
     * 7: minimax_greedy vs random.
     * 8: random vs random_blocker.
     * 9: random vs random. 
     * 10: random_blocker vs random_blocker.      
     */
    
    #ifdef DEBUG
        std::cout << "EXPERIMENTO C: incremento de filas." << std::endl;
    #endif

    for (int j = 1; j <= 10; ++j) {
        std::string fight_name;

        // Abro el archivo específico.
        if (j == 1) {
            fight_name = "csv/exp_c_greedy_vs_random.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == random */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 2) {
            fight_name = "csv/exp_c_greedy_vs_random_blocker.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == random_blocker */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 3) {
            fight_name = "csv/exp_c_greedy_vs_greedy.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == greedy */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 4) {
            fight_name = "csv/exp_c_greedy_vs_minimax_greedy.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == minimax_greedy */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 5) {
            fight_name = "csv/exp_c_minimax_greedy_vs_minimax_greedy.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == minimax_greedy */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 6) {
            fight_name = "csv/exp_c_minimax_greedy_vs_random_blocker.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == random_blocker */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 7) {
            fight_name = "csv/exp_c_minimax_greedy_vs_random.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == random */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 8) {
            fight_name = "csv/exp_c_random_vs_random_blocker.csv"; /* PLAYER_1 == random  VS  PLAYER_2 == random_blocker */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 9) {
            fight_name = "csv/exp_c_random_vs_random_csv"; /* PLAYER_1 == random  VS  PLAYER_2 == random */
            of_c.open(fight_name, std::ofstream::out);
        } else if (j == 10) {
            fight_name = "csv/exp_c_random_blocker_vs_random_blocker.csv"; /* PLAYER_1 == random_blocker  VS  PLAYER_2 == random_blocker */
            of_c.open(fight_name, std::ofstream::out);
        }

        // Experimentación. 3 <= f <= 6.
        for (int i = 3; i <= 10; ++i) {
            // Si es el primero, agrego headers, sino, abro en modo append pues cerrado por iteración anterior.
            if (i == 3) {
                of_c << "home_away,filas,columnas,c,max_p,ganador,tiempo\n";
            } else {
                of_c.open(fight_name, std::ofstream::out | std::ofstream::app);
            }

            // run(i, 7, 4, of_c, j, parameters);
            run(i, 10, 4, of_c, j, parameters);
            of_c.close();
        }
    }
}

void exp_d(std::vector<int> &p1, std::vector<int> &p2, std::vector<int> &p3, std::vector<int> &p4) {
    std::ofstream of_d;

    /* 1: greedy_vs_random
     * 2: greedy_vs_random_blocker
     * 3: greedy_vs_greedy
     * 4: greedy vs min_greedy
     * 5: min_greedy vs min_greedy
     * 6: min_greedy vs random_blocker.
     * 7: minimax_greedy vs random.
     * 8: random vs random_blocker.
     * 9: random vs random. 
     * 10: random_blocker vs random_blocker.      
     */
    
    #ifdef DEBUG
        std::cout << "EXPERIMENTO C: incremento de filas." << std::endl;
    #endif

    for (int j = 1; j <= 10; ++j) {
        std::string fight_name;

        // Abro el archivo específico.
        if (j == 1) {
            fight_name = "csv/exp_d_greedy_vs_random.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == random */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 2) {
            fight_name = "csv/exp_d_greedy_vs_random_blocker.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == random_blocker */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 3) {
            fight_name = "csv/exp_d_greedy_vs_greedy.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == greedy */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 4) {
            fight_name = "csv/exp_d_greedy_vs_minimax_greedy.csv"; /* PLAYER_1 == greedy  VS  PLAYER_2 == minimax_greedy */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 5) {
            fight_name = "csv/exp_d_minimax_greedy_vs_minimax_greedy.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == minimax_greedy */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 6) {
            fight_name = "csv/exp_d_minimax_greedy_vs_random_blocker.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == random_blocker */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 7) {
            fight_name = "csv/exp_d_minimax_greedy_vs_random.csv"; /* PLAYER_1 == minimax_greedy  VS  PLAYER_2 == random */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 8) {
            fight_name = "csv/exp_d_random_vs_random_blocker.csv"; /* PLAYER_1 == random  VS  PLAYER_2 == random_blocker */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 9) {
            fight_name = "csv/exp_d_random_vs_random_csv"; /* PLAYER_1 == random  VS  PLAYER_2 == random */
            of_d.open(fight_name, std::ofstream::out);
        } else if (j == 10) {
            fight_name = "csv/exp_d_random_blocker_vs_random_blocker.csv"; /* PLAYER_1 == random_blocker  VS  PLAYER_2 == random_blocker */
            of_d.open(fight_name, std::ofstream::out);
        }

        // Experimentación. 4 <= c-linea <= 7.
        for (int i = 4; i <= 7; ++i) {
            // Si es el primero, agrego headers, sino, abro en modo append pues cerrado por iteración anterior.
            if (i == 4) {
                of_d << "home_away,filas,columnas,c,max_p,ganador,tiempo\n";
            } else {
                of_d.open(fight_name, std::ofstream::out | std::ofstream::app);
            }

            // (c, |parameters|), 4 7, 5 11, 6 15, 7 19.
            if (i == 4) run(10, 10, i, of_d, j, p1);
            else if (i == 5) run(10, 10, i, of_d, j, p2);
            else if (i == 6) run(10, 10, i, of_d, j, p3);
            else if (i == 7) run(10, 10, i, of_d, j, p4);

            of_d.close();
        }
    }
}


int main() {
    srand(time(NULL));

    // Creo directorio csv/ con todos los permisos en modo 0777 = rwxrwxrwx.
    mkdir("csv", ACCESSPERMS);

    /* Tiempos */
    std::vector<int> p1 = {52, 100, 24, 47, 95, 21, 11};
    std::vector<int> p2 = {52, 100, 24, 47, 95, 21, 11, 67, 31, 57, 25};
    std::vector<int> p3 = {52, 100, 24, 47, 95, 21, 11, 67, 31, 57, 25, 77, 79, 84, 76};
    std::vector<int> p4 = {52, 100, 24, 47, 95, 21, 11, 67, 31, 57, 25, 77, 79, 84, 76, 54, 49, 85, 28};

    exp_b(p1);              // b) Incremento columnas(M). N=6, M=3, c=4.
    exp_c(p1);              // c) Incremento filas(N). N=3, M=7, c=4.
    exp_d(p1, p2, p3, p4);  // d) Variar c=[4,5,6,7] en N=6, M=7 o 10 10.
    exp_a();                // a) Exactos.
}
