/*
    This file "hasegawa.c" can edit by ONLY Hasegawa.
*/

/* header files */
#include "header.h"

/* functions */
int * hasegawa_search(int * solution_path);
int * euclid_search(int * solution_path);
int * graph_search(int * solution_path);
int * two_opt_tabu(int * solution_path);
int * two_opt_only(int * solution_path);
void choice_4indexs(int type, int * cities, int * solution_path);
int permit_worse(double bef_aft_distance);
int mode_select(int mode, int * solution_path);
void create_2opt_tabulist(int tsp_size, int mode);
int next_index(int target, int maximum);
int prev_index(int target, int maximum);
int now_index(int target, int maximum);
int * mallocer_ip(int size);
double bef_aft_distance(int * cities);
void exchange_branch(int * solution_path, int * indexs);
void get_cities_by_indexs(int * cities, int * indexs, int * solution_path);
double get_worse_permit(void);
void change_worse_permit(int type);
double get_distance(int a, int ad, int b, int bd);
double get_cost(int a, int ad, int b, int bd);
double get_branch_distance(int a, int b);
double get_branch_cost(int a, int b);
double get_graph_cost(int a,int b);
double get_now_parcentage(void);
int check_parcentage(double bef_aft_distance);
void get_min_exchange_indexs(int * solution_path, int * indexs);
double make_distance(int x1, int y1, int x2, int y2);
int get_x(int city_index);
int get_y(int city_index);
int is_2opt_tabu(int * cities);
void error_procedure(char * message);
int check_manneri(int type);
void add_2opt_tabulist(int * cities);
int not_found_looping(int * cities, int * indexs, int type);
void set_middle_mannneri(int on_or_off);
#ifdef MPIMODE
int is_2opt_share_tabu(int * cities1);
#endif

int * hasegawa_search(int * solution_path)
{
    int * return_data;

    /* Search Euclid-Data */
    if(modep->euclid_mode == ON) {
        return_data = euclid_search(solution_path);
    }
    /* Search Graph-Data (non-available) */
    else if(modep->graph_mode == ON) {
        return_data = graph_search(solution_path);
    }
    /* Error */
    else {
        error_procedure("hasegawa_search()");
    }

    return return_data;
}

int * euclid_search(int * solution_path)
{
    int * return_data;
    int mode;

    mode = mode_select(mode, solution_path);

    switch(mode) {
        case DEFAULT:
            /* search by 2-opt procedure */
            return_data = two_opt_tabu(solution_path);
            break;
    }

    return return_data;
}

int * graph_search(int * solution_path)
{
    int * return_data;
    int mode;

    if(check_manneri(MIDDLEMODE) == YES) {
        set_middle_mannneri(ON);
    }

    mode = mode_select(mode, solution_path);

    switch(mode) {
        case DEFAULT:
            return_data = two_opt_only(solution_path);
            break;
        case TABU2OPT:
            return_data = two_opt_tabu(solution_path);
            break;
    }

    return return_data;
}

int mode_select(int mode, int * solution_path)
{
    if(modep->only2opt_mode == ON) {
        mode = DEFAULT;
    }
    else if(modep->tabu2opt_mode == ON) {
        mode = TABU2OPT;
    }

    return mode;
}

int * two_opt_tabu(int * solution_path)
{
    int indexs[4], cities[4];
    int loop_times = 0;

    /* (1) First, this fucn exchange branch by "2-opt" only toward better without using tabu-list */
    if(get_tabu_mode() == OFF) {
        solution_path = two_opt_only(solution_path);

        if(check_manneri(SHORTMODE) == YES) {
            set_tabu_mode(ON);
        }
    }
    /* (2) Second, permit exchange toward worse, and use tabu-list */
    else {
        if(turn_loop_times(READONLY) % 3 == 0) {
            solution_path = two_opt_only(solution_path);
        }
        else {
            do {
                choice_4indexs(DEFAULT, indexs, solution_path);
                get_cities_by_indexs(cities, indexs, solution_path);
                if(not_found_looping(cities, indexs, COUNT) == YES) {
                    not_found_looping(cities, indexs, READONLY);
                    break;
                }
#ifdef MPIMODE
            } while(permit_worse(bef_aft_distance(cities)) == NO || is_2opt_share_tabu(cities) == YES || is_2opt_tabu(cities) == YES);
#else
            } while(permit_worse(bef_aft_distance(cities)) == NO || is_2opt_tabu(cities) == YES);
#endif
            not_found_looping(cities, indexs, INIT);
            exchange_branch(solution_path, indexs);
        }
    }

    return solution_path;
}

