#include "greedy.h"

int random_max(std::vector<movement_score>& valid_movements){

    srand(time(NULL)); // esto aca no esta bueno sirve para cuando juegan greedy vs greedy

    std::sort(valid_movements.rbegin(), valid_movements.rend());
    std::vector<movement_score> best;
    best.push_back(valid_movements[0]);
    int i = 1;
    while(i < static_cast<int>(valid_movements.size()) and best[0].score == valid_movements[i].score){
        best.push_back(valid_movements[i]);
        i++;
    }


    // // DEBUG
    // for(int i = 0; i < best.size(); ++i){
    //     std::cerr << best[i].movement << " " << best[i].score << " -- ";
    // }
    // std::cerr << std::endl;

    int r = rand() + rand() + 1;
    // std::cerr << r << " " << best.size() << std::endl;
    return best[r % best.size()].movement;
}

// weights of lines length                                              --> p       size == c-2
// weights of lines quality                                             --> q       size == c-2
// weights of lines length associated with the number of played tiles   --> r       size == c-2
// rival weights of lines length                                        --> s       size == c-3
// matrix weight of (rows X lines length)                               --> m       size == rows * (c-2)
// parameters.size() == 3*(c-2) + rows*(c-2).
void assign_parameters(int rows, int c, std::vector<int>& parameters, std::vector<int>& p, std::vector<int>& q, std::vector<int>& r, std::vector<int>& s, std::vector< std::vector<int> >& m){
    // Only with first basic parameter.
    // p = parameters;

    int global_index = 0;
    p.push_back(0); // para el indice 0, asi despues indexamos tranquilos
    p.push_back(0); // para el indice 1, asi despues indexamos tranquilos
    for (int i = 0; i < c - 2; ++i) {
        p.push_back(parameters[global_index]);
        global_index++;
    }
    p.push_back(c * INFINITE); // para el indice C, asi despues indexamos tranquilos

    q.push_back(0); // para el indice 0, asi despues indexamos tranquilos
    q.push_back(0); // para el indice 1, asi despues indexamos tranquilos
    for (int i = 0; i < c - 2; ++i) {
        q.push_back(parameters[global_index]);
        global_index++;
    }
    q.push_back(c * INFINITE); // para el indice C, asi despues indexamos tranquilos

    r.push_back(0); // para el indice 0, asi despues indexamos tranquilos
    r.push_back(0); // para el indice 1, asi despues indexamos tranquilos
    for (int i = 0; i < c - 2; ++i) {
        r.push_back(parameters[global_index]);
        global_index++;
    }
    r.push_back(c * INFINITE); // para el indice C, asi despues indexamos tranquilos

    s.push_back(0); // para el indice 0, asi despues indexamos tranquilos
    s.push_back(0); // para el indice 1, asi despues indexamos tranquilos
    for (int i = 0; i < c - 3; ++i) {
        s.push_back(parameters[global_index]);
        global_index++;
    }
    s.push_back(INFINITE); // para el indice C-1, asi despues indexamos tranquilos
    s.push_back(INFINITE); // para el indice C, asi despues indexamos tranquilos

    for (int i = 0; i < rows; ++i) {
        std::vector<int> row;
        row.push_back(0); // para el indice 0, asi despues indexamos tranquilos
        row.push_back(0); // para el indice 1, asi despues indexamos tranquilos
        for (int j = 0; j < c - 2; ++j) {
            row.push_back(parameters[global_index]);
            global_index++;
        }
        row.push_back(INFINITE); // para el indice C, asi despues indexamos tranquilos
        m.push_back(row);
    }
}

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
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i-k][j];
                    result[partial_count] += m[i-k][partial_count];
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
                    // result[partial_count] += m[i-k][i+down-k];
                    result[partial_count] += m[i-k][partial_count];
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
            if(partial_count > 1){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i+up-k][i-k];
                    result[partial_count] += m[i+up-k][partial_count];
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
            if(partial_count > 1){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i+k][up+j-k];
                    result[partial_count] += m[i+k][partial_count];
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
            if(partial_count > 1){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i-down+k][j-k];
                    result[partial_count] += m[i-down+k][partial_count];
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

