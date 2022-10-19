#include "reduction.h"
#include "Z3Tools.h"
#include "game.h"
#include <stdlib.h>

/* The code of this formula is given to you so that all of you will have the same notations for the variables.
    You should call this function every time you need to use the variable [row,col] either in constructing the formula or determining their value in a model. It is not necessary to create them only once (and it is not possible). Every call with the same argument will return the same function.
*/



void displayAst(Z3_context ctx, Z3_ast ast){
    printf("%s\n",Z3_ast_to_string(ctx, ast));
}

void displayAsttab(Z3_context ctx, Z3_ast *tab, uint size){
    printf("%u\n",size);
    for(uint i = 0; i < size; i ++){
        displayAst(ctx,tab[i]);
    }
}

Z3_ast getVariableCell(Z3_context ctx, uint row, uint col)
{
    char name[40];
    snprintf(name, 40, "[%d,%d]", row, col);
    return mk_bool_var(ctx, name);
}

uint findNeighbourLinear(const game g, Z3_context ctx, uint i, uint j, Z3_ast *neighbour){
    uint it = 0;
    for(uint x = i + 1; x < game_nb_rows(g); x++){
        if(game_is_black(g, x, j)) break;
        neighbour[it] = getVariableCell(ctx, x , j);
        it += 1;;
    }
    for(uint y = j + 1; y < game_nb_cols(g); y++){
        if(game_is_black(g, i, y)) break;
        neighbour[it] = getVariableCell(ctx, i, y);
        it ++;
    }
    for(int x = i - 1; x > (-1); x--){
        if(game_is_black(g, (unsigned int)x, j)) break;
        neighbour[it] = getVariableCell(ctx, (unsigned int)x, j);
        it ++;
    }
    for(int y = j - 1; y > (-1); y--){
        if(game_is_black(g, i, (unsigned int)y)) break;
        neighbour[it] = getVariableCell(ctx, i, (unsigned int)y);
        it ++;
    }
    return it;
}

uint findNeighbourBlack(const game g, Z3_context ctx, uint row, uint col, Z3_ast *neighbour){
    uint it = 0;
    if(row > 0) if(!game_is_black(g, row - 1, col)){
        neighbour[it] = getVariableCell(ctx, row - 1, col);
        it ++;
    }
    if(row < game_nb_rows(g) - 1) if(!game_is_black(g, row + 1, col)){
        neighbour[it] = getVariableCell(ctx, row + 1, col);
        it ++;
    }
    if(col > 0) if(!game_is_black(g, row, col - 1)){
        neighbour[it] = getVariableCell(ctx, row, col - 1);
        it ++;
    }
    if(col < game_nb_cols(g) - 1) if(!game_is_black(g, row, col + 1)){
        neighbour[it] = getVariableCell(ctx, row, col + 1);
        it ++;
    }
}

/**
 * @brief Generates a formula stating that at most one of the formulae from @p formulae is true.
 *
 * @param ctx The solver context.
 * @param formulae The formulae.
 * @param size The number of formulae.
 * @return Z3_ast The obtained formula.
 */
Z3_ast atMostFormula(Z3_context ctx, Z3_ast *formulae, uint size)
{   Z3_ast result;
    Z3_ast tab[size];
    for(uint i = 0; i < size ; i += 2){
        tab[i] = Z3_mk_not(ctx, formulae[i]);
    }
    return Z3_mk_and(ctx, size, tab);
}

Z3_ast atLeastFormula(Z3_context ctx, Z3_ast *formulae, uint size, Z3_ast pos)
{   Z3_ast result;
    printf("%u\n",size);
    
    result = Z3_mk_and(ctx, size, formulae);
    return Z3_mk_xor(ctx, result,pos);
}

