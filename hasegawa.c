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
int permit_worse(int * cities, int tsp_size);
int mode_select(int mode, int * solution_path);
void create_2opt_tabulist(int tsp_size);
int next_index(int target, int maximum);
int prev_index(int target, int maximum);
int * mallocer_ip(int size);

int is_2opt_tabu(int * cities);
void error_procedure(char * message);

/* grobal variable */
int * point_of_cities;

int * hasegawa_search(int * solution_path)
{
    int * return_data;

    /* Search Euclid-Data */
    if(modep->euclid_mode == ON) {
        return_data = euclid_search(solution_path);
    }
    /* Search Graph-Data (non-available) */
    else if(modep->graph_mode == ON) {
        error_procedure("hasegawa_search() non-available");
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
            /* create tabu list for 2-opt */
            create_2opt_tabulist(solution_path[0]);

            /* search by 2-opt procedure */
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

    do {
        do {
            /* only choice the 4 cities by random */
            choice_4indexs(DEFAULT, cities, solution_path);

            if(permit_worse(cities, solution_path[0]) == YES) {
                choice_4indexs(PERMITWORSE, cities, solution_path);
            }
        /* check the 4 cities are tabu or not */
        } while(is_2opt_tabu(cities));
    /* because of diciding which search toward "permit worse" or "better only", check the distance */
    } while(permit_worse(cities, solution_path[0]) == NO);

    return solution_path;
}

void choice_4indexs(int type, int * return_data, int * solution_path)
{
    int a,b;
    int max = solution_path[0];

    /* 'type-Default' means just-randomly choice */
    if(type == DEFAULT) {
        a = random_num(max);

        do {
            b = random_num(max);
        } while(b == prev_index(a, max) || b == a || b == next_index(a, max));

        return_data[0] = a; return_data[1] = next_index(a, max);
        return_data[2] = b; return_data[3] = next_index(b, max);
    }
    if(type == PERMITWORSE) {
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

int permit_worse(int * cities, int tsp_size)
{
    int i;
    int return_num = YES;
    int a = cities[0], ad = cities[1];
    int b = cities[2], bd = cities[3];

    /* first procedure */
    if(turn_loop_times(READONLY) == 0 && search_loop_times(READONLY) == 0) {
        point_of_cities = mallocer_ip(tsp_size);
        point_of_cities[0] = tsp_size;
    }
    /* if now is first turn, set the "city point" */
    if(turn_loop_times(READONLY) == 0) {
        set_city_point(point_of_cities);
    }

    /* decrement 1 point at the "point_of_cities" */
    for(i = 1; i <= tsp_size; i++) {
        if(i == a || i == ad || i == b || i == bd) {
            point_of_cities[i]--;
        }
        if(point_of_cities[i] == 0) {
            return_num = NO;
        }
    }

    return return_num;
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
