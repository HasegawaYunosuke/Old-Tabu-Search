#include "header.h"

int * mallocer_ip(int size);
int get_all_search_is_done(void);
void set_MPI_parameter(void);
void set_MPI_group(void);
void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name);
void set_MPI_group_data(int group_num, int my_group);
void set_same_group_list(int * list);
void set_other_solution_path(void);
void set_other_solution_path_data(int * solution_path);
void set_group_start_process(int group_start_process);
void create_same_group_list(int group_num, int my_group);
int * get_other_solution_path_data(void);
int get_num_of_all_proc(void);
int get_process_number(void);
int get_tsp_size(void);
int get_all_MPI_group_data(void);
int * get_same_group_list(void);
char * get_process_name(void);
void best_MPI_send(void);
void best_MPI_recv(int * recv_process_number);
int * get_best_solution_path(void);
int * get_merge_route(void);
int decide_create_mode(void);
int check_other_data_satisfactory(void);
int check_other_group_data_satisfactory(int type);
int get_other_group_stac_satisfaction(void);
int random_num(int maximum);
int * get_solution_path(void);
void set_merge_branchs(void);
void merge_route(int * sol_path, int * other_sol, int choice);
int * get_branchA(void);
int * get_branchB(void);
int * get_temp_path(void);
void adjust_branchs(int * branchs, int * other_sol, int * temp_path, int choice);
void set_branch_data(int * branchs, int * path);
void sort_branch_data(int * branchs);
void check_matching(int * branchsA, int * branchsB);
void initialize_matched(int * matched);
int * get_matchedA(void);
int * get_matchedB(void);
void get_route_by_matched(int * sol_path, int * matchedB, int * temp_path);
void initialize_leftovers_path(int * sol_path, int maximum, int * used_cities);
double * get_graph_data(void);
int check_manneri(int type);
int is_this_ok_same_group_list(int * list, int all_process);
void how_long_matched(int * maximum, int * max_i, int * matchedB, int size);
void group_reader_process(void);
void create_readers_list(void);
int * get_readers_list(void);
void group_reader_recv(int * argument);
void group_reader_send(int * type);
void group_reader_send_thread(int type);
void set_other_group_sol_path(void);
int * get_other_group_sol_path(void);
void set_other_group_sol_path_data(int * pointer);
void copy_to_group_data(int * buffer, int element_num, int stac_num);
void set_now_other_group_stac_index(int stac_num);
int get_now_other_group_stac_index(void);
int get_send_recv_element_num(void);
int * get_tabulist_data(void);
int * get_tabulist_data_buffer(void);
void copy_to_share_tabulist(int * tabulist_buffer, int element_num);
int share_tabulist_is_satisfactory(void);
int * get_share_tabulist(void);
/* DEL ST */
void check_send_data(int * send_data, int send_num);
void show_saved_other_sol(void);
/* DEL EN */

#ifdef DEBUG
void mpi_comunication_log_manage(int type);
void figure_of_match_num(int matched_num);
void output_other_sol_path(void);
void test_debug_log(char message[128], int num);
#endif

/* grobal variable */
pthread_mutex_t recv_sol_lock;
int before_send_process_index;

/* if Using Score System */
void set_MPI_parameter(void)
{
    int num_of_all_proc;
    int process_number;
    int name_length;
    char process_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Comm_size(MPI_COMM_WORLD, &num_of_all_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);
    MPI_Get_processor_name(process_name, &name_length);

    set_parameter_data(num_of_all_proc, process_number, name_length, process_name);
    set_MPI_group();
    set_other_solution_path();
    set_other_group_sol_path();
    set_merge_branchs();
    before_send_process_index = 0;
    #ifdef DEBUG
    mpi_comunication_log_manage(INIT);
    #endif
}