// Devuelve un arreglo donde cada índice (i) indica la cantidad de juegos formados de tamaño i que tienen chances de extenderse
std::vector<int> effective_lenght_lines(Game &g, char player){

    std::vector<int> result(g.c + 1, 0);

    // cuento lineas horizontales
    for (int i = 0; i < g.rows; i++) {
        for (int j = 0; j < g.cols - 1; j++) {
            int partial_count = 1;

            while (j + 1 < g.cols and g.board[i][j] == g.board[i][j+1] and g.board[i][j] == player) {
                partial_count++;
                j++;
            }

            //Esto puede pasar, si tengo una cadena de X y una de Y separadas por un espacio
            // y pongo la ficha, puedo formar una de X+Y+1 que puede ser mayor que c
            partial_count = (partial_count > g.c)? g.c : partial_count;

            //result[partial_count]++;
            // Si es una cadena mayor a 1 y se puede continuar, la sumo.
            if(partial_count > 1 and ( (j+1 < g.cols and g.board[i][j+1] == EMPTY) or (0 <= j-partial_count and g.board[i][j-partial_count] == EMPTY) ) )
                result[partial_count]++;
                
            // Si es una cadena de uno y es mi ficha, sumo uno (esto lo hago unicamente en horizontal, luego no).
            if(partial_count == 1 and g.board[i][j] == player and ((j+1 < g.cols and g.board[i][j+1] == EMPTY) or (0 < j and g.board[i][j-1] == EMPTY)))
                result[1]++;
        }
    }


    // cuento lineas verticales
    for (int j = 0; j < g.cols; j++) {
        for (int i = 0; i < g.rows - 1; i++) {
            int partial_count = 1;

            while (i + 1 < g.rows and g.board[i][j] == g.board[i+1][j] and g.board[i][j] == player) {
                partial_count++;
                i++;
            }

            partial_count = (partial_count > g.c)? g.c : partial_count;
            
            //result[partial_count]++;
            // Si es una cadena mayor a 1 y se puede continuar, la sumo.
            if(partial_count > 1 and ( (i+1 < g.rows and g.board[i+1][j] == EMPTY) or (0 <= i-partial_count and g.board[i-partial_count][j] == EMPTY) ) )
                result[partial_count]++;

        }
    }

    // Diagonal /
    int down = 0;
    while(down < g.cols - 1){
        int i = 0;
        while(i < g.rows - 1 and down + i < g.cols - 1){
            int partial_count = 1;

            while(i < g.rows - 1 and down + i < g.cols - 1 and g.board[i][i+down] == g.board[i+1][i+down+1] and g.board[i][i+down] == player){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (i < g.rows-1 and down+i < g.cols-1 and g.board[i+1][i+down+1] == EMPTY) or (0 <= i-partial_count and 0 <= down+i-partial_count and g.board[i-partial_count][down+i-partial_count] == EMPTY) ) )
                result[partial_count]++;

            i += 1;
        }
        down += 1;
    }
    int up = 1;
    while(up < g.rows - 1){ 
        int i = 0;
        while(i < g.cols - 1 and up + i < g.rows - 1){
            int partial_count = 1;

            while(i < g.cols - 1 and up + i < g.rows - 1 and g.board[i+up][i] == g.board[i+up+1][i+1] and g.board[i+up][i] == player){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (i < g.cols-1 and up+i < g.rows-1 and g.board[i+up+1][i+1] == EMPTY) or (0 <= i+up-partial_count and 0 <= i-partial_count and g.board[i+up-partial_count][i-partial_count] == EMPTY) ) )
                result[partial_count]++;
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

            while(0 < i and up + j < g.cols - 1 and g.board[i][up+j] == g.board[i-1][up+1+j] and g.board[i][up+j] == player){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (0 < i and up+j < g.cols-1 and g.board[i-1][up+1+j] == EMPTY) or (i+partial_count < g.rows and 0 <= up+j-partial_count and g.board[i+partial_count][up+j-partial_count] == EMPTY) ) )
                result[partial_count]++;
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

            while(0 < i - down and j < g.cols - 1 and g.board[i-down][j] == g.board[i-down-1][j+1] and g.board[i-down][j] == player){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (0 < i-down and j < g.cols-1 and g.board[i-down-1][j+1] == EMPTY) or (i-down+partial_count < g.rows and 0 <= j-partial_count and g.board[i-down+partial_count][j-partial_count] == EMPTY) ) )
                result[partial_count]++;
            i -= 1;
            j += 1;
        }
        down += 1;
    }

    result[0] = 0;
    result[1] = 0;
    return result;
}

