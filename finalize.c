/* header files */
#include "header.h"

/* functions */
void final_result_show(FILE * fp);
void finalize(void);
double * get_graph_data(void);
int * get_main_base_data(void);
struct parameter * get_parameterp(void);
void output_log(void);

#ifdef MPIMODE
void parallel_finalize(void);
#endif

void finalize(void)
{
    final_result_show(stdout);
    output_log();

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
