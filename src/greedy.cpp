#include "greedy.h"

// weights of lines length                                              --> p       size == c-2
// weights of possible lines length                                     --> q       size == c-1
// rival weights of lines length                                        --> s       size == c-3
// rival weights of possible lines length                               --> t       size == c-3
// parameters.size() == (c-1) + (c-2) + 2*(c-3)
void assign_parameters(int c, std::vector<int>& parameters, std::vector<int>& p, std::vector<int>& q, std::vector<int>& s, std::vector<int>& t){
    assert(parameters.size() == (c-2) + (c-1) + (c-3)*2);

    int global_index = 0;
    p.push_back(0); // peso para el indice 0, asi despues indexamos tranquilos
    p.push_back(0); // peso para el indice 1
    for (int i = 0; i < c - 2; ++i) {
        p.push_back(parameters[global_index]);
        global_index++;
    }
    p.push_back(0); // peso para el indice C

    q.push_back(0);
    q.push_back(0);
    for (int i = 0; i < c - 1; ++i) {
        q.push_back(parameters[global_index]);
        global_index++;
    }

    s.push_back(0); // peso para el indice 0
    s.push_back(0); // peso para el indice 1
    for (int i = 0; i < c - 3; ++i) {
        s.push_back(parameters[global_index]);
        global_index++;
    }
    s.push_back(0); // peso para el indice C-1
    s.push_back(0); // peso para el indice C

    t.push_back(0); // peso para el indice 0
    t.push_back(0); // peso para el indice 1
    for (int i = 0; i < c - 3; ++i) {
        t.push_back(parameters[global_index]);
        global_index++;
    }
    t.push_back(0); // peso para el indice C-1
    t.push_back(0); // peso para el indice C
}

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
    return best[r % best.size()].movement;
}

int center_max(std::vector<movement_score>& valid_movements, int center){
    std::sort(valid_movements.rbegin(), valid_movements.rend());
    std::vector<movement_score> best;
    best.push_back(valid_movements[0]);
    int i = 1;
    while(i < static_cast<int>(valid_movements.size()) and best[0].score == valid_movements[i].score){
        best.push_back(valid_movements[i]);
        i++;
    }
    movement_score min_vs_center = best[0];
    for(unsigned int i = 1; i < best.size(); ++i){
        if(abs(best[i].movement - center) < abs(min_vs_center.movement - center)){
            min_vs_center = best[i];
        }
    }
    
    return min_vs_center.movement;
}