Z3_ast exactNumFormula(Z3_context ctx, Z3_ast *vars, uint num, uint numTrue)
{
    switch(numTrue){
        Z3_ast first;
        Z3_ast second;
        Z3_ast third;
        Z3_ast fourth;
        Z3_ast fifth;
        Z3_ast sixth;
        case 0:
            for (uint i = 0; i < num; i++)
            {
                vars[i] = Z3_mk_not(ctx, vars[i]);
            }
            switch(num){
                case 0:
                    return Z3_mk_true(ctx);
                case 1:
                    return Z3_mk_and(ctx,1,vars);
                case 2:
                    return Z3_mk_and(ctx,2,vars);
                case 3:
                    return Z3_mk_and(ctx,3,vars);
                case 4:
                    return Z3_mk_and(ctx,4,vars);
            }
            break;
        case 1:
            switch(num){
                case 1:
                    return Z3_mk_and(ctx,1,vars);
                case 2:
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    first = Z3_mk_and(ctx,2,vars); // ¬a ^ b
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    second = Z3_mk_and(ctx,2,vars); // a ^ ¬b
                    Z3_ast combine12[2] = {first,second};
                    return Z3_mk_or(ctx,2,combine12);
                case 3:
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    first = Z3_mk_and(ctx,3,vars); // ¬a ^ ¬b ^ c
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    second = Z3_mk_and(ctx,3,vars); // a ^ ¬b ^ ¬c
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    third = Z3_mk_and(ctx,3,vars); // ¬a ^ b ^ ¬c
                    Z3_ast combine13[3] = {first,second,third};
                    return Z3_mk_or(ctx,3,combine13);
                case 4:
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    first = Z3_mk_and(ctx,4,vars); // ¬a ^ ¬b ^ ¬c ^ d
                    vars[3] = Z3_mk_not(ctx, vars[3]);
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    second = Z3_mk_and(ctx,4,vars); // ¬a ^ ¬b ^ c ^ ¬d
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    third = Z3_mk_and(ctx,4,vars); // ¬a ^ b ^ ¬c ^ ¬d
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    fourth = Z3_mk_and(ctx,4,vars); // a ^ ¬b ^ ¬c ^ ¬d
                    Z3_ast combine14[4] = {first,second,third,fourth};
                    return Z3_mk_or(ctx,4,combine14);
            }
            break;
        case 2:
            switch(num){
                case 2:
                    return Z3_mk_and(ctx,2,vars);
                case 3:
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    first = Z3_mk_and(ctx,3,vars); // ¬a ^ b ^ c
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    second = Z3_mk_and(ctx,3,vars); // a ^ ¬b ^ c
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    third = Z3_mk_and(ctx,3,vars); // a ^ b ^ ¬c
                    Z3_ast combine23[3] = {first,second,third};
                    return Z3_mk_or(ctx,3,combine23);
                case 4:
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    first = Z3_mk_and(ctx,4,vars); // ¬a ^ ¬b ^ c ^ d
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    second = Z3_mk_and(ctx,4,vars); // ¬a ^ b ^ ¬c ^ d
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[3] = Z3_mk_not(ctx, vars[3]);
                    third = Z3_mk_and(ctx,4,vars); // ¬a ^ b ^ c ^ ¬d
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[3] = Z3_mk_not(ctx, vars[3]);
                    fourth = Z3_mk_and(ctx,4,vars); // a ^ ¬b ^ ¬c ^ d
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[3] = Z3_mk_not(ctx, vars[3]);
                    fifth = Z3_mk_and(ctx,4,vars); // a ^ ¬b ^ c ^ ¬d
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    sixth = Z3_mk_and(ctx,4,vars); // a ^ b ^ ¬c ^ ¬d
                    Z3_ast combine24[6] = {first,second,third,fourth,fifth,sixth};
                    return Z3_mk_or(ctx,6,combine24);
            }
        case 3:
            switch(num){
                case 3:
                    return Z3_mk_and(ctx,3,vars);
                case 4:
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    first = Z3_mk_and(ctx,4,vars); // ¬a ^ b ^ c ^ d
                    vars[0] = Z3_mk_not(ctx, vars[0]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    second = Z3_mk_and(ctx,4,vars); // a ^ ¬b ^ c ^ d
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[1] = Z3_mk_not(ctx, vars[1]);
                    third = Z3_mk_and(ctx,4,vars); // a ^ b ^ ¬c ^ d
                    vars[2] = Z3_mk_not(ctx, vars[2]);
                    vars[3] = Z3_mk_not(ctx, vars[3]);
                    fourth = Z3_mk_and(ctx,4,vars); // a ^ b ^ c ^ ¬d
                    Z3_ast combine34[4] = {first,second,third,fourth};
                    return Z3_mk_or(ctx,4,combine34);
            }
        case 4:
            switch(num){
                case 4:
                    return Z3_mk_and(ctx,4,vars);
            }
    }
}

/**
 * @brief programme should run goodly. But a segfault income at the end of formulae construction. Trying to resolve but didn't understand how happen
*/
Z3_ast gameFormula(Z3_context ctx, game g)
{
    /*Implement the reduction here. You should cut it in subfunction, for example one per rule of the game.*/
    uint rows = game_nb_rows(g);
    uint cols = game_nb_cols(g);
    uint nb_case = rows * cols;
    
    Z3_ast finalAnd = Z3_mk_true(ctx);
    Z3_ast result;
    Z3_ast * neighbourLinear = malloc(sizeof(Z3_ast) * nb_case);
    Z3_ast * neighbour = malloc(sizeof(Z3_ast) * 4);
    Z3_ast tab[2];
    uint nb_neighbourLinear;
    uint nb_neighbour;
    for(uint i = 0; i < rows; i ++){
        for(uint j = 0; j < cols; j ++){
            square s = game_get_square(g,i,j);
            if(s == BLANK){
                    nb_neighbourLinear = findNeighbourLinear(g,ctx, i, j, neighbourLinear);
                    result = atLeastFormula(ctx, neighbourLinear, nb_neighbourLinear, getVariableCell(ctx,i,j));
            }else if(s == LIGHTBULB){
                    nb_neighbourLinear = findNeighbourLinear(g,ctx, i, j, neighbourLinear);
                    result = atMostFormula(ctx, neighbourLinear, nb_neighbourLinear);
            }else{
                nb_neighbour = findNeighbourBlack(g, ctx, i, j, neighbour);
                int black_nb = game_get_black_number(g,i,j);
                if(black_nb == 0){
                    result = exactNumFormula(ctx,neighbour,nb_neighbour,(unsigned int)0);
                }else if(black_nb == 1){
                    result = exactNumFormula(ctx,neighbour,nb_neighbour,(unsigned int)1);
                }else if(black_nb == 2){
                        result = exactNumFormula(ctx,neighbour,nb_neighbour,(unsigned int)2);
                }else if(black_nb == 3){
                        result = exactNumFormula(ctx,neighbour,nb_neighbour,(unsigned int)3);
                }else if(black_nb == 4){
                        result = exactNumFormula(ctx,neighbour,nb_neighbour,(unsigned int)4);
                }
            }
            unsigned int indice = 2;
            tab[0] = finalAnd;
            tab[1] = result;
            displayAst(ctx, finalAnd);
            displayAst(ctx,result);
            Z3_ast finalAnd  = Z3_mk_and(ctx,indice,tab);
            displayAst(ctx,finalAnd);
        }
    }
    return finalAnd;
}

Z3_ast other_sol_formula(Z3_context ctx, game g, Z3_ast formula, Z3_model model)
{
    /*Implement here the formula described in the bonus of the reduction part.
    Note that you do not have to regenerate the formula describing the game as it is given in argument, you simply have to complete it.*/
    return (Z3_mk_true(ctx));
}

void applySolutionToGame(Z3_context ctx, Z3_model model, game g)
{
    for (uint i = 0; i < game_nb_rows(g); i++){
        for (uint j = 0; j < game_nb_cols(g); j++){
            Z3_ast variable = getVariableCell(ctx,i,j);
            if(valueOfVarInModel(ctx,model,variable) == true){
                game_set_square(g,i,j,LIGHTBULB);
            }
        }
    }
}