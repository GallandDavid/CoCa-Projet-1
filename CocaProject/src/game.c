#include "game.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

struct game_s
{
    uint nb_rows;    /**< number of rows in the game */
    uint nb_cols;    /**< number of columns in the game */
    square *squares; /**< the grid of squares */
};

#define INDEX(g, i, j) ((i) * (g->nb_cols) + (j))
#define SQUARE(g, i, j) ((g)->squares[(INDEX(g, i, j))])

static int value[255] = {['b'] = BLANK, ['0'] = BLACK0, ['1'] = BLACK1, ['2'] = BLACK2, ['3'] = BLACK3, ['4'] = BLACK4, ['w'] = BLACKU, ['*'] = LIGHTBULB};

int _str2square(char c)
{
    unsigned char uc = c;
    return value[uc] == 0 ? 0 : value[uc];
}

static char image[255] = {
    [BLANK] = ' ', [BLACK0] = '0', '1', '2', '3', '4', [BLACKU] = 'w', [LIGHTBULB] = '*'};

char _square2str(square s)
{
    return image[s];
}

uint game_nb_rows(game g) { return g->nb_rows; }

/* ************************************************************************** */

uint game_nb_cols(game g) { return g->nb_cols; }

void game_delete(game g)
{
    free(g->squares);
    free(g);
}

bool is_lighted(game g, uint row, uint col)
{
    for (uint a = row; true; a--)
    {
        if (game_is_lightbulb(g, a, col))
            return true;
        if (game_is_black(g, a, col))
            break;
        if (a == 0)
            break;
    }
    for (uint a = row; a < game_nb_rows(g); a++)
    {
        if (game_is_lightbulb(g, a, col))
            return true;
        if (game_is_black(g, a, col))
            break;
    }
    for (uint a = col; true; a--)
    {
        if (game_is_lightbulb(g, row, a))
            return true;
        if (game_is_black(g, row, a))
            break;
        if (a == 0)
            break;
    }
    for (uint a = col; a < game_nb_cols(g); a++)
    {
        if (game_is_lightbulb(g, row, a))
            return true;
        if (game_is_black(g, row, a))
            break;
    }
    return false;
}

void game_print(const game g, bool utf8)
{
    assert(g);
    int cols = game_nb_cols(g);
    int digit = 1;
    while (cols > 10)
    {
        digit *= 10;
        cols /= 10;
    }
    while (digit > 0)
    {
        printf("    ");
        for (uint col = 0; col < game_nb_cols(g); col++)
            printf("%d", (col / digit) % 10);
        printf("\n");
        digit /= 10;
    }
    printf("   ┌");
    for (uint col = 0; col < game_nb_cols(g); col++)
        printf("─");
    printf("┐\n");
    for (uint row = 0; row < game_nb_rows(g); row++)
    {
        printf("%2d │", row);
        for (uint col = 0; col < game_nb_cols(g); col++)
        {
            square s = game_get_square(g, row, col);
            char c = _square2str(s);
            if (s == BLANK && is_lighted(g, row, col))
                if (utf8)
                    printf("░");
                else
                    printf(".");
            else if (utf8 && c == '*')
                printf("☀");
            else if (utf8 && c == 'w')
                printf("█");
            else
                printf("%c", c);
        }
        printf("│\n");
    }
    printf("   └");
    for (uint col = 0; col < game_nb_cols(g); col++)
        printf("─");
    printf("┘\n");
}

