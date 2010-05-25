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

#ifdef MPIMODE
void parallel_finalize(void);
void set_logfile_name(int * buffer, int element_num);
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
    final_result_show(stdout);
    output_log();

    #ifdef MPIMODE
    if(modep->parallel_mode == ON) {
        parallel_finalize();
    }
    #endif

    free(get_graph_data());

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
    free(get_main_base_data());
    free(get_parameterp());
    mannneri_finalize();

    //printf("Program is normally terminated.....\n");
}

void set_logfile_name(int * buffer, int element_num)
{
    int i;

    lnp = mallocer_ip(element_num);
    for(i = 0; i < element_num; i++) {
        lnp[i] = buffer[i];
    }
}

/*DEL*/
int get_process_number(void);
/*DEL*/

void output_log(void)
{
    char time_data[64];
    char logfilename[128];
    time_t timer;
    struct tm * date;
    FILE * fp;

    timer = get_start_time();
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
    if((fp = fopen(logfilename, "w")) != NULL) {
        //sleep(get_process_number());
        final_result_show(fp);
        fclose(fp);
    }

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
