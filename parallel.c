#include "header.h"

int * mallocer_ip(int size);
void set_MPI_parameter(void);
void set_MPI_group(void);
void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name);
void set_MPI_group_data(int group_num, int my_group);
void set_same_group_list(int * list);
void set_other_solution_path(void);
void set_other_solution_path_data(int * solution_path);
int get_num_of_all_proc(void);
int get_process_number(void);
int get_tsp_size(void);
int get_all_MPI_group_data(void);
int * get_same_group_list(void);
char * get_process_name(void);
void best_MPI_send(void);

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
    /*DEL ST*/
    best_MPI_send();
    /*DEL EN*/
}

void set_other_solution_path(void)
{
    int create_num = get_all_MPI_group_data() - 1;
    int tsp_size = get_tsp_size();
    int * save_pointer;

    save_pointer = mallocer_ip(tsp_size * create_num * 2);

    set_other_solution_path_data(save_pointer);
}

void parallel_finalize(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

void set_MPI_group(void)
{
    int i,j = 0;
    int num_of_all_proc = get_num_of_all_proc();
    int process_number = get_process_number();
    char * process_name = get_process_name();
    int group_num;
    int my_group;
    int group_start_process;
    int * same_group_list;

    if(num_of_all_proc < 4 || num_of_all_proc % 4 != 0) {
        error_procedure("set_MPI_group()");
    }

    group_num = num_of_all_proc / DEFAULT_MPIGROUPNUM;
    my_group = process_number / group_num;
    set_MPI_group_data(group_num, my_group);

    same_group_list = mallocer_ip(group_num - 1);
    group_start_process = my_group * DEFAULT_MPIGROUPNUM;
    for(i = group_start_process; i < (group_num + group_start_process); i++) {
        if(i != process_number) {
            same_group_list[j] = i;
            j++;
        }
    }
    set_same_group_list(same_group_list);
    free(same_group_list);
}

#define BEST_SOLUTION 101

void best_MPI_send(void)
{
    int my_process_num = get_process_number();
    int send_process_name;
    int recv_process_name;
    int ary[5] = {0};
    int i;
    MPI_Status stat;

    if(get_process_number() == 0) {
        for(i = 0; i < 5; i++) {
            ary[i] = i;
            printf("send:ary[%d] == %d\n",i,ary[i]);
        }
        send_process_name = 1;
        MPI_Send((void *)ary, 5, MPI_INT, send_process_name, BEST_SOLUTION, MPI_COMM_WORLD);
    }
    else if(get_process_number() == 1){
        recv_process_name = 0;
        for(i = 0; i < 5; i++) {
            printf("bef;recv:ary[%d] == %d\n",i,ary[i]);
        }
        MPI_Recv((void *)ary, 5, MPI_INT, recv_process_name, BEST_SOLUTION, MPI_COMM_WORLD, &stat);
        for(i = 0; i < 5; i++) {
            printf("recv:ary[%d] == %d\n",i,ary[i]);
        }
    }
}