/* allocation of Other Group's Solution-Path Data Memory */
/* data-format is followed.
 * (city1, city2, ... , cityN, parameter1, parameter2, ... ,parameterM,
 *  city1, city2, ... , cityN, parameter1, parameter2, ... ,parameterM,
 *  ...
 *  <The line-num of city-and-parameter data depend on proc num(<--create_num) in One-Group>
 *  ...
 *  city1, city2, ... , cityN, parameter1, parameter2, ... ,parameterM)
 *
 * 'N' is TSP-Problem-Size, 'M' is DEFAULT_SENDPARAMETERNUM's size */
void set_other_group_sol_path(void)
{
    int create_num = DEFAULT_GROUP_DATASTOCKNUM;
    int size = get_tsp_size() + DEFAULT_SENDPARAMETERNUM;

    set_other_group_sol_path_data(mallocer_ip(size * create_num));
}

void set_other_solution_path(void)
{
    int create_num = get_all_MPI_group_data() - 1;
    int * save_pointer;

    save_pointer = mallocer_ip((get_tsp_size() + DEFAULT_SENDPARAMETERNUM) * create_num);
    set_other_solution_path_data(save_pointer);
}

/* If this process is appointed as a "group-reader",
 * this process have to send-and-recv something data to other-GROUP (reader). */
void group_reader_process(void)
{
    pthread_t group_reader_thread;
    int argument = TABU_LIST_SHARE;

    /* create other group-reader's process-num list */
    /* (ex)
     * this-proc-num is "0", all-proc-num is "8", and the num of Groups is "4",
     * "process:0,2,4,6" are the Group-Reader. So, the content of list is "2,4,6" (except myself) */
    create_readers_list();

    /* reader's reciev_process */
    pthread_create(&group_reader_thread,
                    NULL,
                    (void *)group_reader_recv,
                    (void *)&argument);
}

void group_reader_recv(int * argument)
{
    int element_num;
    int buffer[TSPMAXSIZE];
    int * tabulist_buffer = get_tabulist_data_buffer();
    MPI_Status stat;
    int stac_num = 0;
    int recvbuff_flag = 0;

    switch((*argument)) {
        case SOL_PATH_SHARE:
            element_num = get_tsp_size() + DEFAULT_SENDPARAMETERNUM;
            break;
        case TABU_LIST_SHARE:
            element_num = get_send_recv_element_num();
            break;
    }

    switch((*argument)) {
        case SOL_PATH_SHARE:
            for(;;) {
                if(get_all_search_is_done() == YES) {
                    exit(0);
                }
                MPI_Iprobe(MPI_ANY_SOURCE, GROUP_SOLUTION, MPI_COMM_WORLD, &recvbuff_flag, &stat);
                if(recvbuff_flag == 1) {
                    MPI_Recv((void *)buffer, element_num, MPI_INT, MPI_ANY_SOURCE, GROUP_SOLUTION, MPI_COMM_WORLD, &stat);
                    copy_to_group_data(buffer, element_num, stac_num);
                    if(stac_num > DEFAULT_GROUP_DATASTOCKNUM- 1) {
                        stac_num = 0;
                        set_now_other_group_stac_index(-1);
                    }
                    else {
                        stac_num++;
                        set_now_other_group_stac_index(stac_num - 1);
                    }
                }
                usleep(100000);
            }
        case TABU_LIST_SHARE:
            for(;;) {
                if(get_all_search_is_done() == YES) {
                    exit(0);
                }
                MPI_Iprobe(MPI_ANY_SOURCE, GROUP_SOLUTION, MPI_COMM_WORLD, &recvbuff_flag, &stat);
                if(recvbuff_flag == 1) {
                    MPI_Recv((void *)tabulist_buffer, element_num, MPI_INT, MPI_ANY_SOURCE, GROUP_SOLUTION, MPI_COMM_WORLD, &stat);
                    copy_to_share_tabulist(tabulist_buffer, element_num);
                }
                usleep(100000);
            }
    }
}

void group_reader_send_thread(int type)
{
    pthread_t thread_tt;

    pthread_create(&thread_tt,
                    NULL,
                    (void *)group_reader_send,
                    (void *)&type);
}

