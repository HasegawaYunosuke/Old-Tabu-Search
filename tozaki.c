/*
    This file "tozaki.c" can edit by ONLY Tozaki.
*/

/* header files */
#include "header.h"
#define _GUN_SOURCE
#include <sched.h>
#ifdef LINUXUSER
#include <linux/unistd.h>
#endif
#define THREAD_NUM 2
#define TOTAL_SOLUTION_NUM 4
#define CPU_ZERO
#define CPU_SET

/* functions */
int * tozaki_search(int * solution_path);
int * copy_euclid_search(int * solution_path);
int * copy_graph_search(int * solution_path);
int * copy_two_opt_tabu(int * solution_path);
int * copy_two_opt_only(int * solution_path);
void copy_choice_4indexs(int type, int * cities, int * solution_path);
int copy_permit_worse(double bef_aft_distance, double * worse);
int copy_mode_select(int mode, int * solution_path);
void create_2opt_tabulist(int tsp_size, int mode);
int copy_next_index(int target, int maximum);
int copy_prev_index(int target, int maximum);
int copy_now_index(int target, int maximum);
int * mallocer_ip(int size);
double copy_bef_aft_distance(int * cities, double * worse);
void copy_exchange_branch(int * solution_path, int * indexs);
void copy_get_cities_by_indexs(int * cities, int * indexs, int * solution_path);
double get_worse_permit(void);
double copy_get_distance(int a, int ad, int b, int bd);
double copy_get_cost(int a, int ad, int b, int bd);
double copy_get_branch_distance(int a, int b);
double copy_get_branch_cost(int a, int b);
double get_graph_cost(int a,int b);
double get_now_parcentage(void);
void copy_get_min_exchange_indexs(int * solution_path, int * indexs);
double make_distance(int x1, int y1, int x2, int y2);
int get_x(int city_index);
int get_y(int city_index);
int is_2opt_tabu(int * cities);
void error_procedure(char * message);
int check_manneri(int type);
void add_2opt_tabulist(int * cities);
void thread_core_assigned(void * arg);
void thread_two_opt_tabu(int * solution_path);
int * get_solution_path();
int get_tsp_size(void);
void copy_set_now_parcentage(double before, double after, double * worse);
int copy_check_parcentage(double bef_aft_distance, double * worse);

/* grobal variable */
int grobal_indexs[4];
double best_bef_aft_cost;
int thread_loop_counter;
pthread_mutex_t parallel_mutex;

typedef struct _thread_arg {
    int core_num;
    int thread_no;
    int * path;
} thread_arg_t;

int * tozaki_search(int * solution_path)
{
    int * return_data;

    return_data = copy_graph_search(solution_path);

    return return_data;
}

int * copy_euclid_search(int * solution_path)
{
    int * return_data;
    int mode;

    mode = copy_mode_select(mode, solution_path);

    switch(mode) {
        case DEFAULT:
            /* search by 2-opt procedure */
            return_data = copy_two_opt_tabu(solution_path);
            break;
    }
    return return_data;
}

int * copy_graph_search(int * solution_path)
{
    int * return_data;
    int mode;

    mode = copy_mode_select(mode, solution_path);

    switch(mode) {
        case DEFAULT:
            return_data = copy_two_opt_only(solution_path);
            break;
        case TABU2OPT:
            return_data = copy_two_opt_tabu(solution_path);
            break;
    }
    return return_data;
}

int copy_mode_select(int mode, int * solution_path)
{
    if(modep->only2opt_mode == ON) {
        mode = DEFAULT;
    }
    else if(modep->tabu2opt_mode == ON) {
        mode = TABU2OPT;
    }
    return mode;
}

