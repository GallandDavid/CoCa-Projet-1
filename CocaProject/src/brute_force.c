#include "brute_force.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct pos_t{
    int row;
    int col;
}pos;


void printPos(pos p){
    printf("(%d,%d)",p.row,p.col);
}

void printPossibilityTab(int **Psblty_tab, int size, pos *LPs){
    printf("PT_size = %d\n",size);
    printf("Possibility_tab = { ");
    for(int i = 0; i <= size; i++){
        printf("[ ");
        if(Psblty_tab[i] != NULL){
            printf("Psblty_tab[i][0] = %d\n", Psblty_tab[i][0]);
            for(int j = 1; j <= Psblty_tab[i][0]; j ++){
                printPos(LPs[Psblty_tab[i][j]]);
                if(j != Psblty_tab[i][0]) printf(" , ");
            }
        }
        printf(" ]");
    }
    printf(" }\n");
}

void print_1d_tab_pos(pos *LPs, int size){
    printf("LPS = [ ");
    for(int i = 0; i < size; i ++){
        pos position = LPs[i];
        printPos(LPs[i]);
    }
    printf("]\n");
}

void printLPt(int * LPt){
    printf("LPt_cursor = %d\n",LPt[0]);
    printf("LPt = [ ");
    for(int i = 1; i < LPt[0]; i++){
        printf("%d,",LPt[i]);
    }
    printf(" ]\n");
}

void information(game g, pos *LPs, int *LPt , int nb_lights, int **possibility_tab, int nb_pos){
    game_print(g, true);
    printf("nb lights = %d\n", nb_lights);
    print_1d_tab_pos(LPs,nb_pos);
    printLPt(LPt);
    printPossibilityTab(possibility_tab, LPt[0] - 2,LPs);
}

//verifier
bool light_is_alone(const game g, uint i, uint j){
    for(uint x = i + 1; x < game_nb_rows(g); x++){
        if(game_is_black(g, x, j)) break;
        if(game_is_lightbulb(g, x, j)) return false;
    }
    for(uint y = j + 1; y < game_nb_cols(g); y++){
        if(game_is_black(g, i, y)) break;
        if(game_is_lightbulb(g, i, y)) return false;
    }
    for(int x = i - 1; x > (-1); x--){
        if(game_is_black(g, (unsigned int)x, j)) break;
        if(game_is_lightbulb(g, (unsigned int)x, j)) return false;
    }
    for(int y = j - 1; y > (-1); y--){
        if(game_is_black(g, i, (unsigned int)y)) break;
        if(game_is_lightbulb(g, i, (unsigned int)y)) return false;
    }
    return true;
}

//verifié
int nbBlankCases(game g){
    int nb_blank = 0;
    for(uint i = 0; i < game_nb_rows(g); i++){
        for(uint j = 0; j < game_nb_cols(g); j ++){
            if(game_is_blank(g, i, j)) nb_blank ++;
        }
    }
    return nb_blank;
}

bool check_around_lesser(game g, uint row, uint col, int nb_light){
    if(nb_light == -1) return true;
    int lights = 0;
    if(row > 0) if(game_is_lightbulb(g, row - 1, col)) lights ++;
    if(row < game_nb_rows(g) - 1) if(game_is_lightbulb(g, row + 1, col)) lights ++;
    if(col > 0) if(game_is_lightbulb(g, row, col - 1)) lights ++;
    if(col < game_nb_cols(g) - 1) if(game_is_lightbulb(g, row, col + 1)) lights ++;
    return nb_light >= lights;
}

bool haveBlack0(game g, uint row, uint col){
    if(row > 0) if(game_is_black(g, row - 1, col)) if(game_get_black_number(g, row - 1, col) == 0) return true;
    if(row < game_nb_rows(g) - 1) if(game_is_black(g, row + 1, col)) if(game_get_black_number(g, row + 1, col) == 0) return true;
    if(col > 0) if(game_is_black(g, row, col - 1)) if(game_get_black_number(g, row, col - 1) == 0) return true;
    if(col < game_nb_cols(g) - 1) if(game_is_black(g, row, col + 1)) if(game_get_black_number(g, row, col + 1) == 0) return true;
    return false;
}

