/*
    This file "tozaki.c" can edit by ONLY Tozaki.
*/

/* header files */
#include "header.h"
#include <sched.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#define _GUN_SOURCE 1
#define THREAD_NUM 16
#define DEFAULT_BRANCH_PERMITWORSE 10
#define DEFAULT_ALL_COST_PERMITWORSE 3
#define __CPU_ZERO
#define __CPU_SET

/* functions */
int * tozaki_search(int * solution_path);
int * copy_euclid_search(int * solution_path);
int * copy_graph_search(int * solution_path);
int * copy_two_opt_tabu(int * solution_path);
int * copy_two_opt_only(int * solution_path);
void copy_choice_4indexs(int type, int * cities, int * solution_path);
int branch_exchange_permit_worse(double bef_aft_distance, double * branch_permit_worse);
int copy_mode_select(int mode, int * solution_path);
void create_2opt_tabulist(int tsp_size, int mode);
int copy_next_index(int target, int maximum);
int copy_prev_index(int target, int maximum);
int copy_now_index(int target, int maximum);
int * mallocer_ip(int size);
double branch_bef_aft_distance(int * cities);
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
void thread_core_assigned(void *arg);
void thread_two_opt_tabu(int * solution_path, int tsp_size, int thread_num, double * branch_permit_worse);
int * get_solution_path();
void branch_change_worse_permit(int type, double * branch_permit_worse);
int get_tsp_size(void);
int all_cost_permit_worse(int * solution_path,int * indexs);
double get_all_cost_by_graph(int * solution_path);
int turn_loop_times(int type);

/* grobal variable */
int * grobal_solution_path;
int mutex_cheaker;
pthread_mutex_t parallel_mutex;

typedef struct _thread_arg {
    int core_num;
    int thread_no;
    double branch_permit_worse_persentage;
    int * path;
} thread_arg_t;

