/**
 * @file reduction.h
 * @author Vincent Penelle <vincent.penelle@u-bordeaux.fr>
 * @brief Function for creating and manipulating a reduction from lightup to SAT.
 * @version 1
 * @date 2022-09-19
 *
 * @copyright Creative Commons
 *
 */

#include "game.h"
#include "z3.h"

/**
 * @brief Returns the variable representing the cell (@p row, @p col).
 * Use this function everytime you need to create the variable in your formula, no need to store them somewhere.
 *
 * @param ctx
 * @param row
 * @param col
 * @return Z3_ast
 */
Z3_ast getVariableCell(Z3_context ctx, uint row, uint col);

/**
 * @brief Computes the formula whose satisfiability is equivalent from that of the game @p g.
 *
 * @param ctx
 * @param g
 * @return Z3_ast
 */
Z3_ast gameFormula(Z3_context ctx, game g);

/**
 * @brief Computes a formula which is satisfiable if and only if there exists another solution than that present in @p model. Does it by modifying @p formula which has been obtained by gameFormula.
 *
 * @param ctx
 * @param formula
 * @param model
 * @return Z3_ast
 */
Z3_ast other_sol_formula(Z3_context ctx, game g, Z3_ast formula, Z3_model model);

/**
 * @brief Fills the game @p g with lights according to the informations provided by @p model. Does not verify that the returned solution is correct.
 *
 * @param ctx
 * @param model
 * @param g
 */
void applySolutionToGame(Z3_context ctx, Z3_model model, game g);