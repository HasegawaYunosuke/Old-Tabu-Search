/* header files */
#include "header.h"

/* functions */
void mannneri_initialize(void);
int search_is_done(int type);
void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name);
int get_num_of_all_proc(void);
int get_process_number(void);
int get_name_length(void);
void set_MPI_group_data(int all_MPI_group, int MPI_group);
int get_MPI_group_data(void);
int get_all_MPI_group_data(void);
void set_same_group_list(int * list);
int * get_same_group_list(void);
char * get_process_name(void);
int not_found_looping(int * cities, int * indexs, int type);
void set_search_time(double search_time);
void set_tabu2opt_mode(void);
void set_euclid_mode(void);
void set_visual_mode(int mode);
int get_realtime_visual_mode(void);
void set_parallel_mode(void);
void set_pole_mode(void);
void set_tozaki_mode(void);
void set_tabu_mode(int type);
void set_middle_mannneri(int on_or_off);
int get_tabu_mode(void);
void set_ga_mode(int type);
int get_ga_mode(void);
void create_2opt_tabulist(int tsp_size, int mode);
void set_now_parcentage(double before, double after);
void set_2opt_loop(void);
void set_graph_data(double * graph_data);
double * get_graph_data(void);
void set_main_base_data(int * main_base_data);
int * get_main_base_data(void);
void set_solution_path(int * solution_path);
void set_best_solution_path(int * best_solution_path);
int * get_best_solution_path(void);
void set_best_solution_path_data(void);
void set_other_solution_path_data(int * solution_path);
int * get_other_solution_path_data(void);
int check_other_solution_path_data(int *other_sol_path);
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
int * mallocer_ip(int size);

void set_counter(void);
int * get_ga_solution_path(void);
void set_ga_solution_path(int * solution_path);
int get_ga_mode(void);
void set_start_time(time_t start_time);
time_t get_start_time(void);

#ifdef MPIMODE
void initialize_share_tabulist(void);
void create_readers_list(void);
int * get_readers_list(void);
int get_group_reader(void);
void set_now_other_group_stac_index(int stac_num);
int get_now_other_group_stac_index(void);
int * get_same_group_list(void);
void set_group_start_process(int group_start_process);
int get_group_start_process(void);
void set_other_group_sol_path_data(int * pointer);
int * get_other_group_sol_path(void);
int get_other_group_stac_satisfaction(void);
void best_MPI_send(void);
void set_merge_branchs(void);
void adjust_group_sol_to_return(int * all_path, int * return_data, int choice_index);
void free_merge_branchs(void);
int * get_branchA(void);
int * get_branchB(void);
int * get_temp_path(void);
int * get_matchedA(void);
int * get_matchedB(void);
#endif

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
    int * best_solution_path;   /* Local Best Solution Path */
    int * other_solution_path;
    int * other_group_path;
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
    int MPI_group;
    int group_reader_process;
    int now_other_group_stac_index;
    int other_group_stac_satisfaction_flag;
    int all_MPI_group;
    int * same_group_list;
    int * group_readers_list;
    int group_start_process;
    int name_length;
    char * process_name;
    int * ga_solution_path;
    time_t start_time;
    int * branchsA;
    int * branchsB;
    int * temp_path;
    int * matchedA;
    int * matchedB;
};

struct parameter * parameterp;

struct parameter * get_parameterp(void);
struct mode * get_modep(void);

struct parameter * get_parameterp(void)
{
    return parameterp;
}

struct mode * get_modep(void)
{
    return modep;
}

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
    modep->visual_mode = OFF;
    modep->realtime_visual_mode = OFF;
    modep->middle_manneri = OFF;
}

void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name)
{
    int i;

    parameterp->num_of_all_proc = num_of_all_proc;
    parameterp->process_number = process_number;
    parameterp->name_length = name_length;
    parameterp->process_name = malloc(sizeof(char) * name_length);
    for(i = 0; i < name_length; i++) {
        parameterp->process_name[i] = process_name[i];
    }
    parameterp->other_group_stac_satisfaction_flag = OFF;
}

void create_readers_list(void)
{
    int i, index = 0;
    int num_of_proc_in_one_group = get_num_of_all_proc() / DEFAULT_MPIGROUPNUM;

    parameterp->group_readers_list = mallocer_ip(DEFAULT_MPIGROUPNUM - 1);

    for(i = 0; i < DEFAULT_MPIGROUPNUM; i++) {
        if(i * num_of_proc_in_one_group != get_process_number()) {
            parameterp->group_readers_list[index] = i * num_of_proc_in_one_group;
            index++;
        }
    }
}

void set_now_other_group_stac_index(int stac_num)
{
    if(stac_num >= 0) {
        parameterp->now_other_group_stac_index = stac_num;
    }
    else {
        parameterp->other_group_stac_satisfaction_flag = ON;
    }
}

int get_other_group_stac_satisfaction(void)
{
    return parameterp->other_group_stac_satisfaction_flag;
}

int get_now_other_group_stac_index(void)
{
    return parameterp->now_other_group_stac_index;
}

int * get_readers_list(void)
{
    return parameterp->group_readers_list;
}

void set_MPI_group_data(int all_MPI_group, int MPI_group)
{
    parameterp->MPI_group = MPI_group;
    parameterp->all_MPI_group = all_MPI_group;
    parameterp->group_reader_process =  MPI_group * (get_num_of_all_proc() / DEFAULT_MPIGROUPNUM);
}

