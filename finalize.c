/* header files */
#include "header.h"

/* functions */
void final_result_show(FILE * fp);
void finalize(void);
void mannneri_finalize(void);
double * get_graph_data(void);
int * get_main_base_data(void);
struct parameter * get_parameterp(void);
void output_log(void);
time_t get_start_time(void);
void error_procedure(char * message);
int * mallocer_ip(int size);
int search_loop_times(int type);
void create_filename(char * logfilename);
void free_tabu(void);
void free_parameterp(void);
void free_historyp(void);

#ifdef MPIMODE
void parallel_finalize(void);
void MPI_final_result_show(FILE * fp, int * recv_data, int send_data_num);
void set_logfile_name(int * buffer, int element_num);
int get_process_number(void);
int get_num_of_all_proc(void);
void init_send_final_data(int * send_data, int send_data_num);
void free_MPI_recv_thread(void);
#endif
#ifdef DEBUG
void close_loging_initial_path(void);
#ifdef MPIMODE
void mpi_comunication_log_manage(int type);
void tabu_matching_loging(int type);
#ifdef POLEDEBUG
void close_loging_other_sol_path(void);
#endif
#endif
#endif
#ifdef CROSSOVER_BEF_AFT
void close_loging_x_sol_path(void);
#endif

/* global variable */
int * lnp;

void finalize(void)
{
    //final_result_show(stdout);
    output_log();

    #ifdef DEBUG
    close_loging_initial_path();
    #endif
    #ifdef MPIMODE
        #ifdef POLEDEBUG
        close_loging_other_sol_path();
        #endif
    #endif
    #ifdef CROSSOVER_BEF_AFT
    close_loging_x_sol_path();
    #endif

    mannneri_finalize();
    free_historyp();
    free_tabu();
    #ifdef MPIMODE
    if(modep->parallel_mode == ON) {
        free_MPI_recv_thread();
        parallel_finalize();
    }
    #endif
    free_parameterp();
}

void set_logfile_name(int * buffer, int element_num)
{
    int i;

    lnp = mallocer_ip(element_num);
    for(i = 0; i < element_num; i++) {
        lnp[i] = buffer[i];
    }
}

void output_log(void)
{
    FILE * fp;
    char logfilename[128];
    int send_data_num = 8;
    int send_data[send_data_num]; /* ProcNum, GroupNum, BestCost, SearchCount, TurnCount, Local/ShareTabuListNum */
    int recv_data[256];

    create_filename(logfilename);

    #ifdef MPIMODE
    init_send_final_data(send_data, send_data_num);
    MPI_Allgather(send_data, send_data_num,
        MPI_INT, recv_data, send_data_num, MPI_INT, MPI_COMM_WORLD);
    if(get_process_number() == 0) {
        if((fp = fopen(logfilename, "w")) != NULL) {
            MPI_final_result_show(fp, recv_data, send_data_num);
            MPI_final_result_show(stdout, recv_data, send_data_num);
            fclose(fp);
        }
    }
    #else
    if((fp = fopen(logfilename, "w")) != NULL) {
        final_result_show(fp);
        fclose(fp);
    }
    #endif

    #ifdef MPIMODE
        #ifdef DEBUG
        if(modep->parallel_mode == ON) {
            mpi_comunication_log_manage(CHECK);
            tabu_matching_loging(CHECK);
            #ifdef SEND_AMONGGROUP
            if(get_group_reader() == get_process_number()) {
                tabu_matching_loging(SHARE);
            }
            #endif
        }
        #endif
    #endif
}

void create_filename(char * logfilename)
{
    char time_data[64];
    time_t timer;
    struct tm * date;

    date = localtime(&timer);
    #ifdef MPIMODE
    if(modep->parallel_mode == ON) {
        sprintf(time_data,"log_data/%d.%d.%d_%d.%d.%d",lnp[0], lnp[1], lnp[2], lnp[3], lnp[4], lnp[5]);
    }
    #else
    strftime(time_data, 63, "log_data/%Y.%m.%d_%H.%M.%S",date);
    #endif

    if(get_process_number() < 10) {
        sprintf(logfilename, "%s.node0%d.log",time_data,get_process_number());
    }
    else {
        sprintf(logfilename, "%s.node%d.log",time_data,get_process_number());
    }
}