void group_reader_send(int * type)
{
    int * my_best_sol = get_best_solution_path();
    int * my_share_tabulist = get_share_tabulist();
    int element_num;
    int * list = get_readers_list();
    int send_node;
    int type_test = TABU_LIST_SHARE;
    int i;

    switch(type_test) {
        case SOL_PATH_SHARE:
            element_num = get_tsp_size() + DEFAULT_SENDPARAMETERNUM;
            send_node = list[random_num(DEFAULT_MPIGROUPNUM - 1)];
            MPI_Send((void *)my_best_sol, element_num, MPI_INT, send_node, GROUP_SOLUTION, MPI_COMM_WORLD);
            break;
        case TABU_LIST_SHARE:
            element_num = get_send_recv_element_num();
            //send_node = list[random_num(DEFAULT_MPIGROUPNUM - 1)];
            for(i = 0; i < (DEFAULT_MPIGROUPNUM - 1); i++) {
                MPI_Send((void *)my_share_tabulist, element_num, MPI_INT, list[i], GROUP_SOLUTION, MPI_COMM_WORLD);
            }
            break;
    }
}

void copy_to_group_data(int * buffer, int element_num, int stac_num)
{
    int * other_group_sol = get_other_group_sol_path();
    int i;
    int start_point = stac_num * element_num;

    for(i = 0; i < element_num; i++) {
        other_group_sol[start_point + i] = buffer[i];
    }
}

void copy_to_share_tabulist(int * tabulist_buffer, int element_num)
{
    int i;
    int * share_tabulist;

    share_tabulist = get_tabulist_data();
    for(i = 0; i < element_num; i++) {
        share_tabulist[i] = tabulist_buffer[i];
    }
}

int decide_create_mode(void)
{
    int initialize_path_create_mode = DEFAULT;

    if(check_other_data_satisfactory() == YES) {
        initialize_path_create_mode = MERGECREATE;
        /*if(check_other_group_data_satisfactory(SOL_PATH_SHARE) == YES) {
            initialize_path_create_mode = GROUPCREATE;
        }*/
    }

    return initialize_path_create_mode;
}

int check_other_data_satisfactory(void)
{
    int i;
    int return_num = NO;
    int * other_sol_path;
    int group_num = get_num_of_all_proc() / DEFAULT_MPIGROUPNUM - 1;
    int data_cell_num = get_tsp_size() + DEFAULT_SENDPARAMETERNUM;

    if((other_sol_path = get_other_solution_path_data()) == NULL) {
        return return_num;
    }
    else {
        for(i = 0; i < group_num; i++) {
            if(other_sol_path[i * data_cell_num] == 0) {
                return return_num;
            }
        }
        return_num = YES;
    }

    return return_num;
}

int check_other_group_data_satisfactory(int type)
{
    int return_num = NO;

    switch(type) {
        case SOL_PATH_SHARE:
            if(get_other_group_stac_satisfaction() == ON) {
                return_num = YES;
            }
            break;
        case TABU_LIST_SHARE:
            return_num = share_tabulist_is_satisfactory();
            break;
    }

    return return_num;
}

void set_MPI_group(void)
{
    int group_num;
    int my_group;

    if(get_num_of_all_proc() < 4 || get_num_of_all_proc() % 4 != 0) {
        error_procedure("set_MPI_group()");
    }

    group_num = get_num_of_all_proc() / DEFAULT_MPIGROUPNUM;
    my_group = get_process_number() / group_num;
    set_MPI_group_data(group_num, my_group);
    create_same_group_list(group_num, my_group);
    pthread_mutex_init(&recv_sol_lock, NULL);
}

void create_same_group_list(int group_num, int my_group)
{
    int i,j = 0;
    int group_start_process;
    int * same_group_list;

    same_group_list = mallocer_ip(group_num - 1);
    group_start_process = my_group * group_num;
    for(i = group_start_process; i < (group_num + group_start_process); i++) {
        if(i != get_process_number()) {
            same_group_list[j] = i;
            j++;
        }
    }

    set_same_group_list(same_group_list);
    set_group_start_process(group_start_process);
}