int * copy_two_opt_tabu(int * solution_path)
{
    int loop_times = 0;
    int tsp_size = solution_path[0];
    int cpu_num;
    int i, j;

    /* (1) First, this fucn exchange branch by "2-opt" only toward better without using tabu-list */
    if(get_tabu_mode() == OFF) {
        solution_path = copy_two_opt_only(solution_path);

        if(check_manneri(SHORTMODE) == YES) {
            set_tabu_mode(ON);
        }
    }
    /* (2) Second, permit exchange toward worse, and use tabu-list */
    else {
        if(turn_loop_times(READONLY) % 3 == 0) {
            solution_path = copy_two_opt_only(solution_path);
        }
        else {
            cpu_num = sysconf(_SC_NPROCESSORS_CONF);

			pthread_t parallel_thread[THREAD_NUM];
			thread_arg_t targ[THREAD_NUM];

            /* mutex init */
            pthread_mutex_init(&parallel_mutex, NULL);
            best_bef_aft_cost = DBL_MAX * (-1);
            thread_loop_counter = 0;
  
            /* go to thread_two_opt_tabu() */
            for(i = 0; i < THREAD_NUM; i++) {
                targ[i].core_num = i % cpu_num;
                targ[i].thread_no = i;
                targ[i].path = mallocer_ip(tsp_size + 1);
                for(j = 0; j < tsp_size + 1; j++) {
                    targ[i].path[j] = solution_path[j];
                }
                pthread_create(&parallel_thread[i],
                               NULL,
                               (void *) thread_core_assigned,
                               (void *) &targ[i]);
            }
            /* wait to end thread_two_opt_tabu() */
            for(i = 0; i < THREAD_NUM; i++) {
                pthread_join(parallel_thread[i], NULL);
            }
            for(i = 0; i < THREAD_NUM; i++) {
                free(targ[i].path);
            }
            /* mutex destroy */
            pthread_mutex_destroy(&mutex);

            copy_exchange_branch(solution_path, grobal_indexs);
        }
    }
    return solution_path;
}

void thread_core_assigned(void * arg)
{
    #ifdef LINUXUSER
    cpu_set_t mask;
    /* change void to thread_arg_t */
    thread_arg_t * targ = (thread_arg_t *)arg;
    int tsp_size = targ->path[0];

    CPU_ZERO(&mask);

    if(sched_getaffinity(syscall(__NR_gettid), sizeof(mask), &mask) == -1) {
        error_procedure("sched_getaffnity()");
    }

    CPU_SET(targ->core_num, &mask);

    if(sched_setaffinity(syscall(__NR_gettid), sizeof(mask), &mask) == -1) {
        error_procedure("sched_setaffnity()");
    }

    thread_two_opt_tabu(targ->path);
    #endif
}

void thread_two_opt_tabu(int * solution_path)
{
    int i;
    int indexs[4];
    int cities[4];
    double bef_aft_cost;
    double worse;

    do {
        copy_choice_4indexs(DEFAULT, indexs, solution_path);
        copy_get_cities_by_indexs(cities, indexs, solution_path);
        
        if(not_found_looping(cities, indexs, COUNT) == YES) {
            not_found_looping(cities, indexs, READONLY);
            break;
        }
    } while(copy_permit_worse(bef_aft_cost = copy_bef_aft_distance(cities, &worse), &worse) == NO || is_2opt_tabu(cities) == YES);
    
    not_found_looping(cities, indexs, INIT);

    if(get_tabu_mode() == ON) {
        pthread_mutex_lock(&parallel_mutex);
    }

    if(best_bef_aft_cost < bef_aft_cost) {
        best_bef_aft_cost = bef_aft_cost;
        for(i = 0; i < 4; i++) {
            grobal_indexs[i] = indexs[i];
        }
    }

    thread_loop_counter++;

    if(get_tabu_mode() == ON) {
        pthread_mutex_unlock(&parallel_mutex);
    }
    if(thread_loop_counter < TOTAL_SOLUTION_NUM) {
        thread_two_opt_tabu(solution_path);
    }
}

/* permit exchange toward worse if under permit_baseline */
int copy_permit_worse(double bef_aft_distance, double * worse)
{
    int return_num = NO;

    /* if exchange is toward better, permit */
    if(bef_aft_distance > 0) {
        return_num = YES;
    }
    else{
        /* permit_worse discribed > 0 */
        if(check_parcentage(bef_aft_distance, worse) == NO) {
            return_num = NO;
        }
        else {
            return_num = YES;
        }
    }
    return return_num;
}

void copy_choice_4indexs(int type, int * return_data, int * solution_path)
{
    int a,b;
    int max = solution_path[0];

    /* 'type-Default' means just-randomly choice */
    if(type == DEFAULT) {
        a = random_num(max);

        do {
            b = random_num(max);
        } while(b == copy_prev_index(a, max) || b == a || b == copy_next_index(a, max));

        return_data[0] = a; return_data[1] = copy_next_index(a, max);
        return_data[2] = b; return_data[3] = copy_next_index(b, max);
    }
    else if(type == PERMITWORSE) {
        a = random_num(max);

        do {
            b = random_num(max);
        } while(b == copy_prev_index(a, max) || b == a || b == copy_next_index(a, max));

        return_data[0] = a; return_data[1] = copy_next_index(a, max);
        return_data[2] = b; return_data[3] = copy_next_index(b, max);
    }
    else {
        error_procedure("copy_choice_4indexs()");
    }
}

