/* header files */
#include "header.h"

/* functions */
void final_result_show(FILE * fp);
void finalize(void);
double * get_graph_data(void);
int * get_main_base_data(void);
struct parameter * get_parameterp(void);
void output_log(void);
void error_procedure(char * message);

#ifdef MPIMODE
void parallel_finalize(void);
#endif

void finalize(void)
{
    final_result_show(stdout);
    //#ifdef MPIMODE
    output_log();
    //#endif

    #ifdef MPIMODE
    if(modep->parallel_mode) {
    }
    else {
        parallel_finalize();
    }
    #endif

    if(modep->graph_mode == ON) {
        free(get_graph_data());
    }
    free(get_main_base_data());
    free(get_parameterp());
    printf("Program is normally terminated.....\n");
}

void output_log(void)
{
    char time_data[64];
    char logfilename[128];
    time_t timer;
    struct tm * date;
    FILE * fp;

    timer = time(NULL);
    date = localtime(&timer);
    strftime(time_data, 63, "log_data/%Y%m%d_%H:%M:%S",date);
    sprintf(logfilename, "%s.node:%d.log",time_data,get_process_number());
    if((fp = fopen(logfilename, "w")) == NULL) {
        error_procedure("can\'t find \"log_data\" directory");
    }
    final_result_show(fp);
    fclose(fp);
}