void best_MPI_send(void)
{
    int my_process_num = get_process_number();
    int element_num = get_tsp_size() + DEFAULT_SENDPARAMETERNUM;
    int * my_best_sol = get_best_solution_path();
    int * other_list = get_same_group_list();
    int i;
    MPI_Status stat;
    MPI_Request req;

    if(check_manneri(FIRST_MIDDLEMODED) == YES) {
        /* DEL ST */
        check_send_data(my_best_sol, element_num);
        /* DEL EN */

        if(before_send_process_index >= (get_all_MPI_group_data() - 2)) {
            before_send_process_index = 0;
        }
        /* Send it-self best-solution-path to All other processes (High-Cost)
        for(i = 0; i < get_all_MPI_group_data() - 1; i++) {
            MPI_Send((void *)my_best_sol, element_num, MPI_INT, other_list[i], BEST_SOLUTION, MPI_COMM_WORLD);
        }*/

        /* Send it-self best-solution-path to Only-One process that chose by turn (Low-Cost)*/
        /* Blocking Send */
        MPI_Send((void *)my_best_sol, element_num, MPI_INT, other_list[before_send_process_index], BEST_SOLUTION, MPI_COMM_WORLD);

        /* Non-Blocking Send () */
        //MPI_Isend((void *)my_best_sol, element_num, MPI_INT, other_list[before_send_process_index], BEST_SOLUTION, MPI_COMM_WORLD, &req);
        //MPI_Wait(&req, &stat);

        before_send_process_index++;
#ifdef DEBUG
        mpi_comunication_log_manage(MPI_SENDADD);

        /* DEL ST */
        test_debug_log("+++ best_MPI_send() +++", -1);
        for(i = 0; i < element_num; i++) {
            test_debug_log("send_sol == ", my_best_sol[i]);
        }
        test_debug_log("+++++++++++++++++++++++", -1);
        /* DEL EN */
#endif
    }
}

void best_MPI_recv(int * recv_process_number)
{
    int i;
    int tsp_size = get_tsp_size();
    int element_num = tsp_size + DEFAULT_SENDPARAMETERNUM;
    int buffer[element_num];
    int * other_sol_path = get_other_solution_path_data();
    int * other_list = get_same_group_list();
    int this_threads_index = 0;
    int recvbuff_flag = 1;
    MPI_Status stat;
    MPI_Request req;

    for(i = 0; i < get_all_MPI_group_data() - 1; i++) {
        if(other_list[i] == (*recv_process_number)) {
            this_threads_index = i * element_num;
        }
    }

    pthread_mutex_init(&recv_sol_lock, NULL);

    for(;;) {
        if(get_all_search_is_done() == YES) {
            exit(0);
        }
        MPI_Iprobe(MPI_ANY_SOURCE, BEST_SOLUTION, MPI_COMM_WORLD, &recvbuff_flag, &stat);
        if(recvbuff_flag == 1) {
            /* Blocking Recv */
            MPI_Recv((void *)buffer, element_num, MPI_INT, MPI_ANY_SOURCE, BEST_SOLUTION, MPI_COMM_WORLD, &stat);

            /* Non-Blocking Recv (Low-Speed) */
            //MPI_Irecv((void *)buffer, element_num, MPI_INT, MPI_ANY_SOURCE, BEST_SOLUTION, MPI_COMM_WORLD, &req);
            //MPI_Wait(&req, &stat);

            pthread_mutex_lock(&recv_sol_lock);
            for(i = 0; i < element_num; i++) {
                other_sol_path[this_threads_index + i] = buffer[i];
            }
            pthread_mutex_unlock(&recv_sol_lock);

#ifdef DEBUG
            mpi_comunication_log_manage(MPI_RECVADD);
            output_other_sol_path();
#endif
        }

        /* this sleep is Hummmmmm... */
        usleep(100000);
    }
}