int * copy_two_opt_only(int * solution_path)
{
    int indexs[4];

    copy_get_min_exchange_indexs(solution_path, indexs);

    copy_exchange_branch(solution_path, indexs);

    return solution_path;
}

void copy_get_min_exchange_indexs(int * solution_path, int * best_indexs)
{
    int i, j, k;
    int tsp_size = solution_path[0];
    int cities[4];
    int indexs[4];
    double now_distance = DBL_MAX * (-1);
    double maximum = DBL_MAX * (-1);
    double worse;

    for(i = 1; i < tsp_size - 2; i++) {
        indexs[0] = i; indexs[1] = i + 1;
        for(j = i + 2; j < tsp_size; j++) {
            indexs[2] = j; indexs[3] = j + 1;
            copy_get_cities_by_indexs(cities, indexs, solution_path);
            now_distance = copy_bef_aft_distance(cities, &worse);
            if(maximum < now_distance) {
                maximum = now_distance;
                for(k = 0; k < 4; k++) {
                    best_indexs[k] = indexs[k];
                }
            }
        }
    }
}

void copy_get_cities_by_indexs(int * cities, int * indexs, int * solution_path)
{
    int i;

    for(i = 0; i < 4; i++) {
        cities[i] = solution_path[indexs[i]];
    }
}

void copy_set_now_parcentage(double before, double after, double * worse)
{
    double best_cost = 0;

    best_cost = get_best_cost();

    /* permit_worse > 0 */
    * worse = (after - before) / best_cost * 100;
}

/* return num is YES or NO */
int copy_check_parcentage(double bef_aft_distance, double * worse)
{
    int return_num = NO;
    double best_cost = get_best_cost();
    double after_all_cost;

    after_all_cost = get_all_cost_by_graph(get_solution_path()) - bef_aft_distance;

    if(((get_worse_permit() / 100 + 1) * best_cost) > after_all_cost) {
        return_num = YES;
    }

    return return_num;
}

/* return "YES" or "NO" (Decrease -> YES) */
double copy_bef_aft_distance(int * cities,double * worse)
{
    double before, after;

    before = copy_get_cost(cities[0],cities[1],cities[2],cities[3]);
    after = copy_get_cost(cities[0],cities[2],cities[1],cities[3]);
    set_now_parcentage(before, after, worse);

    return (before - after);
}

/* input datas (a,ad,b,bd) are the number of index of city (Euclide) */
double copy_get_distance(int a, int ad, int b, int bd)
{
    return (copy_get_branch_distance(a, ad) + copy_get_branch_distance(b, bd));
}

/* input datas (a,ad,b,bd) are the number of index of city (Graph) */
double copy_get_cost(int a, int ad, int b, int bd)
{
    return (copy_get_branch_cost(a, ad) + copy_get_branch_cost(b, bd));
}

/* input datas (a,b) are the number of index of city */
double copy_get_branch_distance(int a, int b)
{
    return make_distance(get_x(a), get_y(a), get_x(b), get_y(b));
}

/* input datas (a,b) are the number of index of city */
double copy_get_branch_cost(int a, int b)
{
    return get_graph_cost(a,b);
}

void copy_exchange_branch(int * solution_path, int * indexs)
{
    int i, count;
    int tsp_size = solution_path[0];
    int * copy;
    int cities[4];
   
    copy = mallocer_ip(tsp_size + 1);

    for(i = 0; i <= tsp_size; i++) {
        copy[i] = solution_path[i];
    }
    for(i = 0; i <= (count = copy_get_among(indexs[1], indexs[2], tsp_size)); i++) {
        solution_path[copy_now_index((indexs[2] - i), tsp_size)] = copy[copy_now_index((indexs[1] + i), tsp_size)];
    }

    if(get_tabu_mode() == ON) {
        copy_get_cities_by_indexs(cities, indexs, solution_path);
        add_2opt_tabulist(cities);
    }

    free(copy);
}

int copy_get_among(int start, int end, int tsp_size)
{
    int return_num = end - start;

    if(return_num < 0) {
        return_num += tsp_size;
    }

    return return_num;
}
/* (1 <= return_num <= Max) */
int copy_next_index(int target, int maximum)
{
    return copy_now_index((target % maximum + 1), maximum);
}

/* (1 <= return_num <= Max) */
int copy_prev_index(int target, int maximum)
{
    return copy_now_index(((target == 1) ? maximum : target -1), maximum);
}

/* (1 <= return_num <= Max) */
int copy_now_index(int target, int maximum)
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
