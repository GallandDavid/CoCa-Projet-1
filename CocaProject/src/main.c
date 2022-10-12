/**
 * @file main.c
 * @author Vincent Penelle <vincent.penelle@u-bordeaux.fr>
 * @brief Program that solves lightup with several algorithms.
 * @version 1
 * @date 2022-09-20
 *
 * @copyright Creative Commons
 *
 */

#include "game.h"
#include "reduction.h"
#include "Z3Tools.h"
#include "brute_force.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <signal.h>

/*Timeout for brute force (in seconds). Update it as needed.*/
#define TIMEOUT 30

void handler(int sig)
{
    switch (sig)
    {
    case SIGALRM:
        printf("Timeout in Brute force in %d seconds!\n", TIMEOUT);
        exit(-1);
    case SIGSEGV:
        printf("Segfault in Brute force !\n");
        exit(-1);
    default:
        printf("Other error occured in Brute Force\n");
        exit(-1);
    };
}

void usage()
{
    printf("Use: solver.out [options] file\n");
    printf(" file should contain a lightup game instance -- either in explicit format or in string format exported from https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html\nThe program loads the game (detection of format is automatic), and then applies a brute force solver and/or a SAT reduction (depending on options).\nCan display the format both on the command line or as a file in explicit format");
    printf("Options: \n");
    printf(" -s         Deactivates utf8 characters in terminal display\n");
    printf(" -h         Displays this help\n");
    printf(" -v         Activate verbose mode (displays parsed empty game)\n");
    printf(" -B         Solves the problem using the brute force algorithm\n");
    printf(" -R         Solves the problem using a reduction\n");
    printf(" -F         Displays the formula computed in a file");
#ifdef SUBJECT
    printf(" (obviously not in this version, but you should really display it in your code)");
#endif
    printf(". See -o for names of the file Only active if -R is active\n");
    printf(" -u         Determines if there exists an other solution. Only for the reduction. If with -f is present, will generate a file with the solution\n");
    printf(" -q         Deactivates the printing of the solution on the standard output (only displays existence of solution)\n");
    printf(" -f         Writes the solution in file (if such a solution exists). See next option for the name. These files will be produced in the folder 'sol'\n");
    printf(" -o NAME    Writes the solution in \"NAME_Brute.ls\" or \"NAME_SAT.ls\" depending of the algorithm used and the formula in \"NAME.formul\". [if not present: \"result_SAT.ls\", \"result_Brute.ls\" and \"result.formul\"]\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
        return 0;
    }

    bool utf8 = true;
    bool verbose = false;
    bool display_terminal = true;
    bool output_file = false;
    bool print_formula = false;
    bool brute_force_s = false;
    bool reduction = false;
    bool second_sol = false;
    char *solution_name = "result";
    int option;

    while ((option = getopt(argc, argv, ":hsvFBRuqfo:")) != -1)
    {
        switch (option)
        {
        case 'h':
            usage();
            return EXIT_SUCCESS;
        case 's':
            utf8 = false;
            break;
        case 'v':
            verbose = true;
            break;
        case 'B':
            brute_force_s = true;
            break;
        case 'R':
            reduction = true;
            break;
        case 'F':
            print_formula = true;
            break;
        case 'q':
            display_terminal = false;
            break;
        case 'u':
            second_sol = true;
            break;
        case 'f':
            output_file = true;
            break;
        case 'o':
            solution_name = optarg;
            break;
        case '?':
            printf("unkown option: %c\n", optopt);
            break;
        }
    }

    if (argc - optind < 1)
    {
        printf("No argument given. Exiting.\n");
        return 0;
    }

    game g = game_load(argv[optind]);
    if (verbose)
        game_print(g, utf8);

    if (reduction)
    {
        Z3_context ctx = makeContext();
        clock_t start = clock();
        Z3_ast formula = gameFormula(ctx, g);
        clock_t end = clock();
        printf("Formula computed in %g seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);

        if (print_formula)
        {
#ifndef SUBJECT
            struct stat st = {0};
            if (stat("./sol", &st) == -1)
                mkdir("./sol", 0777);
            int length = strlen(solution_name) + 13;
            char nameFile[length];
            snprintf(nameFile, length, "sol/%s.formula", solution_name);
            FILE *file = fopen(nameFile, "w");
            fprintf(file, "%s\n", Z3_ast_to_string(ctx, formula));
            fclose(file);
            printf("Formula printed in sol/%s.formula\n", solution_name);
#else
            printf("Nah, I'm not displaying the formula in the given executable\n");
#endif
        }

        Z3_model model;
        start = clock();
        Z3_lbool isSat = solveFormula(ctx, formula, &model);
        end = clock();
        printf("Formula solved in %g seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);
        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("no solution found\n");
            break;
        case Z3_L_UNDEF:
            printf("not able to determine veracity\n");
            break;
        case Z3_L_TRUE:
            printf("Yes, there is a solution\n");
            game g_display = game_copy(g);
            applySolutionToGame(ctx, model, g_display);
            if (display_terminal)
                game_print(g_display, utf8);
            if (output_file)
            {
                int length = strlen(solution_name) + 12;
                char nameFile[length];
                snprintf(nameFile, length, "%s_Sat", solution_name);
                game_print_to_file(g_display, nameFile);
                printf("Solution printed in sol/%s.ls.\n", nameFile);
            }
            if (second_sol)
            {
                Z3_model second_model;
                Z3_ast other_formula = other_sol_formula(ctx, g, formula, model);
                start = clock();
                Z3_lbool isSat2 = solveFormula(ctx, other_formula, &second_model);
                end = clock();
                printf("Formula for determining other solution solved in %g seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);
                switch (isSat2)
                {
                case Z3_L_FALSE:
                    printf("The first solution is unique.\n");
                    break;
                case Z3_L_UNDEF:
                    printf("not able to determine if there exists another solution.\n");
                    break;
                case Z3_L_TRUE:
                    printf("There exists another solution.\n");
                    game_reset(g_display);
                    applySolutionToGame(ctx, second_model, g_display);
                    if (display_terminal)
                        game_print(g_display, utf8);
                    if (output_file)
                    {
                        int length = strlen(solution_name) + 12;
                        char nameFile[length];
                        snprintf(nameFile, length, "%s_Sat2", solution_name);
                        game_print_to_file(g_display, nameFile);
                        printf("Solution printed in sol/%s.ls.\n", nameFile);
                    }
                }
                game_delete(g_display);
            }
        }
        Z3_del_context(ctx);
    }

    if (brute_force_s)
    {
        struct sigaction sa;

        sa.sa_handler = handler;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGALRM, &sa, NULL);
        alarm(TIMEOUT);
        clock_t start = clock();
        bool res = brute_force(g);
        clock_t end = clock();
        if (res)
        {
            printf("Brute Force successful in %g seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
            game_print(g, utf8);
        }
        else
        {
            printf("Brute Force found no solution in %g seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
        }
    }
    game_delete(g);
}