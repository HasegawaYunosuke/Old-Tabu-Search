/*
    This file "hasegawa.c" can edit by ONLY Hasegawa.
*/

/* header files */
#include "header.h"

/* functions */
int * get_solution_path(void);
int random_num(int maximum);
int * hasegawa_search(int * solution_path);
int * graph_search(int * solution_path);
int * two_opt_tabu(int * solution_path);
int * two_opt_only(int * solution_path);
void choice_4indexs(int type, int * cities, int * solution_path);
int permit_worse(double bef_aft_distance);
int mode_select(int mode, int * solution_path);
void create_2opt_tabulist(int tsp_size, int mode);
int next_index(int target, int maximum);
int prev_index(int target, int maximum);
int next_city(int target, int maximum);
int prev_city(int target, int maximum);
int now_index(int target, int maximum);
int * mallocer_ip(int size);
double bef_aft_distance(int * cities);
void exchange_branch(int * solution_path, int * indexs);
void get_cities_by_indexs(int * cities, int * indexs, int * solution_path);
int get_index_by_city(int city, int * solution_path);
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
int get_smart_random_city(int maximum);
int get_never_visited_city(int choiced_city);
int get_smart_city(int choiced_city);
int get_num_of_all_proc(void);
int is_2opt_share_tabu(int * cities1);
int get_group_reader(void);
int get_process_number(void);
void add_MPI_same_group_tabulist(int add_mode, int * add_data);
#endif

int * hasegawa_search(int * solution_path)
{
    int * return_data;

    return_data = graph_search(solution_path);

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
    int two_opt_probability = 2;

    #ifdef MPIMODE
        #ifdef NONLEADER_NOT_USE_TWOOPTONLY
        if(get_group_reader() != get_process_number()) {
            set_tabu_mode(ON);
        }
        #endif
        #ifdef LEADER_NOT_USE_TWOOPTONLY
        if(get_group_reader() == get_process_number()) {
            set_tabu_mode(ON);
        }
        #endif
        #ifdef BOTH_NOT_USE_TWOOPTONLY
        set_tabu_mode(ON);
        #endif
    #endif

    /* (1) First, this fucn exchange branch by "2-opt" only toward better without using tabu-list */
    if(get_tabu_mode() == OFF) {
        solution_path = two_opt_only(solution_path);
        if(check_manneri(SHORTMODE) == YES) {
            set_tabu_mode(ON);
        }
    }
    /* (2) Second, permit exchange toward worse, and use tabu-list */
    else {
#ifdef MPIMODE
        if(get_group_reader() != get_process_number()) {
            //two_opt_probability = (get_process_number() + 1) % 3 + 2;
            two_opt_probability = 2;
        }
        if(turn_loop_times(READONLY) % two_opt_probability != 0) {
#else
        if(turn_loop_times(READONLY) % 2 != 0) {
#endif
            solution_path = two_opt_only(solution_path);
        }
        else {
            do {
#ifdef MPIMODE
                if(get_group_reader() != get_process_number()) {
                    //if(turn_loop_times(READONLY) % 2 != 0) {
                        //choice_4indexs(SAMEGROUP_TABULIST_SMART_CHOICE, indexs, solution_path);
                        //choice_4indexs(DEFAULT, indexs, solution_path);
                        choice_4indexs(CHOICE_NEVER_CHOICED, indexs, solution_path);
                    /*else {
                        choice_4indexs(DEFAULT, indexs, solution_path);
                    }*/
                }
                else {
                    choice_4indexs(DEFAULT, indexs, solution_path);
                }
#else
                choice_4indexs(DEFAULT, indexs, solution_path);
#endif
                get_cities_by_indexs(cities, indexs, solution_path);
                if(not_found_looping(cities, indexs, COUNT) == YES) {
                    not_found_looping(cities, indexs, READONLY);
                    break;
                }
#ifdef MPIMODE
            } while(permit_worse(bef_aft_distance(cities)) == NO || is_2opt_tabu(cities) == YES || is_2opt_share_tabu(cities) == YES);
#else
            } while(permit_worse(bef_aft_distance(cities)) == NO || is_2opt_tabu(cities) == YES);
#endif
            not_found_looping(cities, indexs, INIT);
            #ifdef MPIMODE
                #ifdef SAMEGROUP_COMUNICATION
                add_MPI_same_group_tabulist(FOUR_CITIES, cities);
                #endif
            #endif
            exchange_branch(solution_path, indexs);
        }
    }

    return solution_path;
}

