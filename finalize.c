/* header files */
#include "header.h"

/* functions */
void finalize(void);
void final_result_show(void);
double * get_graph_data(void);
int * get_main_base_data(void);
struct parameter * get_parameterp(void);

void finalize(void)
{
    final_result_show();

    if(modep->euclid_mode == ON) {
    }
    else {
        free(get_graph_data());
    }
    free(get_main_base_data());
    free(get_parameterp());

    printf("Program is normally terminated.....\n");
}
