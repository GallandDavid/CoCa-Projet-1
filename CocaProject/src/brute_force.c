#include "brute_force.h"

bool have_other_possibility = false;

typedef struct pos_t{
    int row;
    int col;
}pos;

int nbBlankCases(game g){
    int nb_blank = 0;
    for(uint i = 0; i < game_nb_rows(g); i++){
        for(uint j = 0; j < game_nb_cols(g); j ++){
            if(game_is_blank(g, i, j)) nb_blank ++;
        }
    }
    return nb_blank;
}

bool haveBlack0(game g, uint row, uint col){
    if(row > 0) if(game_is_black(g, row - 1, col)) if(game_get_black_number(g, row - 1, col) == 0) return true;
    if(row < game_nb_rows(g) - 1) if(game_is_black(g, row + 1, col)) if(game_get_black_number(g, row + 1, col) == 0) return true;
    if(col > 0) if(game_is_black(g, row, col - 1)) if(game_get_black_number(g, row, col - 1) == 0) return true;
    if(col < game_nb_cols(g) - 1) if(game_is_black(g, row, col + 1)) if(game_get_black_number(g, row, col + 1) == 0) return true;
    return false;
}

int findAllPositions(game g, pos * LPs){
    int nb_valid_pos = 0;
    for(uint i = 0; i < game_nb_rows(g); i++){
        for(uint j = 0; j < game_nb_cols(g); j ++){
            if(game_is_blank(g, i, j)){
                if(!haveBlack0(g, i, j)){
                    //PAS SUR POUR LA DECLARATION DE POSITION (MALLOC PAS UTILISABLE)
                    pos position; 
                    position.row = i;
                    position.col = j;
                    LPs[nb_valid_pos] = position;
                    nb_valid_pos ++;
                }
            }
        }
    }
    return nb_valid_pos;
}

checkBlockAround(const game g, uint row, uint col){

    if(row > 0) if(game_is_black(g, row - 1, col)) if(!check_around_lesser(g, row -1, col, game_get_black_number(g, row - 1, col))) return false;
    if(row < game_nb_rows(g) - 1) if(game_is_black(g, row + 1, col)) if(!check_around_lesser(g, row + 1, col, game_get_black_number(g, row + 1, col))) return false;
    if(col > 0) if(game_is_black(g, row, col - 1)) if(!check_around_lesser(g, row, col - 1, game_get_black_number(g, row, col - 1))) return false;
    if(col < game_nb_cols(g) - 1) if(game_is_black(g, row, col + 1)) if(!check_around_lesser(g, row, col + 1, game_get_black_number(g, row, col + 1))) return false;
    return true;
}

bool checkIfGoodPlace(game g, uint row, uint col){
    return light_is_alone(g,row,col) && checkBlockAround(g, row, col);
}

int nextPos(game g, pos *LPs, int *LPt, int nb_lights, int **possibility_tab){
    if(LPt[0] > nb_lights){
        LPt[0] -= 1;
        game_set_square(g, LPs[LPt[LPt[0]]].row, LPs[LPt[LPt[0]]].col, BLANK);
        LPt[LPt[0]] = -1;
    }
    if(possibility_tab[LPt[0] - 1][0] == 0){
        if(LPt[0] == 0) return 0;
        if(LPt[LPt[0]] != -1){
            game_set_square(g, LPs[LPt[LPt[0]]].row, LPs[LPt[LPt[0]]].col, BLANK);
            LPt[LPt[0]] = -1;
        }
        free(possibility_tab[LPt[0] - 1]);
        return 1;
    }
    LPt[LPt[0]] = possibility_tab[LPt[0] - 1][0];
    game_set_square(g, LPs[LPt[0]].row, LPs[LPt[0]].col, LIGHTBULB);
    if(checkIfGoodPlace(g, LPs[LPt[0]].row, LPs[LPt[0]].col)){
        possibility_tab[LPt[0] - 1] = realloc(possibility_tab[LPt[0] - 1], sizeof(int) * (possibility_tab[LPt[0] - 1][0] - 1));
        possibility_tab[LPt[0] - 1][0] -= 1;

        if(LPt[0] > nb_lights) return 2;

        if(possibility_tab[LPt[0]] != 0) free(possibility_tab[LPt[0]]);
        possibility_tab[LPt[0]] = malloc(sizeof(int) * possibility_tab[LPt[0] - 1][0]);
        for(int i = 0; i < possibility_tab[LPt[0] - 1]; i ++) possibility_tab[LPt[0]][i] = possibility_tab[LPt[0] - 1][i];
        LPt[0] += 1;
        
        return 1;
    }
    else {
        LPt[LPt[0]] = -1;
        possibility_tab[LPt[0]] = realloc(possibility_tab[LPt[0]], sizeof(int) * (possibility_tab[LPt[0]][0] - 1));
        possibility_tab[LPt[0]][0] -= 1;
        return 1;
    }
}