void game_print_to_file(game g, char *filename)
{
    assert(filename);
    FILE *file;

    struct stat st = {0};
    if (stat("./sol", &st) == -1)
        mkdir("./sol", 0777);

    if (filename == NULL)
    {
        char nameFile[30];
        snprintf(nameFile, 30, "sol/result.ls");
        file = fopen(nameFile, "w");
    }
    else
    {
        int length = strlen(filename) + 12;
        char nameFile[length];
        snprintf(nameFile, length, "sol/%s.ls", filename);
        file = fopen(nameFile, "w");
    }
    assert(file);
    fprintf(file, "%d %d\n", game_nb_rows(g), game_nb_cols(g));
    for (uint row = 0; row < game_nb_rows(g); row++)
    {
        for (uint col = 0; col < game_nb_cols(g); col++)
        {
            square s = game_get_square(g, row, col);
            char c = _square2str(s);
            if (s == BLANK && is_lighted(g, row, col))
                c = '.';
            fprintf(file, "%c", c);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static game game_load_tatham(FILE *file)
{
    assert(file);
    int nb_rows = 0, nb_cols = 0;
    char descr[10000];
    char line[10000];
    fgets(line, 10000, file);
    int res = sscanf(line, "%dx%d:%s\n", &nb_cols, &nb_rows, descr);
    assert(res == 3);
    game g = string_to_game(descr, nb_rows, nb_cols);
    return g;
}

static game game_load_stream(FILE *file)
{
    assert(file);
    char c;
    // header
    uint nb_rows = 0, nb_cols = 0;
    int res = fscanf(file, "%u %u\n", &nb_rows, &nb_cols);
    assert(res == 2);
    game g = game_new_empty(nb_rows, nb_cols);

    // game grid
    for (uint row = 0; row < nb_rows; row++)
    {
        for (uint col = 0; col < nb_cols; col++)
        {
            res = fscanf(file, "%c", &c);
            assert(res == 1);
            int val = _str2square(c);
            assert(val != -1);
            square s = (square)val;
            game_set_square(g, row, col, s);
        }
        fgetc(file); // read /n
    }
    return g;
}

void game_reset(game g)
{
    for (uint row = 0; row < g->nb_rows; row++)
    {
        for (uint col = 0; col < g->nb_cols; col++)
        {
            if (game_is_lightbulb(g, row, col))
                game_set_square(g, row, col, BLANK);
        }
    }
}

game game_copy(game g)
{
    game result = game_new_empty(g->nb_rows, g->nb_cols);
    for (int row = 0; row < g->nb_rows; row++)
    {
        for (int col = 0; col < g->nb_cols; col++)
        {
            game_set_square(result, row, col, game_get_square(g, row, col));
        }
    }
    return result;
}

/* ************************************************************************** */

game game_load(char *filename)
{
    assert(filename);
    FILE *file = fopen(filename, "r");
    assert(file);
    game g;
    int toto;
    int res = fscanf(file, "%u %u\n", &toto, &toto);
    fclose(file);
    file = fopen(filename, "r");
    if (res == 2)
        g = game_load_stream(file);
    else
        g = game_load_tatham(file);
    fclose(file);
    return g;
}

game string_to_game(char *description, uint nb_rows, uint nb_cols)
{
    game g = game_new_empty(nb_rows, nb_cols);
    uint pos = 0;
    uint index = 0;
    while (pos < nb_rows * nb_cols)
    {
        if ('a' <= description[index] && description[index] <= 'z')
            pos = pos + (description[index] - 'a' + 1);
        if (description[index] == 'B')
            g->squares[pos++] = BLACKU;
        if (description[index] == '0')
            g->squares[pos++] = BLACK0;
        if (description[index] == '1')
            g->squares[pos++] = BLACK1;
        if (description[index] == '2')
            g->squares[pos++] = BLACK2;
        if (description[index] == '3')
            g->squares[pos++] = BLACK3;
        if (description[index] == '4')
            g->squares[pos++] = BLACK4;
        index++;
    }
    return g;
}

game game_new_empty(uint nb_rows, uint nb_cols)
{
    game g = (game)malloc(sizeof(struct game_s));
    assert(g);
    g->nb_rows = nb_rows;
    g->nb_cols = nb_cols;
    g->squares = (square *)calloc(g->nb_rows * g->nb_cols, sizeof(uint));
    assert(g->squares);
    return g;
}

/* ************************************************************************** */

void game_set_square(game g, uint row, uint col, square s)
{
    assert(g);
    assert(row < g->nb_rows);
    assert(col < g->nb_cols);
    SQUARE(g, row, col) = s;
}

/* ************************************************************************** */

square game_get_square(const game g, uint row, uint col)
{
    assert(g);
    assert(row < g->nb_rows);
    assert(col < g->nb_cols);
    return SQUARE(g, row, col);
}

bool game_is_blank(const game g, uint row, uint col)
{
    assert(g);
    assert(row < g->nb_rows);
    assert(col < g->nb_cols);
    square s = SQUARE(g, row, col);
    if (s == BLANK)
        return true;
    return false;
}

/* ************************************************************************** */

bool game_is_black(const game g, uint row, uint col)
{
    assert(g);
    assert(row < g->nb_rows);
    assert(col < g->nb_cols);
    square s = SQUARE(g, row, col);
    if (s & BLACK0)
        return true;
    return false;
}

/* ************************************************************************** */

bool game_is_lightbulb(const game g, uint row, uint col)
{
    assert(g);
    assert(row < g->nb_rows);
    assert(col < g->nb_cols);
    square s = SQUARE(g, row, col);
    if (s == LIGHTBULB)
        return true;
    return false;
}

/* ************************************************************************** */

int game_get_black_number(const game g, uint row, uint col)
{
    assert(g);
    assert(row < g->nb_rows);
    assert(col < g->nb_cols);
    square s = SQUARE(g, row, col);
    assert(s & BLACK0);
    if (s == BLACKU)
        return -1;
    else
        return (s - BLACK0);
}