// Devuelve un arreglo donde cada índice (i) indica la calidad de juegos formados de tamaño i que tienen chances de extenderse
std::vector<int> effective_quality_lines(Game &g, char player, std::vector< std::vector<int> >& m){
    std::vector<int> result(g.c + 1, 0);

    // cuento lineas horizontales
    for (int i = 0; i < g.rows; i++) {
        for (int j = 0; j < g.cols - 1; j++) {
            int partial_count = 1;

            while (j + 1 < g.cols and g.board[i][j] == g.board[i][j+1] and g.board[i][j] == player) {
                partial_count++;
                j++;
            }

            //Esto puede pasar, si tengo una cadena de X y una de Y separadas por un espacio
            // y pongo la ficha, puedo formar una de X+Y+1 que puede ser mayor que c
            partial_count = (partial_count > g.c)? g.c : partial_count;

            //result[partial_count]++;
            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1 and ( (j+1 < g.cols and g.board[i][j+1] == EMPTY) or (0 <= j-partial_count and g.board[i][j-partial_count] == EMPTY) ) ){
                result[partial_count] += partial_count * m[i][partial_count];
            }
                
            // Si es una cadena de uno y es mi ficha, sumo uno (esto lo hago unicamente en horizontal, luego no).
            if(partial_count == 1 and g.board[i][j] == player and ((j+1 < g.cols and g.board[i][j+1] == EMPTY) or (0 < j and g.board[i][j-1] == EMPTY)))
                result[1] += m[i][1];
        }
    }


    // cuento lineas verticales
    for (int j = 0; j < g.cols; j++) {
        for (int i = 0; i < g.rows - 1; i++) {
            int partial_count = 1;

            while (i + 1 < g.rows and g.board[i][j] == g.board[i+1][j] and g.board[i][j] == player) {
                partial_count++;
                i++;
            }

            partial_count = (partial_count > g.c)? g.c : partial_count;
            
            if(partial_count > 1 and ( (i+1 < g.rows and g.board[i+1][j] == EMPTY) or (0 <= i-partial_count and g.board[i-partial_count][j] == EMPTY) ) ){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i-k][j];
                    result[partial_count] += m[i-k][partial_count];
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

            while(i < g.rows - 1 and down + i < g.cols - 1 and g.board[i][i+down] == g.board[i+1][i+down+1] and g.board[i][i+down] == player){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (i < g.rows-1 and down+i < g.cols-1 and g.board[i+1][i+down+1] == EMPTY) or (0 <= i-partial_count and 0 <= down+i-partial_count and g.board[i-partial_count][down+i-partial_count] == EMPTY) ) ){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i-k][i+down-k];
                    result[partial_count] += m[i-k][partial_count];
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

            while(i < g.cols - 1 and up + i < g.rows - 1 and g.board[i+up][i] == g.board[i+up+1][i+1] and g.board[i+up][i] == player){
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (i < g.cols-1 and up+i < g.rows-1 and g.board[i+up+1][i+1] == EMPTY) or (0 <= i+up-partial_count and 0 <= i-partial_count and g.board[i+up-partial_count][i-partial_count] == EMPTY) ) ){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i+up-k][i-k];
                    result[partial_count] += m[i+up-k][partial_count];
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

            while(0 < i and up + j < g.cols - 1 and g.board[i][up+j] == g.board[i-1][up+1+j] and g.board[i][up+j] == player){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (0 < i and up+j < g.cols-1 and g.board[i-1][up+1+j] == EMPTY) or (i+partial_count < g.rows and 0 <= up+j-partial_count and g.board[i+partial_count][up+j-partial_count] == EMPTY) ) ){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i+k][up+j-k];
                    result[partial_count] += m[i+k][partial_count];
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

            while(0 < i - down and j < g.cols - 1 and g.board[i-down][j] == g.board[i-down-1][j+1] and g.board[i-down][j] == player){
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1 and ( (0 < i-down and j < g.cols-1 and g.board[i-down-1][j+1] == EMPTY) or (i-down+partial_count < g.rows and 0 <= j-partial_count and g.board[i-down+partial_count][j-partial_count] == EMPTY) ) ){
                for(int k = 0; k < partial_count; k++){
                    // result[partial_count] += m[i-down+k][j-k];
                    result[partial_count] += m[i-down+k][partial_count];
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

struct line_info {
    int start;
    int finish;
    int start_i;
    int start_j;
    int finish_i;
    int finish_j;
    int size;
};

// Devuelve un arreglo donde cada índice (i) indica la cantidad de juegos formados de tamaño i
std::vector<int> possible_lenght_lines(Game &g, char player){

    std::vector<int> result(g.c + 1, 0);
    std::vector<int> result_possible_lines(g.c + 1, 0);

    // cuento lineas horizontales
    for (int i = 0; i < g.rows; i++) {
        std::vector<line_info> lines_in_row;
        for (int j = 0; j < g.cols - 1; j++) {
            int partial_count = 1;
            line_info li;
            li.start = j;
            li.finish = j;

            while (j + 1 < g.cols and g.board[i][j] == g.board[i][j+1] and g.board[i][j] == player) {
                li.finish++;
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
            if(partial_count == 1 && g.board[i][j] == player) result[1]++;

            // Agrego la linea solo si es me pertenece:
            if(g.board[i][li.start] == player) lines_in_row.push_back(li);

        }
        // termino la fila, hago el recuento de posibles lineas:
        for(int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq de k
                if(0 <= lines_in_row[k].start-1 and g.board[i][lines_in_row[k].start-1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k].start-1] != EMPTY))){
                    int l_1_size = lines_in_row[k].finish - lines_in_row[k].start + 1;
                    int l_2_size = 1;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    result_possible_lines[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish + 1 == lines_in_row[k+1].start - 1 and g.board[i][lines_in_row[k].finish+1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k].finish+1] != EMPTY))){
                    int l_1_size = lines_in_row[k].finish - lines_in_row[k].start + 1;
                    int l_2_size = lines_in_row[k+1].finish - lines_in_row[k+1].start + 1;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    result_possible_lines[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha de k y a la izq de k+1
                    if(lines_in_row[k].finish+1 < g.cols and g.board[i][lines_in_row[k].finish+1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k].finish+1] != EMPTY))){
                        int l_1_size = lines_in_row[k].finish - lines_in_row[k].start + 1;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }                    
                    if(0 <= lines_in_row[k+1].start-1 and g.board[i][lines_in_row[k+1].start-1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k+1].start-1] != EMPTY))){
                        int l_1_size = lines_in_row[k+1].finish - lines_in_row[k+1].start + 1;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha
                if(lines_in_row[k].finish+1 < g.cols and g.board[i][lines_in_row[k].finish+1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k].finish+1] != EMPTY))){
                    int l_1_size = lines_in_row[k].finish - lines_in_row[k].start + 1;
                    int l_2_size = 1;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    result_possible_lines[val]++;
                }

            }
        }
    }


    // cuento lineas verticales
    for (int j = 0; j < g.cols; j++) {
        std::vector<line_info> lines_in_row;
        for (int i = 0; i < g.rows - 1; i++) {
            int partial_count = 1;
            line_info li;
            li.start = i;
            li.finish = i;

            while (i + 1 < g.rows and g.board[i][j] == g.board[i+1][j] and g.board[i][j] == player) {
                li.finish++;
                partial_count++;
                i++;
            }

            partial_count = (partial_count > g.c)? g.c : partial_count;
            
            //result[partial_count]++;
            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1) result[partial_count]++;

            // Agrego la linea solo si es me pertenece:
            if(g.board[li.start][j] == player) lines_in_row.push_back(li);

        }
        // termino la columna, hago el recuento de posibles lineas,
        // solo la ultima linea tiene chances de ser continuada:
        if(0 < lines_in_row.size() and lines_in_row[lines_in_row.size()-1].finish+1 < g.rows and g.board[lines_in_row[lines_in_row.size()-1].finish+1][j] == EMPTY){
            int l_1_size = lines_in_row[lines_in_row.size()-1].finish - lines_in_row[lines_in_row.size()-1].start + 1;
            int l_2_size = 1;
            int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
            result_possible_lines[val]++;
        }
    }

    // Diagonal /
    int down = 0;
    while(down < g.cols - 1){
        std::vector<line_info> lines_in_row;
        int i = 0;
        while(i < g.rows - 1 and down + i < g.cols - 1){
            int partial_count = 1;
            line_info li;
            li.start_i = i;
            li.start_j = i+down;
            li.finish_i = i;
            li.finish_j = i+down;
            li.size = partial_count;

            while(i < g.rows - 1 and down + i < g.cols - 1 and g.board[i][i+down] == g.board[i+1][i+down+1] and g.board[i][i+down] == player){
                li.finish_i++;
                li.finish_j++;
                li.size++;
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;

            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i += 1;
        }
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq abajo
                if(0 <= lines_in_row[k].start_i-1 and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i-1][lines_in_row[k].start_j-1] == EMPTY and (lines_in_row[k].start_i-1==0 or (g.board[lines_in_row[k].start_i-1-1][lines_in_row[k].start_j-1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i + 1 == lines_in_row[k+1].start_i - 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    result_possible_lines[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha arriba de k y a la izq abajo de k+1
                    if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }                    
                    if(0 <= lines_in_row[k+1].start_i-1 and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i-1][lines_in_row[k+1].start_j-1] == EMPTY and (lines_in_row[k+1].start_i-1==0 or (g.board[lines_in_row[k+1].start_i-1-1][lines_in_row[k+1].start_j-1] != EMPTY))){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha
                if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }

            }
        }
        down += 1;
    }
    int up = 1;
    while(up < g.rows - 1){ 
        std::vector<line_info> lines_in_row;
        int i = 0;
        while(i < g.cols - 1 and up + i < g.rows - 1){
            int partial_count = 1;
            line_info li;
            li.start_i = i+up;
            li.start_j = i;
            li.finish_i = i+up;
            li.finish_j = i;
            li.size = partial_count;

            while(i < g.cols - 1 and up + i < g.rows - 1 and g.board[i+up][i] == g.board[i+up+1][i+1] and g.board[i+up][i] == player){
                li.finish_i++;
                li.finish_j++;
                li.size++;
                partial_count += 1;
                i += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i += 1;
        }
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq abajo
                if(0 <= lines_in_row[k].start_i-1 and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i-1][lines_in_row[k].start_j-1] == EMPTY and (lines_in_row[k].start_i-1==0 or (g.board[lines_in_row[k].start_i-1-1][lines_in_row[k].start_j-1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i + 1 == lines_in_row[k+1].start_i - 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    result_possible_lines[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha arriba de k y a la izq abajo de k+1
                    if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }                    
                    if(0 <= lines_in_row[k+1].start_i-1 and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i-1][lines_in_row[k+1].start_j-1] == EMPTY and (lines_in_row[k+1].start_i-1==0 or (g.board[lines_in_row[k+1].start_i-1-1][lines_in_row[k+1].start_j-1] != EMPTY))){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha
                if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }

            }
        }
        up += 1;
    }

    // Diagonal \ -
    up = 0;
    while(up < g.cols - 1){
        std::vector<line_info> lines_in_row;
        int i = g.rows - 1;
        int j = 0;
        while(0 < i and up + j < g.cols - 1){
            int partial_count = 1;
            line_info li;
            li.start_i = i;
            li.start_j = up+j;
            li.finish_i = i;
            li.finish_j = up+j;
            li.size = partial_count;

            while(0 < i and up + j < g.cols - 1 and g.board[i][up+j] == g.board[i-1][up+1+j] and g.board[i][up+j] == player){
                li.finish_i--;
                li.finish_j++;
                li.size++;
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i -= 1;
            j += 1;
        }
        up += 1;
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq arriba
                if(lines_in_row[k].start_i+1 < g.rows and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i+1][lines_in_row[k].start_j-1] == EMPTY and g.board[lines_in_row[k].start_i][lines_in_row[k].start_j-1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i - 1 == lines_in_row[k+1].start_i + 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    result_possible_lines[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha abajo de k y a la izq arriba de k+1
                    if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }                    
                    if(lines_in_row[k+1].start_i+1 < g.rows and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i+1][lines_in_row[k+1].start_j-1] == EMPTY and g.board[lines_in_row[k+1].start_i][lines_in_row[k+1].start_j-1] != EMPTY){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha abajo
                if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }

            }
        }
    }
    down = 0;
    while(down < g.rows - 1){
        std::vector<line_info> lines_in_row;
        int i = g.rows - 1 - 1;
        int j = 0;
        while(0 < i - down and j < g.cols - 1){
            int partial_count = 1;
            line_info li;
            li.start_i = i-down;
            li.start_j = j;
            li.finish_i = i-down;
            li.finish_j = j;
            li.size = partial_count;

            while(0 < i - down and j < g.cols - 1 and g.board[i-down][j] == g.board[i-down-1][j+1] and g.board[i-down][j] == player){
                li.finish_i--;
                li.finish_j++;
                li.size++;
                partial_count += 1;
                i -= 1;
                j += 1;
            }
            partial_count = (partial_count > g.c)? g.c : partial_count;
            if(partial_count > 1) result[partial_count]++;
            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i -= 1;
            j += 1;
        }
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq arriba
                if(lines_in_row[k].start_i+1 < g.rows and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i+1][lines_in_row[k].start_j-1] == EMPTY and g.board[lines_in_row[k].start_i][lines_in_row[k].start_j-1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i - 1 == lines_in_row[k+1].start_i + 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    result_possible_lines[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha abajo de k y a la izq arriba de k+1
                    if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }                    
                    if(lines_in_row[k+1].start_i+1 < g.rows and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i+1][lines_in_row[k+1].start_j-1] == EMPTY and g.board[lines_in_row[k+1].start_i][lines_in_row[k+1].start_j-1] != EMPTY){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        result_possible_lines[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha abajo
                if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    result_possible_lines[val]++;
                }

            }
        }
        down += 1;
    }

    result[0] = 0;
    result[1] = 0;
    // return result;
    return result_possible_lines;
}



// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game &g, char player, int movement, std::vector<int>& p, std::vector<int>& q, std::vector<int>& r, std::vector<int>& s, std::vector< std::vector<int> >& m) {
    do_move(g, movement);

    std::vector<int> number_of_lines_of_length = effective_lenght_lines(g, player);
    std::vector<int> number_of_possible_lines_of_length = possible_lenght_lines(g, player);
    std::vector<int> quality_of_lines_of_length = effective_quality_lines(g, player, m);


    std::vector<int> rival_number_of_lines_of_length = effective_lenght_lines(g, next_player(player));
    std::vector<int> rival_number_of_possible_lines_of_length = possible_lenght_lines(g, next_player(player));
    std::vector<int> rival_quality_of_lines_of_length = effective_quality_lines(g, next_player(player), m);

    int score = 0;

    for(unsigned int i = 0; i < number_of_lines_of_length.size(); i++){
        score += number_of_lines_of_length[i] * p[i];
    }

    for(unsigned int i = 0; i < quality_of_lines_of_length.size(); i++){
        score += quality_of_lines_of_length[i] * q[i];
    }

    // desfasado porque yo ya jugue la ficha en este turno
    for(unsigned int i = 1; i < number_of_possible_lines_of_length.size(); i++){
        score += number_of_possible_lines_of_length[i] * p[i-1];
    }


    // rival
    for(unsigned int i = 0; i < rival_number_of_lines_of_length.size(); i++){
        score -= rival_number_of_lines_of_length[i] * s[i];
    }
    for(unsigned int i = 0; i < rival_number_of_possible_lines_of_length.size(); i++){
        score -= rival_number_of_possible_lines_of_length[i] * s[i];
    }
    for(unsigned int i = 0; i < rival_quality_of_lines_of_length.size(); i++){
        score -= rival_quality_of_lines_of_length[i] * q[i];
    }

    undo_move(g, movement);
    return score;
}