void set_other_group_sol_path_data(int * pointer)
{
    parameterp->other_group_path = pointer;
}

int * get_other_group_sol_path(void)
{
    return parameterp->other_group_path ;
}

void adjust_group_sol_to_return(int * all_path, int * return_data, int choice_index)
{
    int i;
    int tsp_size = get_tsp_size();
    int element_num = tsp_size + DEFAULT_SENDPARAMETERNUM;
    int start_point = element_num * choice_index;

    for(i = 0; i < element_num; i++) {
        if(i == 0) {
            return_data[i] = tsp_size;
        }
        /* city solution-path */
        else if(i <= tsp_size) {
            return_data[i] = all_path[start_point + i - 1];
        }
        /* parameter */
        else {
            return_data[i] = all_path[start_point + i - 1];
        }
    }
}

int get_group_reader(void)
{
    return parameterp->group_reader_process;
}

int get_MPI_group_data(void)
{
    return parameterp->MPI_group;
}

int get_all_MPI_group_data(void)
{
    return parameterp->all_MPI_group;
}

void set_same_group_list(int * list)
{
    parameterp->same_group_list = list;
}

int * get_same_group_list(void)
{
    return parameterp->same_group_list;
}

int get_num_of_all_proc(void)
{
    return parameterp->num_of_all_proc;
}

int get_process_number(void)
{
    return parameterp->process_number;
}

int get_name_length(void)
{
    return parameterp->name_length;
}

char * get_process_name(void)
{
    return parameterp->process_name;
}

#ifdef MPIMODE
void set_group_start_process(int group_start_process)
{
    parameterp->group_start_process = group_start_process;
}

int get_group_start_process(void)
{
    return parameterp->group_start_process;
}
#endif

void set_merge_branchs(void)
{
    int tsp_size = get_tsp_size();

    parameterp->branchsA = mallocer_ip(tsp_size * 2);
    parameterp->branchsB = mallocer_ip(tsp_size * 2);
    parameterp->temp_path = mallocer_ip(tsp_size + 1);
    parameterp->matchedA= mallocer_ip(tsp_size);
    parameterp->matchedB= mallocer_ip(tsp_size);
}

int * get_branchA(void)
{
    return parameterp->branchsA;
}

int * get_branchB(void)
{
    return parameterp->branchsB;
}

int * get_temp_path(void)
{
    return parameterp->temp_path;
}

int * get_matchedA(void)
{
    return parameterp->matchedA;
}

int * get_matchedB(void)
{
    return parameterp->matchedB;
}

void free_merge_branchs(void)
{
    free(get_branchA());
    free(get_branchB());
    free(get_temp_path());
    free(get_matchedA());
    free(get_matchedB());
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

void set_visual_mode(int mode)
{
    modep->visual_mode = ON;
    if(mode == 1) {
        modep->realtime_visual_mode = ON;
    }
}

int get_realtime_visual_mode(void)
{
    return modep->realtime_visual_mode;
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
    modep->ga_mode = ON;
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
    parameterp->MPI_group = 0;
    parameterp->all_MPI_group = 0;
    set_2opt_loop();

    /* initilize manneri-functions */
    mannneri_initialize();

    create_historys();
    /* create tabu list for 2-opt (only first procedure) */
    if(modep->tabu2opt_mode == ON) {
        create_2opt_tabulist(get_tsp_size(), INIT);
#ifdef MPIMODE
        initialize_share_tabulist();
#endif
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

void set_ga_mode(int type)
{
    if(type == ON) {
        modep->ga_mode = ON;
    }
    else if(type == OFF) {
        modep->ga_mode = OFF;
    }
}

int get_ga_mode(void)
{
    return modep->ga_mode;
}

void set_middle_mannneri(int on_or_off)
{
    switch(on_or_off) {
        case ON:
            modep->middle_manneri = ON;
            break;
        case OFF:
            modep->middle_manneri = OFF;
            break;
    }
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

void set_ga_solution_path(int * solution_path)
{
    parameterp->ga_solution_path = solution_path;
}

void set_best_solution_path(int * best_solution_path)
{
    parameterp->best_solution_path = best_solution_path;
}

int * get_best_solution_path(void)
{
    return parameterp->best_solution_path;
}

void set_best_solution_path_data(void)
{
    int i;
    int tsp_size = get_tsp_size();
    int * now_best_sol_path = get_solution_path();
    int * old_best_sol_path = get_best_solution_path();

    for(i = 0; i < tsp_size; i++) {
        old_best_sol_path[i] = now_best_sol_path[i + 1];
    }
}

void set_other_solution_path_data(int *other_solution_path)
{
    parameterp->other_solution_path = other_solution_path;
}

int * get_other_solution_path_data(void)
{
    return parameterp->other_solution_path;
}

int check_other_solution_path_data(int *other_sol_path)
{
    int i;
    int tsp_size = get_tsp_size();
    int return_num = YES;

    for(i = 0; i < tsp_size + 1; i++){
        if(other_sol_path[i] < 1 || tsp_size < other_sol_path[i]){
            return_num = NO;
            break;
        }
    }

    return return_num;
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

int * get_ga_solution_path(void)
{
    return parameterp->ga_solution_path;
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
        set_best_solution_path_data();
        #ifdef MPIMODE
        //best_MPI_send();
        #endif
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

void set_start_time(time_t start_time)
{
    parameterp->start_time = start_time;
}

time_t get_start_time(void)
{
    return parameterp->start_time;
}
