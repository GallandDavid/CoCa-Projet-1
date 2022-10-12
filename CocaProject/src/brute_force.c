#include "brute_force.h"

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
    if(row < game_nb_cols(g) - 1) if(game_is_lightbulb(g, row, col + 1)) lights ++;
    return nb_light == lights;
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

bool brute_force(game g)
{
    /*Implement here a brute force algorithm, calling verifier as a subroutine. You may (should) define an auxiliary function that is recursive to perform the choices.
    Make it as efficient as possible, while being correct.*/
    return false;
}