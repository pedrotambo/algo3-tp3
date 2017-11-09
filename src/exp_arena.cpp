#include "greedy.h"
#include "random_blocker.h"
#include "opt_players.h"


char fight_minimax_vs_alfabeta(Game &g){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? minimax_move(g) : alfabeta_move(g);
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

char fight_greedy_vs_minimax(Game &g, std::vector<int>& parameters){
    while(not finished(g)){
        int movement = (g.current_player == PLAYER_1) ? greedy_move(g, parameters) : minimax_move(g);
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

int main() {
    
    srand(time(NULL));

    int N = 6;
    int M = 7;
    int c = 4;
    int max_p = N * M;

    // Juego base
    Game g(N, M, c, max_p, PLAYER_1);

    int games_played = 0;
    int games_won = 0;
    int games_lost = 0;
    int games_tied = 0;

    int iterations = 100;
    std::vector<int> parameters = {52, 100, 24, 47, 95, 21, 11};

    for(int i = 0; i < iterations; ++i){
        std::cerr << i << std::endl;

        Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
        int result = fight_greedy_vs_random_blocker(g_home, parameters);
        games_played++;
        if(result == PLAYER_1) games_won++;
        else if(result == PLAYER_2) games_lost++;
        else games_tied++;
        
        Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
        games_played++;
        result = fight_greedy_vs_random_blocker(g_away, parameters);
        if(result == PLAYER_1) games_won++;
        else if(result == PLAYER_2) games_lost++;
        else games_tied++;
    }

    // // Ejemplo Ida y vuelta: PLAYER_1 == MINIMAX  VS  PLAYER_2 == ALFA-BETA
    // Game g_home(g.rows, g.cols, g.c, g.max_p, PLAYER_1);
    // int result = fight_minimax_vs_alfabeta(g_home);
    // games_played++;
    // if(result == PLAYER_1) games_won++;
    // else if(result == PLAYER_2) games_lost++;
    // else games_tied++;
    
    // Game g_away(g.rows, g.cols, g.c, g.max_p, PLAYER_2);
    // games_played++;
    // result = fight_minimax_vs_alfabeta(g_away);
    // if(result == PLAYER_1) games_won++;
    // else if(result == PLAYER_2) games_lost++;
    // else games_tied++;

    std::cout << "PLAYER_1 [won|lost|tied]: " << "[" << (float)games_won / games_played << "|" << (float)games_lost / games_played << "|" << (float)games_tied / games_played << "]" << std::endl;

}