void calculate_game_info(Game &g, char player, std::vector<int>& lines_of_length, std::vector<int>& effective_lines_of_length, std::vector<int>& possible_lines_of_length){

    lines_of_length = std::vector<int>(g.c + 1, 0);
    effective_lines_of_length = std::vector<int>(g.c + 1, 0);
    possible_lines_of_length = std::vector<int>(g.c + 1, 0);

    // Cuento lineas horizontales
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

            // Si es una cadena mayor a 1, la sumo.
            if(partial_count > 1) lines_of_length[partial_count]++;
                
            // Si es una cadena de uno y es mi ficha, sumo uno (esto lo hago unicamente en horizontal, luego no).
            if((partial_count == 1 and g.board[i][j] == player) or (j+1 < g.cols and g.board[i][j+1] == player)) 
                lines_of_length[1]++;                
                
            if(partial_count > 1 and ((j+1 < g.cols and g.board[i][j+1] == EMPTY) or (0 <= j-partial_count and g.board[i][j-partial_count] == EMPTY))){
                effective_lines_of_length[partial_count]++;
            }

            if(partial_count == 1 and g.board[i][j] == player and ((j+1 < g.cols and g.board[i][j+1] == EMPTY) or (0 < j and g.board[i][j-1] == EMPTY))){
                effective_lines_of_length[1]++;
            }

            // Agrego la linea solo si me pertenece:
            if(g.board[i][li.start] == player) lines_in_row.push_back(li);

            // Este es un caso muy borde: si estoy en la ultima iteracion:
            if(j+1 == g.cols-1 and g.board[i][j+1] == player){
                li.start = j+1;
                li.finish = j+1;
                lines_in_row.push_back(li);            
            }

        }
        // termino la fila, hago el recuento de posibles lineas:
        for(unsigned int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq de k
                if(0 <= lines_in_row[k].start-1 and g.board[i][lines_in_row[k].start-1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k].start-1] != EMPTY))){
                    int l_1_size = lines_in_row[k].finish - lines_in_row[k].start + 1;
                    int l_2_size = 1;
                    int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                    possible_lines_of_length[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish + 1 == lines_in_row[k+1].start - 1 and g.board[i][lines_in_row[k].finish+1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k].finish+1] != EMPTY))){
                    int l_1_size = lines_in_row[k].finish - lines_in_row[k].start + 1;
                    int l_2_size = lines_in_row[k+1].finish - lines_in_row[k+1].start + 1;
                    int val = (l_1_size + l_2_size + 1 > g.c) ? g.c : l_1_size + l_2_size + 1;
                    possible_lines_of_length[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha de k y a la izq de k+1
                    if(lines_in_row[k].finish+1 < g.cols and g.board[i][lines_in_row[k].finish+1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k].finish+1] != EMPTY))){
                        int l_1_size = lines_in_row[k].finish - lines_in_row[k].start + 1;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }                    
                    if(0 <= lines_in_row[k+1].start-1 and g.board[i][lines_in_row[k+1].start-1] == EMPTY and (i==0 or (g.board[i-1][lines_in_row[k+1].start-1] != EMPTY))){
                        int l_1_size = lines_in_row[k+1].finish - lines_in_row[k+1].start + 1;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
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
                    possible_lines_of_length[val]++;
                }

            }
        }
    }

    // Cuento lineas verticales
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
            
            if(partial_count > 1) lines_of_length[partial_count]++;                
            if(partial_count > 1 and i+1 < g.rows and g.board[i+1][j] == EMPTY) effective_lines_of_length[partial_count]++;
            if(g.board[li.start][j] == player) lines_in_row.push_back(li);

        }
        // termino la columna, hago el recuento de posibles lineas,
        // solo la ultima linea tiene chances de ser continuada:
        if(0 < lines_in_row.size() and lines_in_row[lines_in_row.size()-1].finish+1 < g.rows and g.board[lines_in_row[lines_in_row.size()-1].finish+1][j] == EMPTY){
            int l_1_size = lines_in_row[lines_in_row.size()-1].finish - lines_in_row[lines_in_row.size()-1].start + 1;
            int l_2_size = 1;
            int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
            possible_lines_of_length[val]++;
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

            if(partial_count > 1) lines_of_length[partial_count]++;                
            if(partial_count > 1 and ((i < g.rows-1 and down+i < g.cols-1 and g.board[i+1][i+down+1] == EMPTY) or (0 <= i-partial_count and 0 <= down+i-partial_count and g.board[i-partial_count][down+i-partial_count] == EMPTY))) effective_lines_of_length[partial_count]++;
            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i += 1;
        }
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(unsigned int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq abajo
                if(0 <= lines_in_row[k].start_i-1 and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i-1][lines_in_row[k].start_j-1] == EMPTY and (lines_in_row[k].start_i-1==0 or (g.board[lines_in_row[k].start_i-1-1][lines_in_row[k].start_j-1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i + 1 == lines_in_row[k+1].start_i - 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size + 1 > g.c) ? g.c : l_1_size + l_2_size + 1;
                    possible_lines_of_length[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha arriba de k y a la izq abajo de k+1
                    if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }                    
                    if(0 <= lines_in_row[k+1].start_i-1 and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i-1][lines_in_row[k+1].start_j-1] == EMPTY and (lines_in_row[k+1].start_i-1==0 or (g.board[lines_in_row[k+1].start_i-1-1][lines_in_row[k+1].start_j-1] != EMPTY))){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha
                if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
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
            if(partial_count > 1) lines_of_length[partial_count]++;
            if(partial_count > 1 and ((i < g.cols-1 and up+i < g.rows-1 and g.board[i+up+1][i+1] == EMPTY) or (0 <= i+up-partial_count and 0 <= i-partial_count and g.board[i+up-partial_count][i-partial_count] == EMPTY))) effective_lines_of_length[partial_count]++;
            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i += 1;
        }
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(unsigned int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq abajo
                if(0 <= lines_in_row[k].start_i-1 and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i-1][lines_in_row[k].start_j-1] == EMPTY and (lines_in_row[k].start_i-1==0 or (g.board[lines_in_row[k].start_i-1-1][lines_in_row[k].start_j-1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i + 1 == lines_in_row[k+1].start_i - 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size + 1 > g.c) ? g.c : l_1_size + l_2_size + 1;
                    possible_lines_of_length[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha arriba de k y a la izq abajo de k+1
                    if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }                    
                    if(0 <= lines_in_row[k+1].start_i-1 and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i-1][lines_in_row[k+1].start_j-1] == EMPTY and (lines_in_row[k+1].start_i-1==0 or (g.board[lines_in_row[k+1].start_i-1-1][lines_in_row[k+1].start_j-1] != EMPTY))){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha
                if(lines_in_row[k].finish_i+1 < g.rows and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i+1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i][lines_in_row[k].finish_j+1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
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
            if(partial_count > 1) lines_of_length[partial_count]++;
            if(partial_count > 1 and ((0 < i and up+j < g.cols-1 and g.board[i-1][up+1+j] == EMPTY) or (i+partial_count < g.rows and 0 <= up+j-partial_count and g.board[i+partial_count][up+j-partial_count] == EMPTY))) effective_lines_of_length[partial_count]++;
            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i -= 1;
            j += 1;
        }
        up += 1;
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(unsigned int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq arriba
                if(lines_in_row[k].start_i+1 < g.rows and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i+1][lines_in_row[k].start_j-1] == EMPTY and g.board[lines_in_row[k].start_i][lines_in_row[k].start_j-1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i - 1 == lines_in_row[k+1].start_i + 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size + 1 > g.c) ? g.c : l_1_size + l_2_size + 1;
                    possible_lines_of_length[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha abajo de k y a la izq arriba de k+1
                    if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }                    
                    if(lines_in_row[k+1].start_i+1 < g.rows and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i+1][lines_in_row[k+1].start_j-1] == EMPTY and g.board[lines_in_row[k+1].start_i][lines_in_row[k+1].start_j-1] != EMPTY){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha abajo
                if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
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
            if(partial_count > 1) lines_of_length[partial_count]++;
            if(partial_count > 1 and ((0 < i-down and j < g.cols-1 and g.board[i-down-1][j+1] == EMPTY) or (i-down+partial_count < g.rows and 0 <= j-partial_count and g.board[i-down+partial_count][j-partial_count] == EMPTY))) effective_lines_of_length[partial_count]++;
            if(g.board[li.start_i][li.start_j] == player) lines_in_row.push_back(li);
            i -= 1;
            j += 1;
        }
        // termino la primer pasada, hago el recuento de posibles lineas:
        for(unsigned int k = 0; k < lines_in_row.size(); k++){
            // Separo el primer caso
            if(k == 0){
                // Me fijo a la izq arriba
                if(lines_in_row[k].start_i+1 < g.rows and 0 <= lines_in_row[k].start_j-1 and g.board[lines_in_row[k].start_i+1][lines_in_row[k].start_j-1] == EMPTY and g.board[lines_in_row[k].start_i][lines_in_row[k].start_j-1] != EMPTY){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
                }
            }
            // puedo agarrar de a pares
            if(k+1 < lines_in_row.size()){
                if(lines_in_row[k].finish_i - 1 == lines_in_row[k+1].start_i + 1 and lines_in_row[k].finish_j + 1 == lines_in_row[k+1].start_j - 1 and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY){
                    int l_1_size = lines_in_row[k].size;
                    int l_2_size = lines_in_row[k+1].size;
                    int val = (l_1_size + l_2_size + 1 > g.c) ? g.c : l_1_size + l_2_size + 1;
                    possible_lines_of_length[val]++;
                } else {
                    // si no los puedo armar juntos, me fijo a la derecha abajo de k y a la izq arriba de k+1
                    if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                        int l_1_size = lines_in_row[k].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }                    
                    if(lines_in_row[k+1].start_i+1 < g.rows and 0 <= lines_in_row[k+1].start_j-1 and g.board[lines_in_row[k+1].start_i+1][lines_in_row[k+1].start_j-1] == EMPTY and g.board[lines_in_row[k+1].start_i][lines_in_row[k+1].start_j-1] != EMPTY){
                        int l_1_size = lines_in_row[k+1].size;
                        int l_2_size = 1;
                        int val = (l_1_size + l_2_size > g.c) ? g.c : l_1_size + l_2_size;
                        possible_lines_of_length[val]++;
                    }
                }
            } 
            // me queda la ultima:
            else {
                // me fijo a la derecha abajo
                if(0 <= lines_in_row[k].finish_i-1 and lines_in_row[k].finish_j+1 < g.cols and g.board[lines_in_row[k].finish_i-1][lines_in_row[k].finish_j+1] == EMPTY and (lines_in_row[k].finish_i-1 == 0 or (g.board[lines_in_row[k].finish_i-1-1][lines_in_row[k].finish_j+1] != EMPTY))){
                    int val = (lines_in_row[k].size + 1 > g.c) ? g.c : lines_in_row[k].size + 1;
                    possible_lines_of_length[val]++;
                }

            }
        }
        down += 1;
    }
}

// Indica que tan buena es el movimiento pasado como parámetro
int calculate_move_score(Game &g, char player, int movement, std::vector<int>& p, std::vector<int>& q, std::vector<int>& s, std::vector<int>& t){
    do_move(g, movement);

    std::vector<int> number_of_lines_of_length;
    std::vector<int> number_of_effective_lines_of_length;
    std::vector<int> number_of_possible_lines_of_length;

    calculate_game_info(g, player, number_of_lines_of_length, number_of_effective_lines_of_length, number_of_possible_lines_of_length);

    std::vector<int> rival_number_of_lines_of_length;
    std::vector<int> rival_number_of_effective_lines_of_length;
    std::vector<int> rival_number_of_possible_lines_of_length;

    calculate_game_info(g, next_player(player), rival_number_of_lines_of_length, rival_number_of_effective_lines_of_length, rival_number_of_possible_lines_of_length);

    int score = 0;

    for(int i = 2; i <= g.c ; i++){
        score += ( number_of_effective_lines_of_length[i] * p[i] - rival_number_of_effective_lines_of_length[i] * s[i] );
        score += ( number_of_possible_lines_of_length[i] * q[i] - rival_number_of_possible_lines_of_length[i] * t[i] );
    }

    if(0 < rival_number_of_possible_lines_of_length[g.c]) 
        score = rival_number_of_possible_lines_of_length[g.c] * -INFINITE;
    if(number_of_lines_of_length[g.c]) 
        score = INFINITE;

    undo_move(g, movement);
    return score;
}

int greedy_move(Game &g, std::vector<int>& parameters) {
    std::vector<int> p;
    std::vector<int> q;
    std::vector<int> s;
    std::vector<int> t;
    assign_parameters(g.c, parameters, p, q, s, t);

    std::vector<movement_score> valid_movements;
    for (int i = 0; i < g.cols; i++) {
        if (valid_move(g, i)) {
            movement_score mc;
            mc.movement = i;
            mc.score = calculate_move_score(g, g.current_player, i, p, q, s, t);
            // std::cerr << "mov: " << mc.movement << " score: " << mc.score << std::endl;
            valid_movements.push_back(mc);
        }
    }

    return center_max(valid_movements, g.cols/2);

    // // DEBUG LINES
    // int mov = center_max(valid_movements, g.cols/2);
    // do_move(g, mov);
    // std::vector<int> number_of_lines_of_length;
    // std::vector<int> number_of_effective_lines_of_length;
    // std::vector<int> number_of_possible_lines_of_length;

    // calculate_game_info(g, next_player(g.current_player), number_of_lines_of_length, number_of_effective_lines_of_length, number_of_possible_lines_of_length);

    // std::vector<int> rival_number_of_lines_of_length;
    // std::vector<int> rival_number_of_effective_lines_of_length;
    // std::vector<int> rival_number_of_possible_lines_of_length;

    // calculate_game_info(g, g.current_player, rival_number_of_lines_of_length, rival_number_of_effective_lines_of_length, rival_number_of_possible_lines_of_length);

    // std::cerr << "number_of_lines_of_length" << std::endl;    
    // for(int i = 0; i < number_of_lines_of_length.size(); ++i){
    //     std::cerr << number_of_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;
    // for(int i = 0; i < rival_number_of_lines_of_length.size(); ++i){
    //     std::cerr << rival_number_of_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;

    // std::cerr << "number_of_effective_lines_of_length" << std::endl;    
    // for(int i = 0; i < number_of_effective_lines_of_length.size(); ++i){
    //     std::cerr << number_of_effective_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;
    // for(int i = 0; i < rival_number_of_effective_lines_of_length.size(); ++i){
    //     std::cerr << rival_number_of_effective_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;

    // std::cerr << "number_of_possible_lines_of_length" << std::endl;
    // for(int i = 0; i < number_of_possible_lines_of_length.size(); ++i){
    //     std::cerr << number_of_possible_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;
    // for(int i = 0; i < rival_number_of_possible_lines_of_length.size(); ++i){
    //     std::cerr << rival_number_of_possible_lines_of_length[i] << " ";
    // }
    // std::cerr << std::endl;

    // undo_move(g, mov);
    // return mov;
}
