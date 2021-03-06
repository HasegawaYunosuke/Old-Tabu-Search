/* header files */
#include "header.h"

/* functions */
void initial_path(void);
void set_solution_path(int * solution_path);
void set_best_solution_path(int * solution_path);
int * get_solution_path(void);
int * get_main_base_data(void);
double * get_graph_data(void);
int * initial_euclid_path(int * euclid_data);
int * create_euclid_path(int * return_data, int * base_data, int create_mode);
int * initial_graph_path(double * graph_data);
int * create_graph_path(int * path, double * graph_data, int create_mode);
int random_num(int maximum);
double make_distance(int x1, int y1, int x2, int y2);
int already_visited(int * return_data, int city_num);
int search_loop_times(int type);
int * mallocer_ip(int size);
double * mallocer_dp(int size);

#ifdef MPIMODE
int get_smart_random_city(int maximum);
int get_process_number(void);
int get_group_reader(void);
int decide_create_mode(void);
int * get_merge_route(void);
int * get_smart_init_path(int * return_data);
int * get_other_group_sol_path(void);
void adjust_group_sol_to_return(int * all_path, int * return_data, int choice_index);
void add_MPI_same_group_tabulist(int add_mode, int * add_data);
#endif
#ifdef DEBUG
void loging_initial_path(int * path, int create_mode);
#endif

void set_ga_solution_path(int * solution_path);

void initial_path(void)
{
    /* create initial-path */
    set_solution_path(initial_graph_path(get_graph_data()));

    if(modep->pole_mode == ON){
        set_ga_solution_path(initial_graph_path(get_graph_data()));
    }
}
/* global variable */
int create_mode = DEFAULT;

int * initial_graph_path(double * graph_data)
{
    int * return_data;
    int malloc_size = (int)graph_data[0];
    int create_mode = DEFAULT;

    /* first time procedure */
    if(search_loop_times(READONLY) == 0 && turn_loop_times(READONLY) == 0) {
        return_data = mallocer_ip(malloc_size + DEFAULT_SENDPARAMETERNUM);
        set_best_solution_path(mallocer_ip(malloc_size + DEFAULT_SENDPARAMETERNUM));
        srand(time(NULL));
        /* set return_data[0] to 'TSP-example-size' */
        return_data[0] = (int)graph_data[0];
    }

#ifdef MPIMODE
    create_mode = decide_create_mode();
#endif

    /* create the initial path (DEAFAULT is Nearest-Neighbor) */
    return_data = create_graph_path(return_data, graph_data, create_mode);

    return return_data;
}

/* This fucntion makes 'initial-path' that type is 'Euclid' */
int * create_euclid_path(int * return_data, int * euclid_data, int create_mode)
{
    int return_data_index;
    int next_city;
    int min_index;
    int tsp_size = euclid_data[0];
    int first_point;
    int now_x, now_y;
    int next_x, next_y;
    double distance = DBL_MAX;
    double min_distance = DBL_MAX;

    /* Mode DEFAULT (nearest nearby) */
    if(create_mode == DEFAULT) {
        /* get first point by random (1 <= first_point <= tsp_size) */
        first_point = random_num(tsp_size);

        return_data[1] = first_point;
        now_x = euclid_data[first_point * 2];
        now_y = euclid_data[first_point * 2 + 1];
        for(return_data_index = 2; return_data_index <= tsp_size; return_data_index++) {
            for(next_city = 1; next_city <= tsp_size; next_city++) {
                /* check already choiced cities */
                if(already_visited(return_data, next_city) == YES) {
                    continue;
                }
                else {
                    next_x = euclid_data[next_city * 2];
                    next_y = euclid_data[next_city * 2 + 1];
                    distance = make_distance(now_x, now_y, next_x, next_y);
                    if(min_distance > distance) {
                        min_index = next_city;
                        min_distance = distance;

                    }
                }
            }
            /* set the city of nearest at now-city */
            return_data[return_data_index] = min_index;
            min_distance = DBL_MAX;
        }
    }
    /* Mode another */
    else {
        /* Non-aveirable, yet */
    }

    return return_data;
}

/* create graph-path */
/* return_data's format is followed.
 * TSP-Size, city1, city2, ... ,cityN, parameter1, ..., parameterM
 * 'N' means TSP-Size, 'M' is DEFAULT_SENDPARAMETERNUM - 1 (this '1' is First 'TSP-Size')
 * All data length is TSP-Size + DEFAULT_SENDPARAMETERNUM */
int * create_graph_path(int * return_data, double * graph_data, int create_mode)
{
    int i, j;
    int * buff;
    int mini_index;
    int first_point;
    int tsp_size = (int)graph_data[0];
    int now_city, next_city;
    int is_choiced[TSPMAXSIZE] = {NO};
    double distance = DBL_MAX;
    double min_distance = DBL_MAX;

    switch (create_mode) {
    #ifdef MPIMODE
        case SAMETABULIST:
            return_data = get_smart_init_path(return_data);
            break;
        case GROUPCREATE:
            buff = get_other_group_sol_path();
            adjust_group_sol_to_return(buff, return_data, random_num(DEFAULT_GROUP_DATASTOCKNUM - 1));
            #ifdef DEBUG
            //loging_initial_path(return_data, create_mode);
            #endif
            break;
        case MERGECREATE:
            return_data = get_merge_route();
            #ifdef DEBUG
            //loging_initial_path(return_data, create_mode);
            #endif
            break;
    #endif
        case DEFAULT:
#ifdef MPIMODE
            if(get_group_reader() == get_process_number()) {
                first_point = get_smart_random_city(tsp_size);
            }
            else {
                first_point = random_num(tsp_size);
            }
#else
            first_point = random_num(tsp_size);
#endif
            now_city = first_point;
            is_choiced[first_point] = YES;
            return_data[1] = first_point;

            for(i = 2; i <= tsp_size; i++) {
                for(j =1; j <= tsp_size; j++) {
                    if(is_choiced[j] == NO) {
                        next_city = j;
                        distance = graph_data[now_city + tsp_size * next_city];
                        if(min_distance > distance) {
                            mini_index = next_city;
                            min_distance = distance;
                        }
                    }
                    else {
                        continue;
                    }
                }
                return_data[i] = mini_index;
                is_choiced[mini_index] = YES;
                min_distance = DBL_MAX;
                now_city = mini_index;
            }
            break;
    }

    return return_data;
}

int already_visited(int * return_data, int city_num)
{
    int i;
    int return_num = NO;

    for(i = 1; i <= return_data[0]; i++) {
        if(return_data[i] == city_num) {
            return_num = YES;
            break;
        }
    }

    return return_num;
}