int * two_opt_only(int * solution_path)
{
    int indexs[4];

    if(modep->graph_mode = ON) {
        get_min_exchange_indexs(solution_path, indexs);
    }
    else {
        error_procedure("two_opt_only()");
    }

    exchange_branch(solution_path, indexs);

    return solution_path;
}

void get_min_exchange_indexs(int * solution_path, int * best_indexs)
{
    int i, j, k;
    int tsp_size = solution_path[0];
    int cities[4];
    int indexs[4];
    double now_distance = DBL_MAX * (-1);
    double maximum = DBL_MAX * (-1);

    for(i = 1; i < tsp_size - 2; i++) {
        indexs[0] = i; indexs[1] = i + 1;
        for(j = i + 2; j < tsp_size; j++) {
            indexs[2] = j; indexs[3] = j + 1;
            get_cities_by_indexs(cities, indexs, solution_path);
            now_distance = bef_aft_distance(cities);
            if(now_distance > maximum) {
                maximum = now_distance;
                for(k = 0; k < 4; k++) {
                    best_indexs[k] = indexs[k];
                }
            }
        }
    }
}

void get_cities_by_indexs(int * cities, int * indexs, int * solution_path)
{
    int i;

    for(i = 0; i < 4; i++) {
        if(indexs[i] > solution_path[0]) {
            error_procedure("get_cities_by_indexs's");
        }
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

    if(modep->euclid_mode == ON) {
        before = get_distance(cities[0],cities[1],cities[2],cities[3]);
        after = get_distance(cities[0],cities[2],cities[1],cities[3]);
    }
    else if(modep->graph_mode == ON) {
        before = get_cost(cities[0],cities[1],cities[2],cities[3]);
        after = get_cost(cities[0],cities[2],cities[1],cities[3]);
    }

    set_now_parcentage(before, after);

    return (before - after);
}

/* input datas (a,ad,b,bd) are the number of index of city (Euclide) */
double get_distance(int a, int ad, int b, int bd)
{
    return (get_branch_distance(a, ad) + get_branch_distance(b, bd));
}

/* input datas (a,ad,b,bd) are the number of index of city (Graph) */
double get_cost(int a, int ad, int b, int bd)
{
    return (get_branch_cost(a, ad) + get_branch_cost(b, bd));
}

/* input datas (a,b) are the number of index of city */
double get_branch_distance(int a, int b)
{
    return make_distance(get_x(a), get_y(a), get_x(b), get_y(b));
}

/* input datas (a,b) are the number of index of city */
double get_branch_cost(int a, int b)
{
    return get_graph_cost(a,b);
}

/* permit exchange toward worse if under permit_baseline */
int permit_worse(double bef_aft_distance)
{
    int return_num = NO;

    /* if exchange is toward better, permit */
    if(bef_aft_distance > 0) {
        return_num = YES;
    }
    else{
        /* permit_worse discribed > 0 */
        if(check_parcentage(bef_aft_distance) == NO) {
            return_num = NO;
        }
        else {
            return_num = YES;
        }
    }

    return return_num;
}

void exchange_branch(int * solution_path, int * indexs)
{
    int i, count;
    int tsp_size = solution_path[0];
    int * copy;
    int cities[4];

    copy = mallocer_ip(tsp_size + 1);

    for(i = 0; i <= tsp_size; i++) {
        copy[i] = solution_path[i];
    }

    for(i = 0; i <= (count = get_among(indexs[1], indexs[2], tsp_size)); i++) {
        solution_path[now_index((indexs[2] - i), tsp_size)] = copy[now_index((indexs[1] + i), tsp_size)];
    }

    if(get_tabu_mode() == ON) {
        get_cities_by_indexs(cities, indexs, solution_path);
        add_2opt_tabulist(cities);
    }

    free(copy);
}

int get_among(int start, int end, int tsp_size)
{
    int return_num = end - start;

    if(return_num < 0) {
        return_num += tsp_size;
    }

    return return_num;
}
/* (1 <= return_num <= Max) */
int next_index(int target, int maximum)
{
    return now_index((target % maximum + 1), maximum);
}

/* (1 <= return_num <= Max) */
int prev_index(int target, int maximum)
{
    return now_index(((target == 1) ? maximum : target -1), maximum);
}

/* (1 <= return_num <= Max) */
int now_index(int target, int maximum)
{
    int return_num;

    if(target > 0) {
        return_num = (((return_num = target % maximum) == 0) ? maximum : return_num);
    }
    else {
        return_num = (((return_num = target * (-1) % maximum) == 0) ? maximum : return_num);
        if(target % maximum == 0) {
            return_num = maximum;
        }
        else {
            return_num = target % maximum + maximum;
        }
    }

    return return_num;
}
