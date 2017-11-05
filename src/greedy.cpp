#include "greedy.h"

// Devuelve un arreglo donde cada índice (i) indica la cantidad de juegos formados de tamaño i
std::vector<int> lenght_lines(Game &g){

    std::vector<int> result(g.c + 1, 0);

    // cuento lineas horizontales
    for (int i = 0; i < g.rows; i++) {
        for (int j = 0; j < g.cols - 1; j++) {
            int partial_count = 1;

            while (j + 1 < g.cols and g.board[i][j] == g.board[i][j+1] and g.board[i][j] == next_player(g.current_player)) {
                partial_count++;
                j++;
            }

            //Esto puede pasar, si tengo una cadena de X y una de Y separadas por un espacio
            // y pongo la ficha, puedo formar una de X+Y+1 que puede ser mayor que c
            partial_count = (partial_count > g.c)? g.c : partial_count;

            //result[partial_count]++;
            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1) result[partial_count]++;
                
            // Si es una cadena de uno y es mi ficha, sumo uno (esto lo hago unicamente en horizontal, luego no).
            if(partial_count == 1 && g.board[i][j] == next_player(g.current_player)) result[1]++;
        }
    }


    // cuento lineas verticales
    for (int j = 0; j < g.cols; j++) {
        for (int i = 0; i < g.rows - 1; i++) {
            int partial_count = 1;

            while (i + 1 < g.rows and g.board[i][j] == g.board[i+1][j] and g.board[i][j] == next_player(g.current_player)) {
                partial_count++;
                i++;
            }

            partial_count = (partial_count > g.c)? g.c : partial_count;
            
            //result[partial_count]++;
            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1) result[partial_count]++;

        }
    }

    // Diagonal /
    int down = 0;
    while(down < g.cols - 1){
        int i = 0;
        while(i < g.rows - 1 and down + i < g.cols - 1){
            int partial_count = 1;

            while(i < g.rows - 1 and down + i < g.cols - 1 and g.board[i][i+down] == g.board[i+1][i+down+1] and g.board[i][i+down] == next_player(g.current_player)){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            i += 1;
        }
        down += 1;
    }
    int up = 1;
    while(up < g.rows - 1){ 
        int i = 0;
        while(i < g.cols - 1 and up + i < g.rows - 1){
            int partial_count = 1;

            while(i < g.cols - 1 and up + i < g.rows - 1 and g.board[i+up][i] == g.board[i+up+1][i+1] and g.board[i+up][i] == next_player(g.current_player)){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            i += 1;
        }
        up += 1;
    }

    // Diagonal \ -
    up = 0;
    while(up < g.cols - 1){
        int i = g.rows - 1;
        int j = 0;
        while(0 < i and up + j < g.cols - 1){
            int partial_count = 1;

            while(0 < i and up + j < g.cols - 1 and g.board[i][up+j] == g.board[i-1][up+1+j] and g.board[i][up+j] == next_player(g.current_player)){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            i -= 1;
            j += 1;
        }
        up += 1;
    }
    down = 0;
    while(down < g.rows - 1){
        int i = g.rows - 1 - 1;
        int j = 0;
        while(0 < i - down and j < g.cols - 1){
            int partial_count = 1;

            while(0 < i - down and j < g.cols - 1 and g.board[i-down][j] == g.board[i-down-1][j+1] and g.board[i-down][j] == next_player(g.current_player)){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            i -= 1;
            j += 1;
        }
        down += 1;
    }

    result[0] = 0;
    result[1] = 0;
    return result;
}

// Devuelve un arreglo donde cada índice (i) indica la calidad de juegos formados de tamaño i
std::vector<int> quality_lines(Game &g, std::vector< std::vector<int> >& m){

    std::vector<int> result(g.c + 1, 0);

    // cuento lineas horizontales
    for (int i = 0; i < g.rows; i++) {
        for (int j = 0; j < g.cols - 1; j++) {
            int partial_count = 1;

            while (j + 1 < g.cols and g.board[i][j] == g.board[i][j+1] and g.board[i][j] == next_player(g.current_player)) {
                partial_count++;
                j++;
            }

            //Esto puede pasar, si tengo una cadena de X y una de Y separadas por un espacio
            // y pongo la ficha, puedo formar una de X+Y+1 que puede ser mayor que c
            partial_count = (partial_count > g.c)? g.c : partial_count;

            //result[partial_count]++;
            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1){
                result[partial_count] += partial_count * m[i][partial_count];
            }
                
            // Si es una cadena de uno y es mi ficha, sumo uno (esto lo hago unicamente en horizontal, luego no).
            if(partial_count == 1 && g.board[i][j] == next_player(g.current_player)) result[1] += m[i][1];
        }
    }


    // cuento lineas verticales
    for (int j = 0; j < g.cols; j++) {
        for (int i = 0; i < g.rows - 1; i++) {
            int partial_count = 1;

            while (i + 1 < g.rows and g.board[i][j] == g.board[i+1][j] and g.board[i][j] == next_player(g.current_player)) {
                partial_count++;
                i++;
            }

            partial_count = (partial_count > g.c)? g.c : partial_count;
            
            if(partial_count > 1){
                for(int k = i-1; k >= i - partial_count; k--){
                    result[partial_count] += m[k][j];
                }
            }

        }
    }

    // Diagonal /
    int down = 0;
    while(down < g.cols - 1){
        int i = 0;
        while(i < g.rows - 1 and down + i < g.cols - 1){
            int partial_count = 1;

            while(i < g.rows - 1 and down + i < g.cols - 1 and g.board[i][i+down] == g.board[i+1][i+down+1] and g.board[i][i+down] == next_player(g.current_player)){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;

            if(partial_count > 1){
                for(int k = 0; k < partial_count; k++){
                    result[partial_count] += m[i-k][i+down-k];
                } 
            }

            i += 1;
        }
        down += 1;
    }
    int up = 1;
    while(up < g.rows - 1){ 
        int i = 0;
        while(i < g.cols - 1 and up + i < g.rows - 1){
            int partial_count = 1;

            while(i < g.cols - 1 and up + i < g.rows - 1 and g.board[i+up][i] == g.board[i+up+1][i+1] and g.board[i+up][i] == next_player(g.current_player)){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            if(partial_count > 1){
                for(int k = 0; k < partial_count; k++){
                    result[partial_count] += m[i+up-k][i-k];
                } 
            }
            i += 1;
        }
        up += 1;
    }

    // Diagonal \ -
    up = 0;
    while(up < g.cols - 1){
        int i = g.rows - 1;
        int j = 0;
        while(0 < i and up + j < g.cols - 1){
            int partial_count = 1;

            while(0 < i and up + j < g.cols - 1 and g.board[i][up+j] == g.board[i-1][up+1+j] and g.board[i][up+j] == next_player(g.current_player)){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            if(partial_count > 1){
                for(int k = 0; k < partial_count; k++){
                    result[partial_count] += m[i+k][up+j-k];
                } 
            }
            i -= 1;
            j += 1;
        }
        up += 1;
    }
    down = 0;
    while(down < g.rows - 1){
        int i = g.rows - 1 - 1;
        int j = 0;
        while(0 < i - down and j < g.cols - 1){
            int partial_count = 1;

            while(0 < i - down and j < g.cols - 1 and g.board[i-down][j] == g.board[i-down-1][j+1] and g.board[i-down][j] == next_player(g.current_player)){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            if(partial_count > 1){
                for(int k = 0; k < partial_count; k++){
                    result[partial_count] += m[i-down+k][j-k];
                } 
            }
            i -= 1;
            j += 1;
        }
        down += 1;
    }

    result[0] = 0;
    result[1] = 0;
    return result;
}

// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game &g, int movement, std::vector<int>& p, std::vector<int>& q, std::vector<int>& r, std::vector< std::vector<int> >& m) {
    do_move(g, movement);
    std::vector<int> number_of_lines_of_length = lenght_lines(g);
    std::vector<int> quality_of_lines_of_length = quality_lines(g, m);

    int score = 0;

    // Por ahora esto solo se hace con las lineas 
    for(unsigned int i = 0; i < number_of_lines_of_length.size() - 1; i++){
        score += number_of_lines_of_length[i]*p[i];
    }

    for(unsigned int i = 0; i < number_of_lines_of_length.size() - 1; i++){
        score += quality_of_lines_of_length[i]*q[i];
    }

    if (number_of_lines_of_length[g.c] > 0){
        // Si esto pasa, si pongo acá, GANO => MAX SCORE
        score = INT_MAX;
    }

    undo_move(g, movement);
    return score;
}

// weights of lines length                                              --> p
// weights of lines quality                                             --> q
// weights of lines length associated with the number of played tiles   --> r
// matrix weight of (rows X lines length)                               --> m
// If g.rows = n, g.cols = m (m>=3) then it assumes params_size = 3*(cols-2) + rows*(cols-2).
// Example, n=6, m=5 => p:3, q:3, r:3, m:18, params size should be = {0,1,2,3,...,23,24,25,26}.
void assign_parameters(int rows, int cols, std::vector<int>& parameters, std::vector<int>& p, std::vector<int>& q, std::vector<int>& r, std::vector< std::vector<int> >& m){
    // Only with first basic parameter.
    // p = parameters;

    int global_index = 0;
    for (int i = 0; i < cols - 2; ++i) {
        p.push_back(parameters[global_index]);
        global_index++;
    }

    for (int i = 0; i < cols - 2; ++i) {
        q.push_back(parameters[global_index]);
        global_index++;
    }

    for (int i = 0; i < cols - 2; ++i) {
        r.push_back(parameters[global_index]);
        global_index++;
    }

    for (int i = 0; i < rows; ++i) {
        std::vector<int> row;
        for (int j = 0; j < cols - 2; ++j) {
            row.push_back(parameters[global_index]);
            global_index++;
        }
        m.push_back(row);
    }
}

struct movement_score { 
    int movement; 
    int score; 
    bool operator<(const movement_score& other){
        return this->score < other.score;
    }
}; 

int random_max(std::vector<movement_score>& valid_movements){
    std::sort(valid_movements.rbegin(), valid_movements.rend());
    std::vector<movement_score> best;
    best.push_back(valid_movements[0]);
    int i = 1;
    while(i < static_cast<int>(valid_movements.size()) and best[0].score == valid_movements[i].score){
        best.push_back(valid_movements[i]);
        i++;
    }
    int r = rand() + rand() + 1;
    // std::cerr << r << " " << best.size() << std::endl;
    return best[r % best.size()].movement;
}

int greedy_move(Game &g, std::vector<int>& parameters) {
    std::vector<int> p;
    std::vector<int> q;
    std::vector<int> r;
    std::vector< std::vector<int> > m;
    assign_parameters(g.rows, g.cols, parameters, p, q, r, m);

    std::vector<movement_score> valid_movements;

    for (int i = 0; i < g.cols; i++) {
        if (valid_move(g, i)) {
            movement_score mc;
            mc.movement = i;
            mc.score = calculate_move_score(g, i, p, q, r, m);
            valid_movements.push_back(mc);
        }
    }

    return random_max(valid_movements);
}
