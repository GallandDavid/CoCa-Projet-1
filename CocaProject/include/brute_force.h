/**
 * @file brute_force.h
 * @author Vincent Penelle <vincent.penelle@u-bordeaux.fr>
 * @brief Verifier and Brute Force algorithm for lightup instances.
 * @version 1
 * @date 2022-09-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __BRUTE_H__
#define __BRUTE_H__

#include "game.h"

/**
 * @brief Checks if all empty cells of @p g are lighted by some light.
 * 
 * @param g 
 * @return true 
 * @return false 
 */
bool all_cells_lighted(game g);

/**
 * @brief Checks if two lights are lighting each other in @p g.
 * 
 * @param g 
 * @return true 
 * @return false 
 */
bool no_lights_aligned(game g);

/**
 * @brief Checks if all walls have the correct number of lights adjacent to them in @p g.
 * 
 * @param g 
 * @return true 
 * @return false 
 */
bool walls_constraints_check(game g);

/**
 * @brief Checks if an assignation of lights in a lightup instance is a solution.
 * 
 * @param g a game with lights placed
 * @return true if all rules are satisfied
 * @return false if some rule is unsatisfied
 */
bool verifier(game g);

/**
 * @brief Solves a game using a brute force algorithm.
 * 
 * @param g 
 * @return true 
 * @return false 
 */
bool brute_force(game g);

#endif