int greedy_move(Game &g, std::vector<int>& parameters) {
    std::vector<int> p;
    std::vector<int> q;
    std::vector<int> r;
    std::vector<int> s;
    std::vector< std::vector<int> > m;
    assign_parameters(g.rows, g.c, parameters, p, q, r, s, m);

    std::vector<movement_score> valid_movements;
    for (int i = 0; i < g.cols; i++) {
        if (valid_move(g, i)) {
            movement_score mc;
            mc.movement = i;
            // mi ganancia de poner la ficha en la col 'i' menos la ganancia de mi rival dado que pongo la ficha en la col 'i'
            // mc.score = calculate_move_score(g, g.current_player, i, p, q, r, m) - calculate_move_score(g, next_player(g.current_player), i, s, q, r, m);
            mc.score = calculate_move_score(g, g.current_player, i, p, q, r, s, m);
            std::cerr << "mov: " << mc.movement << " score: " << mc.score << std::endl;
            valid_movements.push_back(mc);
        }
    }

    return random_max(valid_movements);


    // // DEBUG LINES
    // int mov = random_max(valid_movements);
    // do_move(g, mov);
    // std::vector<int> quality_of_lines_of_length = effective_quality_lines(g, next_player(g.current_player), m);
    // std::vector<int> number_of_lines_of_length = effective_lenght_lines(g, next_player(g.current_player));
    // std::vector<int> number_of_possible_lines_of_length = possible_lenght_lines(g, next_player(g.current_player));
    // std::cerr << std::endl;
    // for(int i = 0; i < m.size(); ++i){
    //     for(int j = 0; j < m[i].size(); ++j){
    //         std::cerr << m[m.size()-i-1][j] << " ";
    //     }
    //     std::cerr << std::endl;
    // }
    // std::cerr << std::endl;
    // for(int i = 0; i < number_of_lines_of_length.size(); ++i){
    //     std::cerr << number_of_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;
    // for(int i = 0; i < quality_of_lines_of_length.size(); ++i){
    //     std::cerr << quality_of_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;
    // for(int i = 0; i < number_of_possible_lines_of_length.size(); ++i){
    //     std::cerr << number_of_possible_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;
    // undo_move(g, mov);
    // return mov;
}
