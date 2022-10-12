/**
 * @file game.h
 * @brief Basic Game Functions.
 * @author Vincent Penelle <vincent.penelle@u-bordeaux.fr>
 * @details Freely inspired from Course of projet technologiques 2, 2021.
 * @copyright University of Bordeaux. All rights reserved, 2022.
 **/

#ifndef __GAME_H__
#define __GAME_H__
#include <stdbool.h>

/**
 * @brief Standard unsigned integer type.
 **/
typedef unsigned int uint;

/**
 * @brief Size of the default game grid.
 **/
#define DEFAULT_SIZE 7

/**
 * @brief Different squares used in the game.
 **/
typedef enum
{
    /* states */
    BLANK = 0,     /**< a blank square */
    LIGHTBULB = 1, /**< a light bulb */
    BLACK0 = 8,    /**< a numbered black wall (with 0 adjacent lights) */
    BLACK1,        /**< a numbered black wall (with 1 adjacent light) */
    BLACK2,        /**< a numbered black wall (with 2 adjacent lights) */
    BLACK3,        /**< a numbered black wall (with 3 adjacent lights) */
    BLACK4,        /**< a numbered black wall (with 4 adjacent lights) */
    BLACKU,        /**< an unnumbered black wall (any number of adjacent lights) */
} square;

/**
 * @brief The structure pointer that stores the game state.
 **/
typedef struct game_s *game;

/**
 * @brief Loads a game from the file @p filename.
 *
 * @param filename
 * @return game
 */
game game_load(char *filename);

/**
 * @brief Returns a copy of @p g.
 *
 * @param g
 * @return game
 */
game game_copy(game g);

/**
 * @brief Resets the game by removing all lightbulbs.
 *
 * @param g
 */
void game_reset(game g);

/**
 * @brief Loads a game from a description as a string as in Simon Tatham's website.
 *
 * @param description The description of the grid
 * @param nb_rows
 * @param nb_cols
 * @return game
 */
game string_to_game(char *description, uint nb_rows, uint nb_cols);

/**
 * @brief Prints the game @p g as text on the standard output stream.
 * @details The different squares are respectively displayed as text, based on a
 * square-character mapping table. If @p utf8 is set, the display will use UTF8 symbols.
 * @param g the game
 * @param utf8 controls set of characters used
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_print(const game g, bool utf8);

/**
 * @brief Prints the game @p g as text in the file whose name is @p filename.
 * Format compatible with the loading function.
 *
 * @param g
 * @param filename
 */
void game_print_to_file(game g, char *filename);

/**
 * @brief The structure pointer that stores the game state.
 **/
typedef struct game_s *game;

/**
 * @brief Creates a new empty game with defaut size.
 * @details All squares are initialized with blank squares.
 * @return the created game
 **/
game game_new_empty(uint n, uint m);

/**
 * @brief Deletes the game and frees the allocated memory.
 * @param g the game to delete
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_delete(game g);

/**
 * @brief  Returns true if cell (@p row,@p col) is lighted by some light in @p g.
 *
 * @param g
 * @param row
 * @param col
 * @return true
 * @return false
 */
bool is_lighted(game g, uint row, uint col);

/**
 * @brief Sets the value of a given square.
 * @details This function is useful for initializing the squares of an empty
 * game.
 * @param g the game
 * @param row row index
 * @param col column index
 * @param s the square value
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p row < game height
 * @pre @p col < game width
 * @pre @p s must be a valid square value.
 **/
void game_set_square(game g, uint row, uint col, square s);

/**
 * @brief Gets the raw value of a given square.
 * @param g the game
 * @param row row index
 * @param col column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p row < game height
 * @pre @p col < game width
 * @return the square value
 **/
square game_get_square(const game g, uint row, uint col);

/**
 * @brief Test if a given square is blank.
 * @param g the game
 * @param row row index
 * @param col column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p row < game height
 * @pre @p col < game width
 * @return true if the square is blank
 **/
bool game_is_blank(const game g, uint row, uint col);

/**
 * @brief Test if a given square is a lightbulb.
 * @param g the game
 * @param row row index
 * @param col column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p row < game height
 * @pre @p col < game width
 * @return true if the square is a lightbulb
 **/
bool game_is_lightbulb(const game g, uint row, uint col);

/**
 * @brief Test if a given square is black (whether or not it is numbered).
 * @param g the game
 * @param row row index
 * @param col column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p row < game height
 * @pre @p col < game width
 * @return true if the square is black
 **/
bool game_is_black(const game g, uint row, uint col);

/**
 * @brief Get the number of lightbulbs expected against a black wall.
 * @param g the game
 * @param row row index
 * @param col column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p row < game height
 * @pre @p col < game width
 * @return the back wall number, or -1 if it is unumbered
 **/
int game_get_black_number(const game g, uint row, uint col);

/**
 * @brief Returns the number of rows of @p g.
 *
 * @param g
 * @return uint
 */
uint game_nb_rows(game g);

/**
 * @brief Returns the number of cols of @p g.
 *
 * @param g
 * @return uint
 */
uint game_nb_cols(game g);

#endif