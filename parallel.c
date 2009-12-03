#include "header.h"

void set_MPI_parameter(void);
void set_MPI_group(void);
void set_parameter_data(int num_of_all_proc, int process_number, int name_length, char * process_name);
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
}

void parallel_finalize(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

int set_MPI_group_times = 0;

void set_MPI_group(void)
{
    int num_of_all_proc = get_num_of_all_proc();
    int process_number = get_process_number();
    char * process_name = get_process_name();

    set_MPI_group_times++;

    printf("%d:process_name == %s\n",set_MPI_group_times,process_name);
}
