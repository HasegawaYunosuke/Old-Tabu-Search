/*
    This file "hasegawa.c" can edit by ONLY Hasegawa.
*/

/* header files */
#include "header.h"

/* functions */
int * hasegawa_search(int * solution_path);
int * euclid_search(int * solution_path);
int * two_opt_search(int * solution_path);
void choice_4indexs(int type, int * cities, int * solution_path);
int mode_select(int mode, int * solution_path);
int next_index(int target, int maximum);
int prev_index(int target, int maximum);

int is_2opt_tabu(int * cities);
void error_procedure(char * message);

int * hasegawa_search(int * solution_path)
{
    int i;
    int * return_data;

    /* Search Euclid-Data */
    if(modep->euclid_mode == ON) {
        return_data = euclid_search(solution_path);
    }
    /* Search Graph-Data (non-available) */
    else if(modep->graph_mode == ON) {
        printf("hasegawa_search()'s Graph-Mode is now non-available.(in 'hasegawa.c' file)\n");
        printf("This program is terminated by exit(0);\n");
        exit(0);
    }
    /* Error */
    else {
        error_procedure("hasegawa_search()");
    }

    return solution_path;
}

int * euclid_search(int * solution_path)
{
    int * return_data;
    int mode;

    mode = mode_select(mode, solution_path);

    switch(mode) {
        case DEFAULT:
            return_data = two_opt_search(solution_path);
            break;
    }

    return return_data;
}

int mode_select(int mode, int * solution_path)
{
    mode = DEFAULT;

    return mode;
}

int * two_opt_search(int * solution_path)
{
    int cities[4];
    int i;

    //do {
        choice_4indexs(DEFAULT, cities, solution_path);
    //} while(is_2opt_tabu(cities));

    return solution_path;
}

void choice_4indexs(int type, int * return_data, int * solution_path)
{
    int a,b;
    int max = solution_path[0];

    if(type == DEFAULT) {
        a = random_num(max);

        do {
            b = random_num(max);
        } while(b == prev_index(a, max) || b == a || b == next_index(a, max));

        return_data[0] = a; return_data[1] = next_index(a, max);
        return_data[2] = b; return_data[3] = next_index(b, max);
    }
    else {
        error_procedure("choice_4indexs()");
    }
}

/* (1 <= return_num <= Max) */
int next_index(int target, int maximum)
{
    return (target % maximum + 1);
}

/* (1 <= return_num <= Max) */
int prev_index(int target, int maximum)
{
    return ((target == 1) ? maximum : target -1);
}
