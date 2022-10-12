#include "reduction.h"
#include "Z3Tools.h"
#include "game.h"

/* The code of this formula is given to you so that all of you will have the same notations for the variables.
    You should call this function every time you need to use the variable [row,col] either in constructing the formula or determining their value in a model. It is not necessary to create them only once (and it is not possible). Every call with the same argument will return the same function.
*/
Z3_ast getVariableCell(Z3_context ctx, uint row, uint col)
{
    char name[40];
    snprintf(name, 40, "[%d,%d]", row, col);
    return mk_bool_var(ctx, name);
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
{
    /*You might need this formula, implement it as needed.*/
    return Z3_mk_true(ctx);
}

Z3_ast exactNumFormula(Z3_context ctx, Z3_ast *vars, uint num, uint numTrue)
{
    /*Formula corresponding to the one provided in the subject.*/
    return Z3_mk_true(ctx);
}

Z3_ast gameFormula(Z3_context ctx, game g)
{
    /*Implement the reduction here. You should cut it in subfunction, for example one per rule of the game.*/
    return Z3_mk_true(ctx);
}

Z3_ast other_sol_formula(Z3_context ctx, game g, Z3_ast formula, Z3_model model)
{
    /*Implement here the formula described in the bonus of the reduction part.
    Note that you do not have to regenerate the formula describing the game as it is given in argument, you simply have to complete it.*/
    return (Z3_mk_true(ctx));
}

void applySolutionToGame(Z3_context ctx, Z3_model model, game g)
{
    /*Investigates the model obtained to place lamps in g.
    You can obtain the value of a variable with valueOfVarInModel.*/
}