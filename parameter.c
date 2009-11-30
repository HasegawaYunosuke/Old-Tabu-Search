/* header files */
#include "header.h"

/* functions */
int search_is_done(int type);
void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name);
int get_num_of_all_proc(void);
int get_process_number(void);
int not_found_looping(int * cities, int * indexs, int type);
void set_search_time(double search_time);
void set_tabu2opt_mode(void);
void set_euclid_mode(void);
void set_visual_mode(void);
void set_parallel_mode(void);
void set_pole_mode(void);
void set_tozaki_mode(void);
void set_tabu_mode(int type);
int get_tabu_mode(void);
void create_2opt_tabulist(int tsp_size, int mode);
void set_now_parcentage(double before, double after);
void set_2opt_loop(void);
void set_graph_data(double * graph_data);
double * get_graph_data(void);
void set_main_base_data(int * main_base_data);
int * get_main_base_data(void);
void set_solution_path(int * solution_path);
int * get_solution_path(void);
void initial_parameter(int tsp_size);
int turn_loop_times(int type);
int search_loop_times(int type);
int get_2opt_loop(void);
int get_tsp_size(void);
int get_x(int city_index);
int get_y(int city_index);
double get_worse_permit(void);
void change_worse_permit(int type);
double get_now_parcentage(void);
int check_parcentage(double bef_aft_distance);
double bef_aft_distance(int * cities);
double get_graph_cost(int a, int b);
void set_all_cost(void);
void create_historys(void);
void add_history(void);
void set_mode(void);
void set_solution_data_flag(void);
int get_solution_data_flag(void);
double get_all_cost_by_graph(int * solution_path);
double get_all_cost_by_euclid(int * solution_path);
double get_best_cost(void);
int now_index(int target, int maximum);

/* grobal variable */
struct parameter {
    int tsp_size;               /* TSP's Example Size */
    int city_point;
    int two_opt_loop;           /* Maximum of 2-opt loop */
    double now_parcentage;      /* Parcentage of now choice () */
    double permit_worse;        /* Parcentage of permitting to choice toward worse */
    double base_permit_worse;   /* Parcentage of permitting to choice toward worse */
    int * main_base_data;       /* TSPLIB's data, discribed by Euclid */
    double * graph_data;        /* TSPLIB's data, discribed by Graph */
    int * solution_path;        /* the sequence of city data arrived */
    double all_cost;            /* solution_path's cost */
    double best_cost;
    int turn_times;
    int search_times;
    int solution_data_flag;
    int search_is_done;
    int not_found_cities[4];
    int not_found_indexs[4];
    int not_found_loop;
    double not_found_def_aft_dis;
    int process_number;
    int num_of_all_proc;
    int name_length;
    char * process_name;
};

struct parameter * parameterp;

/* set all modes to OFF */
void set_mode(void)
{
    /* allocate "pointer of struct" memory */
    if((modep = malloc(sizeof(struct mode))) == NULL) {
        error_procedure("mode malloc()");
    }

    /* default modes */
    modep->graph_mode = ON;
    modep->hasegawa_mode = ON;
    modep->tabu_mode = OFF;
    modep->only2opt_mode = ON;
}

void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name)
{
    parameterp->num_of_all_proc = num_of_all_proc;
    parameterp->process_number = process_number;
    parameterp->name_length = name_length;
    parameterp->process_name = process_name;
}

int get_num_of_all_proc(void)
{
    return parameterp->num_of_all_proc;
}

int get_process_number(void)
{
    return parameterp->process_number;
}

int search_is_done(int type)
{
    switch(type) {
        case INIT:
            parameterp->search_is_done = YES;
            return 1;
            break;
        case READONLY:
            return parameterp->search_is_done;
            break;
    }
}

void set_tabu2opt_mode(void)
{
    modep->only2opt_mode = OFF;
    modep->tabu2opt_mode = ON;
}