//verifié
int findAllPositions(game g, pos * LPs){
    int nb_valid_pos = 0;
    for(uint i = 0; i < game_nb_rows(g); i++){
        for(uint j = 0; j < game_nb_cols(g); j ++){
            if(game_is_blank(g, i, j)){
                if(!haveBlack0(g, i, j)){
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

bool checkBlockAround(const game g, uint row, uint col){
    if(row > 0) if(game_is_black(g, row - 1, col)) if(!check_around_lesser(g, row -1, col, game_get_black_number(g, row - 1, col))) return false;
    if(row < game_nb_rows(g) - 1) if(game_is_black(g, row + 1, col)) if(!check_around_lesser(g, row + 1, col, game_get_black_number(g, row + 1, col))) return false;
    if(col > 0) if(game_is_black(g, row, col - 1)) if(!check_around_lesser(g, row, col - 1, game_get_black_number(g, row, col - 1))) return false;
    if(col < game_nb_cols(g) - 1) if(game_is_black(g, row, col + 1)) if(!check_around_lesser(g, row, col + 1, game_get_black_number(g, row, col + 1))) return false;
    return true;
}

//verifier
bool checkIfGoodPlace(game g, uint row, uint col){
    bool check = light_is_alone(g,row,col) && checkBlockAround(g, row, col);
    return check;
}

//V
bool checkPossibility(game g, pos *LPs, int *LPt , int nb_lights, int **possibility_tab, int decay){
    //check if have other possibility for this position
    if(possibility_tab[LPt[0] - (1 )][0] == 0 || possibility_tab[LPt[0] - (1 )][0] < nb_lights - LPt[0]) return false; /* check if there is still enough possibility for the other next light to be placed*/
    return true;
}

//V
int nextPossibility(game g, pos *LPs, int *LPt , int nb_lights, int **possibility_tab){
    //put light on solution tab from possibility of this possition
    LPt[LPt[0]] = possibility_tab[LPt[0] - 1][possibility_tab[LPt[0] - 1][0]];

    //put this light on grid and remove this possibility for the other possibility that this position can take
    game_set_square(g, LPs[LPt[LPt[0]]].row, LPs[LPt[LPt[0]]].col, LIGHTBULB);
    possibility_tab[LPt[0] - 1] = realloc(possibility_tab[LPt[0] - 1], sizeof(int) * (possibility_tab[LPt[0] - 1][0]));
    possibility_tab[LPt[0] - 1][0] --;

    //Check if this placement agree conditions
    if(checkIfGoodPlace(g, LPs[LPt[LPt[0]]].row, LPs[LPt[LPt[0]]].col)){

        //Check if it's the last light to place
        if(LPt[0] + 1 > nb_lights){
            LPt[0] ++;
            return 2;
        }

        //create the possibility to the next light
        possibility_tab[LPt[0]] = malloc(sizeof(int) * (possibility_tab[LPt[0] - 1][0] + 1));
        for(int i = 1; i <= possibility_tab[LPt[0] - 1][0]; i ++) possibility_tab[LPt[0]][i] = possibility_tab[LPt[0] - 1][i];
        possibility_tab[LPt[0]][0] = possibility_tab[LPt[0] - 1][0];
        LPt[0] ++;
        return 1;
    }

    //If not, remove light on grid, 
    else {
        game_set_square(g, LPs[LPt[LPt[0]]].row, LPs[LPt[LPt[0]]].col, BLANK);
        LPt[LPt[0]] = -1;
        return 1;
    }
}


int prevLight(game g, pos *LPs, int *LPt , int nb_lights, int **possibility_tab, int nb_pos){
    if(LPt[0] == 1) return 0;
    LPt[0] -= 1;
    game_set_square(g, LPs[LPt[LPt[0]]].row, LPs[LPt[LPt[0]]].col, BLANK);
    LPt[LPt[0]] = -1;/*
    if(possibility_tab[LPt[0] - 1] != NULL){
        free(possibility_tab[LPt[0] - 1]);
        possibility_tab[LPt[0] - 1] = NULL;
    }*/
    if(checkPossibility(g, LPs, LPt , nb_lights, possibility_tab,1)){
        return 1;
    }else{
        return prevLight(g, LPs, LPt , nb_lights, possibility_tab,nb_pos);
    }
}

int nextPos(game g, pos *LPs, int *LPt, int nb_lights, int **possibility_tab, int  nb_pos){
    //check if last light was placed on last position
    if(LPt[0] > nb_lights){
        int ret;
        //prevLight go forward until a light who has enought possibility for it and the next light. return 0 if didn't found a light who agree with conditions 1 otherwise
        ret = prevLight(g, LPs, LPt , nb_lights, possibility_tab, nb_pos);
        if(!ret) return ret;
    }  //check if can place the next light
    if(checkPossibility(g, LPs, LPt , nb_lights, possibility_tab, 0 )){ //true if can place the current light and have possibility to place other next light false otherwise
        return nextPossibility(g,LPs,LPt,nb_lights, possibility_tab);
    }else{
        int ret;
        ret = prevLight(g,LPs,LPt,nb_lights, possibility_tab, nb_pos);
        if(!ret) return ret;
    }
}

//verifier
bool check_is_lighted(game g, uint i, uint j){
    for(uint x = i + 1; x < game_nb_rows(g); x++){
        if(game_is_black(g, x, j)) break;
        if(game_is_lightbulb(g, x, j)) return true;
    }
    for(uint y = j + 1; y < game_nb_cols(g); y++){
        if(game_is_black(g, i, y)) break;
        if(game_is_lightbulb(g, i, y)) return true;
    }
    for(int x = i - 1; x >= 0; x--){
        if(game_is_black(g,(unsigned int) x, j)) break;
        if(game_is_lightbulb(g, (unsigned int)x, j)) return true;
    }
    for(int y = j - 1; y >= 0; y--){
        if(game_is_black(g, i, (unsigned int)y)) break;
        if(game_is_lightbulb(g, i, (unsigned int)y)) return true;
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



bool verifier(game g){
    //game_print(g, true);
    for(uint i = 0; i < game_nb_rows(g); i++){
        for(uint j = 0; j < game_nb_cols(g); j ++){
            if(game_is_lightbulb(g, i, j)){ if(!light_is_alone(g, i, j)) return false; }
            else if(game_is_blank(g, i, j)){ if(!check_is_lighted(g, i, j)) return false; 
            }
            else if(game_is_black(g, i, j)) if(game_get_black_number(g, i, j) != 0 && game_get_black_number(g, i, j) != -1) if(!check_around(g, i, j, game_get_black_number(g, i, j))) return false;            
        }
    }
    /*Implement here the verifier that the game is solved. You may want to cut this function into subfunctions.*/
    return true;
}

bool brute_force(game g){
    int blank_cases = nbBlankCases(g); //compte le nombre totale de case vide et le retourne
    pos *LPs = malloc(sizeof(pos) * blank_cases); //index 0 utilisé pour stocker la taille du tableau
    int nb_pos = findAllPositions(g, LPs); //Ensemble des position valide pour une lampe
    LPs = realloc(LPs, sizeof(pos) * nb_pos); //realloc at the better size 
    int nb_lights = 1;

    //LPt[0] = index of next light that need to be place (if 5 light to be placed and 5 placed LPt[0] = 6 but be catch like end and not next light to be placed)
    int *LPt = malloc(sizeof(int) * 1);

    //test all amount of light
    while(nb_lights <= nb_pos){
        sleep(1);

        //possibility_tab[x][0] = index length of possibility_tab[x]
        int **possibility_tab = malloc(sizeof(int*) * (unsigned long)nb_lights); //allocate as much array of possibility that has light to placed
        possibility_tab[0] = malloc(sizeof(int) * (nb_pos + 1)); //allocate as much possibility for the first light that has good position
        for(int i = 0; i < nb_pos; i ++) possibility_tab[0][i + 1] = i; //fill it
        possibility_tab[0][0] = nb_pos;

        //allocate the solution tab that store index of LPs tab (all the possibility), in order to lights that need to be placed
        LPt = realloc(LPt,sizeof(int) * (nb_lights + 1));
        //fill it with -1 for know that haven't light at this position
        for(int i = 1; i < nb_lights + 1; i ++) LPt[i] = -1;
        LPt[0] = 1;

        //test all the possibility for a light amount
        bool running = true;
        while(running){
            //sleep(1);
            //information(g,LPs,LPt,nb_lights,possibility_tab, nb_pos);
            //try to find a next position for current lamp or go backward until a amount of possibility who's right. return 0 if no other possibility can be tested, 2 if fill the solution tab, 1 otherwise
            int find = nextPos(g,LPs, LPt, nb_lights, possibility_tab, nb_pos);
            if(find == 0){
                running = false;
                break;
            }
            //check if this try is a solution
            if(find == 2) if(verifier(g)){
                free_pos_tab(possibility_tab, nb_lights);
                return true;
            }
        }
        free_pos_tab(possibility_tab, nb_lights);
        nb_lights ++;
    }
    return false;
}

void free_poss_tab(int **possibility_tab, int nb_lights){
    for(int i = 0; i < nb_lights; i++){
        if(possibility_tab[i] != NULL){
            free(possibility_tab[i]);
            possibility_tab[i] = NULL;
        }
    }
    free(possibility_tab);
}