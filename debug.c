/* header files */
#include "header.h"

#ifdef DEBUG
/* global variable */
char debug_log_name[128];
FILE * debug_fp;

#ifdef MPIMODE
struct mpi_comunication_log {
    int mpi_send_num;
    int mpi_recv_num;
};

struct mpi_comunication_log mpi_com_log;
#endif

/* functions */
void open_loging_initial_path(void);
void close_loging_initial_path(void);
void loging_initial_path(int * path, int create_mode);

#ifdef MPIMODE
void mpi_comunication_log_manage(int type);
void mpi_comunication_log_init(void); /* local function */
void mpi_send_num_add(void); /* local function */
void mpi_recv_num_add(void); /* local function */
void loging_mpi_com(void); /* local function */
#endif

void open_loging_initial_path(void)
{
    if(get_process_number() < 10) {
         sprintf(debug_log_name, "debug_log/initial_path.0%d.log",get_process_number());
    }
    else {
         sprintf(debug_log_name, "debug_log/initial_path.%d.log",get_process_number());
    }

    if((debug_fp = fopen(debug_log_name, "a")) == NULL) {
        error_procedure("can\'t find \"debug_log\" directory");
    }
}

void loging_initial_path(int * path, int create_mode)
{
    int i;

    fprintf(debug_fp, "*** initial_path debug START ***\n");
    if(create_mode == DEFAULT) {
        fprintf(debug_fp, "=== DEFAULT MODE ===\n");
    }
    else {
        fprintf(debug_fp, "=== MERGECREATE MODE ===\n");
    }
    for(i = 0; i < get_tsp_size(); i++) {
        fprintf(debug_fp, "No.%3d> city(%3d)\n",i, path[i]);
    }
    fprintf(debug_fp, "*** initial_path debug END ***\n");
}

#ifdef MPIMODE
void mpi_comunication_log_manage(int type)
{
    switch(type) {
        case INIT:
            mpi_comunication_log_init();
            break;
        case MPI_SENDADD:
            mpi_send_num_add();
            break;
        case MPI_RECVADD:
            mpi_recv_num_add();
            break;
        case CHECK:
            loging_mpi_com();
            break;
    }
}

void loging_mpi_com(void)
{
    fprintf(debug_fp, "*** MPI send/recv communication debug START ***\n");
    fprintf(debug_fp, "Number of MPI_Send:%d\n", mpi_com_log.mpi_send_num);
    fprintf(debug_fp, "Number of MPI_Recv:%d\n", mpi_com_log.mpi_recv_num);
    fprintf(debug_fp, "*** MPI send/recv communication debug END ***\n");
}

void mpi_comunication_log_init(void)
{
    mpi_com_log.mpi_send_num = 0;
    mpi_com_log.mpi_recv_num = 0;
}

void mpi_send_num_add(void)
{
    mpi_com_log.mpi_send_num++;
    fprintf(debug_fp, "*** MPI send !!! ***\n");
}

void mpi_recv_num_add(void)
{
    mpi_com_log.mpi_recv_num++;
    fprintf(debug_fp, "*** MPI recv !!! ***\n");
}
#endif

void close_loging_initial_path(void)
{
    fclose(debug_fp);
}

#endif