void set_euclid_mode(void)
{
    modep->euclid_mode = ON;
    modep->graph_mode = OFF;
}

void set_visual_mode(void)
{
    modep->visual_mode = ON;
}

void set_parallel_mode(void)
{
    modep->parallel_mode = ON;
}

void set_pole_mode(void)
{
    modep->pole_mode = ON;
    modep->tozaki_mode = OFF;
    modep->hasegawa_mode = OFF;
    modep->only2opt_mode = OFF;
    modep->tabu2opt_mode = ON;
}

void set_tozaki_mode(void)
{
    modep->tozaki_mode = ON;
    modep->pole_mode = OFF;
    modep->hasegawa_mode = OFF;
}

void initial_parameter(int tsp_size)
{
    /* allocate "pointer of struct" memory */
    if((parameterp = malloc(sizeof(struct parameter))) == NULL) {
        error_procedure("parameter malloc()");
    }

    parameterp->tsp_size = tsp_size;
    parameterp->permit_worse = DEFAULT_PERMITWORSE;
    parameterp->base_permit_worse = DEFAULT_PERMITWORSE;
    parameterp->city_point = DEFAULT_CITYPOINT;
    parameterp->best_cost = DBL_MAX;
    parameterp->all_cost = DBL_MAX;
    parameterp->turn_times = 0;
    parameterp->search_times = 0;
    parameterp->solution_data_flag = OFF;
    parameterp->search_is_done = NO;
    parameterp->not_found_loop = 0;
    parameterp->not_found_def_aft_dis = (-1) * DBL_MAX;
    parameterp->process_number = 0;
    parameterp->num_of_all_proc = 1;
    set_2opt_loop();


    create_historys();
    /* create tabu list for 2-opt (only first procedure) */
    if(modep->tabu2opt_mode == ON) {
        create_2opt_tabulist(get_tsp_size(), INIT);
    }

}

int turn_loop_times(int type)
{
    int return_num;

    if(type == CHECK) {
        parameterp->turn_times++;

        return_num = parameterp->turn_times;
    }
    else if(type == INIT) {
        parameterp->turn_times = 0;
    }
    else if(type == READONLY) {
        return_num = parameterp->turn_times;
    }

    return return_num;
}

int search_loop_times(int type)
{
    int return_num;

    if(type == CHECK) {
        parameterp->search_times++;

        return_num = parameterp->search_times;
    }
    else if(type == READONLY) {
        return_num = parameterp->search_times;
    }

    return return_num;
}

int not_found_looping(int * cities, int * indexs, int type)
{
    int i;
    int return_num = NO;

    switch(type) {
        case COUNT:
            if(parameterp->not_found_def_aft_dis < bef_aft_distance(cities)) {
                parameterp->not_found_def_aft_dis = bef_aft_distance(cities);
                for(i = 0; i < 4; i++) {
                    parameterp->not_found_cities[i] = cities[i];
                    parameterp->not_found_indexs[i] = indexs[i];
                }
            }
            parameterp->not_found_loop++;
            if(parameterp->not_found_loop > DEFAULT_NOTFOUNDLOOP) {
                return_num = YES;
            }
            break;
        case INIT:
            parameterp->not_found_loop = 0;
            parameterp->not_found_def_aft_dis = (-1) * DBL_MAX;
            break;
        case READONLY:
            for(i = 0; i < 4; i++) {
                cities[i] = parameterp->not_found_cities[i];
                indexs[i] = parameterp->not_found_indexs[i];
            }
            break;
    }

    return return_num;
}

void set_city_point(int * point_of_cities)
{
    int i;

    for(i = 1; i <= point_of_cities[0]; i++) {
        point_of_cities[i] = parameterp->city_point;
    }
}

void set_tabu_mode(int type)
{
    if(type == ON) {
        modep->tabu_mode = ON;
    }
    else if(type == OFF) {
        modep->tabu_mode = OFF;
    }
}

