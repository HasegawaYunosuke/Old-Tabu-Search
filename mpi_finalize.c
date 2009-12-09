/* header files */
#include "header.h"

/* functions */
void final_result_show(void);
void finalize(void);
void parallel_finalize(void);
double * get_graph_data(void);
int * get_main_base_data(void);
struct parameter * get_parameterp(void);

void finalize(void)
{
    final_result_show();

    if(modep->parallel_mode) {
        parallel_finalize();
    }

    if(modep->euclid_mode == ON) {
    }
    else {
        free(get_graph_data());
    }
    free(get_main_base_data());

    free(get_parameterp());
    printf("Program is normally terminated.....\n");
}
