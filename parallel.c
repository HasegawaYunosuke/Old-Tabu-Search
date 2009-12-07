#include "header.h"

void set_MPI_parameter(void);
void set_MPI_group(void);
void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name);
void set_MPI_group_data(int group_num, int my_group);
int get_num_of_all_proc(void);
int get_process_number(void);
char * get_process_name(void);

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
}

void parallel_finalize(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

void set_MPI_group(void)
{
    int num_of_all_proc = get_num_of_all_proc();
    int process_number = get_process_number();
    char * process_name = get_process_name();
    int group_num;
    int my_group;

    if(num_of_all_proc < 4 || num_of_all_proc % 4 != 0) {
        error_procedure("set_MPI_group()");
    }

    group_num = num_of_all_proc / DEFAULT_MPIGROUPNUM;
    my_group = process_number / group_num;
    set_MPI_group_data(group_num, my_group);
}