int get_tabu_mode(void)
{
    return modep->tabu_mode;
}

double get_worse_permit(void)
{
    return parameterp->permit_worse;
}

void change_worse_permit(int type)
{
    switch(type) {
        case CLEAR:
            parameterp->permit_worse = parameterp->base_permit_worse;
            break;
        case ADD:
            parameterp->permit_worse += DEFAULT_ADDPERMITWORSE;
            break;
    }
}

int get_2opt_loop(void)
{
    return parameterp->two_opt_loop;
}

void set_2opt_loop(void)
{
    parameterp->two_opt_loop = DEFAULT_2OPTLOOP;
}

void set_graph_data(double * graph_data)
{
    parameterp->graph_data = graph_data;
}

void set_main_base_data(int * main_base_data)
{
    parameterp->main_base_data = main_base_data;
}

void set_solution_path(int * solution_path)
{
    parameterp->solution_path = solution_path;
    set_all_cost();
    add_history();
    set_solution_data_flag();
}

void set_solution_data_flag(void)
{
    parameterp->solution_data_flag = ON;
}

int get_solution_data_flag(void)
{
    return parameterp->solution_data_flag;
}

void set_now_parcentage(double before, double after)
{
    double best_cost = 0;

    best_cost = get_best_cost();

    /* permit_worse > 0 */
    parameterp->now_parcentage = (after - before) / best_cost * 100;
}

/* return num is YES or NO */
int check_parcentage(double bef_aft_distance)
{
    int return_num = NO;
    double best_cost = get_best_cost();
    double after_all_cost;

    if(modep->euclid_mode == OFF) {
        after_all_cost = get_all_cost_by_graph(get_solution_path()) - bef_aft_distance;
    }
    else {
        after_all_cost = get_all_cost_by_euclid(get_solution_path()) - bef_aft_distance;
    }

    if(((get_worse_permit() / 100 + 1) * best_cost) > after_all_cost) {
        return_num = YES;
    }

    return return_num;
}

double get_now_parcentage(void)
{
    return parameterp->now_parcentage;
}

int get_tsp_size(void)
{
    return parameterp->tsp_size;
}

double get_graph_cost(int a, int b)
{
    return parameterp->graph_data[a + get_tsp_size() * b];
}

int * get_main_base_data(void)
{
    return parameterp->main_base_data;
}

double * get_graph_data(void)
{
    return parameterp->graph_data;
}

int * get_solution_path(void)
{
    return parameterp->solution_path;
}

void set_all_cost(void)
{
    double all_cost = 0;

    if(modep->graph_mode == ON) {
        all_cost = get_all_cost_by_graph(get_solution_path());
    }
    else if(modep->euclid_mode == ON) {
        all_cost = get_all_cost_by_euclid(get_solution_path());
    }

    parameterp->all_cost = all_cost;

    if(all_cost < parameterp->best_cost) {
        parameterp->best_cost = all_cost;
    }
}

double get_best_cost(void)
{
    return parameterp->best_cost;
}

double get_all_cost_by_graph(int * cities)
{
    int i, now, next;
    int tsp_size = cities[0];
    double all_cost = 0;

    for(i = 1; i <= tsp_size; i++) {
        now = i;
        next = now_index(i + 1, tsp_size);

        all_cost += get_graph_cost(cities[now], cities[next]);
    }

    return all_cost;
}

double get_all_cost_by_euclid(int * cities)
{
    int tsp_size = cities[0];
    double all_cost = 0;

    /* DEL ST */
    error_procedure("get_all_cost_by_euclid() is non-avairable");
    /* DEL EN */

    return all_cost;
}

int get_x(int city_index)
{
    int * main_base_data;

    main_base_data = get_main_base_data();
    return main_base_data[city_index * 2];
}

int get_y(int city_index)
{
    int * main_base_data;

    main_base_data = get_main_base_data();
    return main_base_data[city_index * 2 + 1];
}
