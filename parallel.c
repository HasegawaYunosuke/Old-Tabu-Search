#include "header.h"

int * mallocer_ip(int size);
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
void final_result_show(FILE * fp);
/* DEL ST */
void show_saved_other_sol(void);
/* DEL EN */

/* grobal variable */
pthread_mutex_t recv_sol_lock;

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
}

void set_other_solution_path(void)
{
    int create_num = get_all_MPI_group_data() - 1;
    int * save_pointer;

    save_pointer = mallocer_ip((get_tsp_size() + 10) * create_num);
    set_other_solution_path_data(save_pointer);
}

void parallel_finalize(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    free(get_other_solution_path_data());
    free(get_same_group_list());
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

#define BEST_SOLUTION 101

void best_MPI_send(void)
{
    int my_process_num = get_process_number();
    int element_num = get_tsp_size() + 10;
    int * my_best_sol = get_best_solution_path();
    int * other_list = get_same_group_list();
    int i;
    MPI_Status stat;

    #ifdef MPIMODE
//    if(check_manneri(MIDDLEMODE) == YES) {
        for(i = 0; i < get_all_MPI_group_data() - 1; i++) {
            MPI_Send((void *)my_best_sol, element_num, MPI_INT, other_list[i], BEST_SOLUTION, MPI_COMM_WORLD);
        }
//    }
    #endif
}


void best_MPI_recv(int * recv_process_number)
{
    int i;
    int tsp_size = get_tsp_size();
    int element_num = tsp_size + 10;
    int buffer[TSPMAXSIZE];
    int * other_sol_path = get_other_solution_path_data();
    int * other_list = get_same_group_list();
    int this_threads_index = 0;
    MPI_Status stat;

    for(i = 0; i < get_all_MPI_group_data() - 1; i++) {
        if(other_list[i] == (*recv_process_number)) {
            this_threads_index = i * element_num;
        }
    }

    for(;;) {
        MPI_Recv((void *)buffer, element_num, MPI_INT, (*recv_process_number), BEST_SOLUTION, MPI_COMM_WORLD, &stat);
        pthread_mutex_lock(&recv_sol_lock);
        for(i = 0; i < element_num; i++) {
            other_sol_path[this_threads_index + i] = buffer[i];
        }
        pthread_mutex_unlock(&recv_sol_lock);
        break;
    }
}

int * get_merge_route(void)
{
}

/* DEL ST */
void show_saved_other_sol(void)
{
    int * other_sol_path = get_other_solution_path_data();
    int tsp_size = get_tsp_size();
    int element_num = tsp_size + 10;
    int i,j;

    for(i = 0; i < (get_all_MPI_group_data() - 1); i++) {
        for(j = 0; j < element_num; j++) {
            printf("%d:index[%3d]:%d\n",i,j,other_sol_path[i * element_num + j]);
        }
    }
}
/* DEL EN */
