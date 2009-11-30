#include "mpi_header.h"

void set_MPI_parameter(void);

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

    parameterp->num_of_all_proc = num_of_all_proc;
    parameterp->process_number = process_number;
    parameterp->name_length = name_length;
    parameterp->process_name = process_name;
}

void parallel_finalize(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