int * tozaki_search(int * solution_path)
{
    int * return_data;

    /* Search Euclid-Data */
    if(modep->euclid_mode == ON) {
        return_data = copy_euclid_search(solution_path);
    }
    /* Search Graph-Data (non-available) */
    else if(modep->graph_mode == ON) {
        return_data = copy_graph_search(solution_path);
    }
    /* Error */
    else {
        error_procedure("tozaki_search()");
    }
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
    pthread_t parallel_thread[THREAD_NUM];
    thread_arg_t targ[THREAD_NUM];
    int tsp_size = solution_path[0];
    int cpu_num;
    int i, j;
    int skip = NO;

    /* (1) First, this fucn exchange branch by "2-opt" only toward better without using tabu-list */
    if(get_tabu_mode() == OFF) {
        solution_path = copy_two_opt_only(solution_path);

        if(check_manneri(SHORTMODE) == YES) {
            set_tabu_mode(ON);
        }
    }
    /* (2) Second, permit exchange toward worse, and use tabu-list */
    else {
        grobal_solution_path = mallocer_ip(tsp_size + 1);

        for(i = 0; i < tsp_size + 1; i++) {
            grobal_solution_path[i] = solution_path[i];
        }

        cpu_num = sysconf(_SC_NPROCESSORS_CONF);
        /* mutex init */
        pthread_mutex_init(&parallel_mutex, NULL);
        mutex_cheaker = 0;

        /*go to thread_two_opt_tabu() */
        for(i = 0; i < THREAD_NUM; i++) {
            targ[i].core_num = i % cpu_num;
            targ[i].thread_no = i;
            targ[i].branch_permit_worse_persentage = DEFAULT_BRANCH_PERMITWORSE;
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

        for(i = 1; i < tsp_size; i++) {
            for(j = i + 1; j <= tsp_size; j++) {
                if(grobal_solution_path[i] == grobal_solution_path[j]) {
                    skip = YES;
                }
            }
        }
        if(skip == NO) {
            for(i = 0; i < tsp_size + 1; i++) {
                solution_path[i] = grobal_solution_path[i];
            }
        }
        /* DEL ST */
        else {
            printf("\n*************************************************************************\n\n");
        }
        /* DEL EN */
        free(grobal_solution_path);
    }
    return solution_path;
}

void thread_core_assigned(void *arg)
{
    cpu_set_t mask;
    /* change void to thread_arg_t */
    thread_arg_t * targ = (thread_arg_t *)arg;
    int tsp_size = targ->path[0];
    int thread_num = targ->thread_no;
    double branch_permit_worse = targ->branch_permit_worse_persentage;

    __CPU_ZERO(&mask);
    
    if(sched_getaffinity(syscall(__NR_gettid), sizeof(mask), &mask) == -1) {
        error_procedure("sched_getaffnity()");
    }
    __CPU_SET(targ->core_num, &mask);

    if(sched_setaffinity(syscall(__NR_gettid), sizeof(mask), &mask) == -1) {
        error_procedure("sched_setaffnity()");
    }

    thread_two_opt_tabu(targ->path, tsp_size, thread_num, &branch_permit_worse);
}

void thread_two_opt_tabu(int * solution_path, int tsp_size, int thread_num, double * branch_permit_worse)
{
    int indexs[4];
    int cities[4];
    int i;
    int skip_cheaker;

    if(thread_num == THREAD_NUM - 1) {
        solution_path = copy_two_opt_only(solution_path);
    }
    else {
        do {
            skip_cheaker = OFF;

            for(i = 0; i < tsp_size + 1; i++) {
                if( mutex_cheaker == OFF) {
                    solution_path[i] = grobal_solution_path[i];
                }
                else {
                    i = 0;
                }
            }

            copy_choice_4indexs(DEFAULT, indexs, solution_path);
            copy_get_cities_by_indexs(cities, indexs, solution_path);

            if(mutex_cheaker == ON) {
                skip_cheaker = ON;
            }

            if(timer(CHECK) == OFF) {
                break;
            }
        } while(skip_cheaker == ON || all_cost_permit_worse(solution_path, indexs) == NO || is_2opt_tabu(cities) == YES);
        copy_exchange_branch(solution_path, indexs);
    }
    turn_loop_times(CHECK);
}

void copy_exchange_branch(int * solution_path, int * indexs)
{
    int i, count;
    int tsp_size = solution_path[0];
    int * copy;
    int cities[4];

    /* lock action or return */
    if(get_tabu_mode() == ON && mutex_cheaker == OFF) {
        pthread_mutex_lock(&parallel_mutex);
    }
    else if(get_tabu_mode() == ON && mutex_cheaker == ON) {
        return;
    }

    if(get_tabu_mode() == ON) {
        mutex_cheaker = ON;
    }
  
    copy = mallocer_ip(tsp_size + 1);

    for(i = 0; i <= tsp_size; i++) {
        copy[i] = solution_path[i];
    }
 
    for(i = 0; i <= (count = copy_get_among(indexs[1], indexs[2], tsp_size)); i++) {
        solution_path[copy_now_index((indexs[2] - i), tsp_size)] = copy[copy_now_index((indexs[1] + i), tsp_size)];
    }

    if(get_tabu_mode() == ON) {
        for(i = 0; i < tsp_size + 1; i++) {
            grobal_solution_path[i] = solution_path[i];
        }
    }

    if(get_tabu_mode() == ON) {
        copy_get_cities_by_indexs(cities, indexs, solution_path);
        add_2opt_tabulist(cities);
    }

    free(copy);

    /* unlock action */
    if(get_tabu_mode() == ON) {
        pthread_mutex_unlock(&parallel_mutex);
        mutex_cheaker = OFF;
    }

}

int * copy_two_opt_only(int * solution_path)
{
    int indexs[4];

    if(modep->graph_mode = ON) {
        copy_get_min_exchange_indexs(solution_path, indexs);
    }
    else {
        error_procedure("copy_two_opt_only()'s 2-opt only mode");
    }

    copy_exchange_branch(solution_path, indexs);

    return solution_path;
}

void copy_get_min_exchange_indexs(int * solution_path, int * best_indexs)
{
    int i, j, k;
    int tsp_size = solution_path[0];
    int cities[4];
    int indexs[4];
    double distance = DBL_MAX * (-1);
    double maximum = DBL_MAX * (-1);

    if(get_tabu_mode() == ON) {
        for(i = 0; i < tsp_size + 1; i++) {
            if( mutex_cheaker == OFF) {
                solution_path[i] = grobal_solution_path[i];
            }
            else {
                i = 0;
            }
        }
    }
    for(i = 1; i <= tsp_size - 3; i++) {
        indexs[0] = i; indexs[1] = i + 1;
        for(j = i + 2; j <= tsp_size - 1; j++) {
            indexs[2] = j; indexs[3] = j + 1;
            copy_get_cities_by_indexs(cities, indexs, solution_path);
            if(maximum < (distance = branch_bef_aft_distance(cities))) {
                maximum = distance;
                for(k = 0; k < 4; k++) {
                    best_indexs[k] = indexs[k];
                }
            }
        }
    }
    if(get_tabu_mode() == ON && mutex_cheaker ==ON) {
        copy_get_min_exchange_indexs(solution_path, best_indexs);
    }
}

/* permit exchange toward worse if under permit_baseline */
int all_cost_permit_worse(int * solution_path,int * indexs)
{
    int return_num = NO;
    double before, after;
    double parcentage;
    int i, count;
    int tsp_size = solution_path[0];
    int * prev_solution_path;

    prev_solution_path = mallocer_ip(tsp_size + 1);

    for(i = 0; i <= tsp_size; i++) {
        prev_solution_path[i] = solution_path[i];
    }
    for(i = 0; i <= (count = copy_get_among(indexs[1], indexs[2], tsp_size)); i++) {
        solution_path[copy_now_index((indexs[2] - i), tsp_size)] = prev_solution_path[copy_now_index((indexs[1] + i), tsp_size)];
    }

    if(modep->graph_mode == ON) {
        before = get_all_cost_by_graph(prev_solution_path);
        after = get_all_cost_by_graph(solution_path);
    }
    else if(modep->euclid_mode == ON) {
        printf("Not yet support so exit(1)\n");
        exit(1);
    }

    free(prev_solution_path);

    parcentage = (after - before) / before * 100;
 
 /* if exchange is toward better, permit */
    if(before - after > 0) {
        return_num = YES;
    }
    else{
        /* permit_worse discribed > 0 */
        if(DEFAULT_ALL_COST_PERMITWORSE < parcentage) {
            return_num = NO;
        }
        else {
            return_num = YES;
        }
    }

    return return_num;
}

/* permit exchange toward worse if under permit_baseline */
int branch_exchange_permit_worse(double bef_aft_distance, double * branch_permit_worse)
{
    int return_num = NO;

    /* if exchange is toward better, permit */
    if(bef_aft_distance > 0) {
        return_num = YES;
    }
    else{
        /* permit_worse discribed > 0 */
        if(* branch_permit_worse < get_now_parcentage()) {
            return_num = NO;
        }
        else {
            return_num = YES;
        }
    }

    if(return_num == YES) {
        change_worse_permit(CLEAR, branch_permit_worse);
    }
    else {
        change_worse_permit(ADD, branch_permit_worse);
    }

    return return_num;
}

void branch_change_worse_permit(int type, double * branch_permit_worse)
{
    switch(type) {
        case CLEAR:
            * branch_permit_worse = DEFAULT_BRANCH_PERMITWORSE;
            break;
        case ADD:
            * branch_permit_worse += DEFAULT_ADDPERMITWORSE / (get_tsp_size() * 100);
            break;
    }
}

/* return "YES" or "NO" (Decrease -> YES) */
double branch_bef_aft_distance(int * cities)
{
    double before, after;

    if(modep->euclid_mode == ON) {
        before = copy_get_distance(cities[0],cities[1],cities[2],cities[3]);
        after = copy_get_distance(cities[0],cities[2],cities[1],cities[3]);
    }
    else if(modep->graph_mode == ON) {
        before = copy_get_cost(cities[0],cities[1],cities[2],cities[3]);
        after = copy_get_cost(cities[0],cities[2],cities[1],cities[3]);
    }

    set_now_parcentage(before, after);

    return (before - after);
}

void copy_get_cities_by_indexs(int * cities, int * indexs, int * solution_path)
{
    int i;

    for(i = 0; i < 4; i++) {
        cities[i] = solution_path[indexs[i]];
    }
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