void check_send_data(int * send_data, int send_num)
{
    int i, tsp_size = get_tsp_size();
    int * used_city_list = mallocer_ip(tsp_size + 1);
    int zero_index = 0;
    int zero_count = 0;

    do {
        for(i = 0; i < tsp_size; i++) {
            if(send_data[i] == 0) {
                zero_index = i;
                zero_count++;
            }
            else {
                used_city_list[send_data[i]] = YES;
            }
        }

        if(zero_count != 0) {
            for(i = 1; i <= tsp_size; i++) {
                if(used_city_list[i] != YES) {
                    send_data[zero_index] = i;
                    zero_count--;
                    break;
                }
            }
        }
    } while(zero_count > 0);

    free(used_city_list);
}

int * get_merge_route(void)
{
    int * other_sol_path;
    int tsp_size = get_tsp_size();
    int element_num = tsp_size + DEFAULT_SENDPARAMETERNUM;
    int all_group_num = get_all_MPI_group_data();
    int choiced;
    int * return_data = get_solution_path();

    other_sol_path = get_other_solution_path_data();

    if(all_group_num < 2) {
        error_procedure("Can't merge from best-routes: parallel.c");
    }
    else {
        choiced = random_num(all_group_num - 1) - 1;

        /* other_sol_path[]'s Data-Format is [0] == city1, [1] == city2, ... [N - 1] == cityN
           return_data[]'s Data-Format is    [0] == N, [1] == city1, ... [N] == cityN */
        merge_route(return_data, other_sol_path, choiced);
    }

    return return_data;
}

/* "sol_path" is practically return data */
void merge_route(int * sol_path, int * other_sol, int choice)
{
    int * branchsA = get_branchA();
    int * branchsB = get_branchB();
    int * temp_path = get_temp_path();

    /* Adjust the basic-data (other_sol) to merge-formatted-data (temp_path) */
    adjust_branchs(branchsB, other_sol, temp_path, choice);
    /* Substitute city-num of basic-data ([sol|temp]_path)
       into branchs-formatted-array (branchs[A|B]) */
    set_branch_data(branchsA, sol_path); set_branch_data(branchsB, temp_path);
    sort_branch_data(branchsA); sort_branch_data(branchsB);
    /* Check each branches-formatted-data wheater Match or Not-Match */
    check_matching(branchsA, branchsB);
    /* Substitute merged data into "sol_path" mainly based other_sol,
       and make initial-path by something method (now, NB) */
    get_route_by_matched(sol_path, get_matchedB(), temp_path);
}

void how_long_matched(int * maximum, int * max_i, int * matchedB, int size)
{
    int i, counter = 0;

    for(i = 0; i < size; i++) {
        if(matchedB[i] == ON) {
            counter++;
        }
        else {
            if(*maximum < counter) {
                *maximum = counter;
                *max_i = i - 1;
            }
            counter = 0;
        }
    }
}

void get_route_by_matched(int * sol_path, int * matchedB, int * temp_path)
{
    int i, start_i;
    int maximum = 0;
    int max_i = 0;
    int size = get_tsp_size();
    int * used_cities;

    used_cities = mallocer_ip(size + 1);

    /* Get the maximum-num of the part of match-array */
    how_long_matched(&maximum, &max_i, matchedB, size);

#ifdef DEBUG
    figure_of_match_num(maximum);
#endif

    start_i = max_i + 1 - maximum;
    used_cities[0] = maximum;

    for(i = 1; i <= maximum; i++) {
        sol_path[i] = temp_path[i + start_i];
        used_cities[sol_path[i]] = ON;
    }

    /* create the leftover-path by Nearby Branch */
    initialize_leftovers_path(sol_path, maximum, used_cities);

    free(used_cities);
}

