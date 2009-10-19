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
int now_index(int target, int maximum);
int * mallocer_ip(int size);
double bef_aft_distance(int * cities);
void exchange_branch(int * solution_path, int * indexs);
void get_cities_by_indexs(int * cities, int * indexs, int * solution_path);
double get_distance(int a, int ad, int b, int bd);
double get_branch_distance(int a, int b);
double make_distance(int x1, int y1, int x2, int y2);
int get_x(int city_index);
int get_y(int city_index);

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
    int indexs[4], cities[4];
    int test;

    /* (1) First, this fucn exchange branch by "2-opt" only toward better without using tabu-list */
    do {
        /* choice 4 indexs by randome */
        choice_4indexs(DEFAULT, indexs, solution_path);
        /* convert "indexs" -> "cities" */
        get_cities_by_indexs(cities, indexs, solution_path);
    } while(bef_aft_distance(cities) < 0);

    /* if it can't find "decrease-exchange", shift "tabu-search-exchange" */

    /* (2) Second, permit exchange toward worse, and use tabu-list */
    /*do {
        do {
            choice_4indexs(DEFAULT, cities, solution_path);
        } while(permit_judgement(cities) == NO);
    } while ();*/
//    is_2opt_tabu(cities);
    /* because of diciding which search toward "permit worse" or "better only", check the distance */

    exchange_branch(solution_path, indexs);

    return solution_path;
}

void get_cities_by_indexs(int * cities, int * indexs, int * solution_path)
{
    int i;

    for(i = 0; i < 4; i++) {
        cities[i] = solution_path[indexs[i]];
    }
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
    else if(type == PERMITWORSE) {
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

/* return "YES" or "NO" (Decrease -> YES) */
double bef_aft_distance(int * cities)
{
    double before, after;

    before = get_distance(cities[0],cities[1],cities[2],cities[3]);
    after = get_distance(cities[0],cities[2],cities[1],cities[3]);

    return (before - after);
}

/* input datas (a,ad,b,bd) are the number of index of city */
double get_distance(int a, int ad, int b, int bd)
{
    return (get_branch_distance(a, ad) + get_branch_distance(b, bd));
}

/* input datas (a,b) are the number of index of city */
double get_branch_distance(int a, int b)
{
    return make_distance(get_x(a), get_y(a), get_x(b), get_y(b));
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

void exchange_branch(int * solution_path, int * indexs)
{
    int i, count;
    int tsp_size = solution_path[0];
    int * copy;

    copy = mallocer_ip(tsp_size + 1);

    for(i = 0; i <= tsp_size; i++) {
        copy[i] = solution_path[i];
    }

    for(i = 0; i < (count = get_among(indexs[1], indexs[2], tsp_size)); i++) {
        solution_path[now_index((indexs[1] + i), tsp_size)] = copy[now_index((indexs[2] - i), tsp_size)];
    }

    free(copy);
}

int get_among(int start, int end, int tsp_size)
{
    int count = 0;
    int now = start;
    int next;

    for(;;) {
        next = next_index(now, tsp_size);
        count++;
        if(next == end) {
            break;
        }
        now = next;
    }

    return count;
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

/* (1 <= return_num <= Max) */
int now_index(int target, int maximum)
{
    int return_num;

    if(target > 0) {
        return_num = (((return_num = target % maximum) == 0) ? maximum : return_num);
    }
    else {
        return_num = (((return_num = target % maximum) == 0) ? maximum : return_num * (-1));
    }

    return return_num;
}