bool light_is_alone(const game g, uint i, uint j){
    for(uint x = i + 1; i < game_nb_rows(g); i++){
        if(game_is_black(g, x, j)) break;
        if(game_is_lightbulb(g, x, j)) return false;
    }
    for(uint y = j + 1; i < game_nb_cols(g); j++){
        if(game_is_black(g, i, y)) break;
        if(game_is_lightbulb(g, i, y)) return false;
    }
    for(uint x = i - 1; i >= 0; i--){
        if(game_is_black(g, x, j)) break;
        if(game_is_lightbulb(g, x, j)) return false;
    }
    for(uint y = j - 1; i >= 0; j--){
        if(game_is_black(g, i, y)) break;
        if(game_is_lightbulb(g, i, y)) return false;
    }
    return true;
}

bool check_is_lighted(game g, uint i, uint j){
    for(uint x = i + 1; i < game_nb_rows(g); i++){
        if(game_is_black(g, x, j)) break;
        if(game_is_lightbulb(g, x, j)) return true;
    }
    for(uint y = j + 1; i < game_nb_cols(g); j++){
        if(game_is_black(g, i, y)) break;
        if(game_is_lightbulb(g, i, y)) return true;
    }
    for(uint x = i - 1; i >= 0; i--){
        if(game_is_black(g, x, j)) break;
        if(game_is_lightbulb(g, x, j)) return true;
    }
    for(uint y = j - 1; i >= 0; j--){
        if(game_is_black(g, i, y)) break;
        if(game_is_lightbulb(g, i, y)) return true;
    }
    return false;
}

bool check_around(game g, uint row, uint col, int nb_light){
    int lights = 0;
    if(row > 0) if(game_is_lightbulb(g, row - 1, col)) lights ++;
    if(row < game_nb_rows(g) - 1) if(game_is_lightbulb(g, row + 1, col)) lights ++;
    if(col > 0) if(game_is_lightbulb(g, row, col - 1)) lights ++;
    if(col < game_nb_cols(g) - 1) if(game_is_lightbulb(g, row, col + 1)) lights ++;
    return nb_light == lights;
}

bool check_around_lesser(game g, uint row, uint col, int nb_light){
    int lights = 0;
    if(row > 0) if(game_is_lightbulb(g, row - 1, col)) lights ++;
    if(row < game_nb_rows(g) - 1) if(game_is_lightbulb(g, row + 1, col)) lights ++;
    if(col > 0) if(game_is_lightbulb(g, row, col - 1)) lights ++;
    if(col < game_nb_cols(g) - 1) if(game_is_lightbulb(g, row, col + 1)) lights ++;
    return nb_light >= lights;
}

bool verifier(game g)
{
    for(uint i = 0; i < game_nb_rows(g); i++){
        for(uint j = 0; j < game_nb_cols(g); j ++){
            if(game_is_lightbulb(g, i, j)){ if(!light_is_alone(g, i, j)) return false; }
            else if(game_is_blank(g, i, j)){ if(!check_is_lighted(g, i, j)) return false; }
            else if(game_is_black(g, i, j)) if(game_get_black_number(g, i, j) != 0) if(!check_around(g, i, j, game_get_black_number(g, i, j))) return false;
            
        }
    }
    /*Implement here the verifier that the game is solved. You may want to cut this function into subfunctions.*/
    return true;
}

bool brute_force(game g){
    int blank_cases = nbBlankCases(g); //compte le nombre totale de case vide et le retourne
    pos *LPs = malloc(sizeof(pos) * blank_cases); //index 0 utilisé pour stocker la taille du tableau
    int nb_pos = findAllPositions(g, LPs); //Ensemble des position valide pour une lampe
    int nb_lights = 1;
    while(nb_lights <= blank_cases){
        int **possibility_tab = malloc(sizeof(int*) * (unsigned long)nb_lights);
        possibility_tab[0] = malloc(sizeof(int) * nb_pos);
        for(int i = 0; i < nb_pos; i ++) possibility_tab[0][i] = i;
        possibility_tab[0][0] = nb_pos;

        int *LPt = malloc(sizeof(int) * nb_lights + 1);
        for(int i = 1; i < nb_lights + 1; i ++) LPt[i] = -1;
        LPt[0] = 1;

        have_other_possibility = false;
        bool running = true;
        while(running){
            int find = nextPos(g,LPs, LPt, nb_lights, possibility_tab);
            if(find == 0) break;
            if(find == 2) if(verifier(g)) return true;
            LPt[0] ++;
        }
        if(!have_other_possibility) break;
        nb_lights ++;
    }
    return false;
}