void initialize_leftovers_path(int * sol_path, int maximum, int * used_cities)
{
    int i;
    int size = get_tsp_size();
    int mode = DEFAULT;
    int now_city;
    int next_city;
    int best_city;
    double * graph_data = get_graph_data();
    double distance = DBL_MAX;
    double min_distance = DBL_MAX;

    switch(mode) {
        case DEFAULT:
            for(i = maximum; i < size; i++) {
                for(next_city = 1; next_city <= size; next_city++) {
                    if(used_cities[next_city] == OFF) {
                        if((distance = graph_data[now_city + size * next_city]) < min_distance) {
                            min_distance = distance;
                            best_city = next_city;
                        }
                    }
                }
                sol_path[i + 1] = best_city;
                used_cities[best_city] = ON;
                distance = DBL_MAX; min_distance = DBL_MAX;
            }
            break;
    }
}

void check_matching(int * branchsA, int * branchsB)
{
    int index1, index2;
    int size = get_tsp_size();
    int * matchedA = get_matchedA();
    int * matchedB = get_matchedB();

    initialize_matched(matchedA); initialize_matched(matchedB);

    for(index1 = 0; index1 < size; index1++) {
        for(index2 = 0; index2 < size; index2++) {
            if(branchsA[index1 * 2] == branchsB[index2 * 2]) {
                if(branchsA[index1 * 2 + 1] == branchsB[index2 * 2 + 1]) {
                    matchedA[index1] = ON; matchedB[index2] = ON;
                }
            }
        }
    }
}

void set_branch_data(int * branchs, int * path)
{
    int i;
    int size = get_tsp_size();

    for(i = 0; i < size; i++) {
        branchs[2 * i] = path[i + 1];
        if(i != (size - 1)) {
            branchs[2 * i + 1] = path[i + 2];
        }
        else {
            branchs[2 * i + 1] = path[1];
        }
    }
}

void initialize_matched(int * matched)
{
    int i;
    int size = get_tsp_size();

    for(i = 0; i < size; i++) {
        matched[i] = OFF;
    }
}

void adjust_branchs(int * branchs, int * other_sol, int * temp_path, int choice)
{
    int i;
    int size = get_tsp_size();
    int start = (size + DEFAULT_SENDPARAMETERNUM) * choice;

    temp_path[0] = size;

    for(i = 1; i <= size; i++) {
        temp_path[i] = other_sol[start + i - 1];
    }
}

void sort_branch_data(int * branchs)
{
    int i, buf;
    int size = get_tsp_size();

    for(i = 0; i < size; i++) {
        if(branchs[2 * i] > branchs[2 * i + 1]) {
            buf = branchs[2 * i];
            branchs[2 * i] = branchs[2 * i + 1];
            branchs[2 * i + 1] = buf;
        }
    }
}

/* return YES or NO */
int is_this_ok_same_group_list(int * list, int all_process)
{
    int i;
    int return_num = YES;
    int group_num = all_process / DEFAULT_MPIGROUPNUM - 1;

    for(i = 0; i < group_num; i++) {
        if(list[i] > all_process) {
            return_num = NO;
            break;
        }
    }

    return return_num;
}

/* DEL ST */
void show_saved_other_sol(void)
{
    int * other_sol_path = get_other_solution_path_data();
    int tsp_size = get_tsp_size();
    int element_num = tsp_size + DEFAULT_SENDPARAMETERNUM;
    int i,j;

    for(i = 0; i < (get_all_MPI_group_data() - 1); i++) {
        for(j = 0; j < element_num; j++) {
            printf("%d:index[%3d]:%d\n",i,j,other_sol_path[i * element_num + j]);
        }
    }
}
/* DEL EN */

int get_all_search_is_done_flag = NO;

int get_all_search_is_done(void)
{
    int * solution_path;
    int return_num = NO;

    if(get_all_search_is_done_flag == NO) {
        sleep(1);
        get_all_search_is_done_flag = YES;
    }
    solution_path = get_solution_path();

    if(solution_path[0] == 0) {
        return_num = YES;
    }

    return return_num;
}

void parallel_finalize(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    /*MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);*/
    MPI_Finalize();
    free(get_other_solution_path_data());
    free(get_same_group_list());
}