int * two_opt_only(int * solution_path)
{
    int indexs[4];

    get_min_exchange_indexs(solution_path, indexs);
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
            fprintf(stderr,"!#! %d,%d,%d,%d#!#!\n", indexs[0], indexs[1], indexs[2], indexs[3]);
            error_procedure("get_cities_by_indexs's");
        }
        cities[i] = solution_path[indexs[i]];
    }
}

int get_index_by_city(int city, int * solution_path)
{
    int tsp_size = solution_path[0];
    int i, return_num = 0;

    for(i = 1; i <= tsp_size; i++) {
        if(solution_path[i] == city) {
            return_num = i;
            break;
        }
    }

    if(return_num == 0) {
        error_procedure("get_index_by_city()");
    }

    return return_num;
}

void choice_4indexs(int type, int * return_data, int * solution_path)
{
    int a,b;
    int a_city, b_city;
    int max = solution_path[0];
    int possibility;

    /* 'type-Default' means just-randomly choice */
    if(type == DEFAULT) {
        a = random_num(max);
        do {
            b = random_num(max);
        } while(b == prev_index(a, max) || b == a || b == next_index(a, max));

        return_data[0] = a; return_data[1] = next_index(a, max);
        return_data[2] = b; return_data[3] = next_index(b, max);
    }
    else if(type == SAMEGROUP_TABULIST_SMART_CHOICE) {
        //a_city = get_smart_random_city(max);
        a_city = random_num(max);
        if((b_city = get_smart_city(a_city)) < 0) {
            choice_4indexs(DEFAULT, return_data, solution_path);
        }
        else {
            a = get_index_by_city(a_city, solution_path);
            b = get_index_by_city(b_city, solution_path);
            return_data[0] = a; return_data[1] = next_index(a, max);
            return_data[2] = b; return_data[3] = next_index(b, max);
        }
    }
    else if(type == CHOICE_NEVER_CHOICED) {
        /* possibility is, if proc_num (1, 2, 3,...,N), N+1, N, N -1,..., 2) */
        possibility = get_num_of_all_proc() - get_process_number() + 1;
        /* It's DEFAULT */
        if((random_num(max) % possibility) != 0) {
            choice_4indexs(DEFAULT, return_data, solution_path);
        }
        /* It's the mode of "CHOICCE_NEVER_CHOICED" */
        else {
            do {
                b_city = NO; a_city = random_num(max);
                b_city = get_never_visited_city(a_city);

            } while(b_city == NO);

            /* DEL ST */
            error_procedure("CHOICE_NEVER_CHOICED");
            /* DEL EN */

            a = get_index_by_city(a_city, solution_path);
            b = get_index_by_city(b_city, solution_path);
            return_data[0] = a; return_data[1] = next_index(a, max);
            return_data[2] = b; return_data[3] = next_index(b, max);
        }
    }
    else if(type == PERMITWORSE) {
        error_procedure("choice_4indexs()");
    }
    else {
        error_procedure("choice_4indexs()");
    }
}

/* return "YES" or "NO" (Decrease -> YES) */
double bef_aft_distance(int * cities)
{
    double before, after;

    before = get_cost(cities[0],cities[1],cities[2],cities[3]);
    after = get_cost(cities[0],cities[2],cities[1],cities[3]);

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

int next_city(int target, int maximum)
{
    int * sol_path = get_solution_path();

    return sol_path[next_index(get_index_by_city(target, sol_path), maximum)];
}

int prev_city(int target, int maximum)
{
    int * sol_path = get_solution_path();

    return sol_path[prev_index(get_index_by_city(target, sol_path), maximum)];
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
