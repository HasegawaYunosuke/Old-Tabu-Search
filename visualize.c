/* header files */
#include "header.h"

/* functions */
void visualizer(int * visual_arg);
int * get_solution_path(void);
int * get_main_base_data(void);

void visualizer(int * visual_arg)
{
    int i;

    pthread_mutex_lock(&mutex);

    /*for(i = 0; i < 100; i++) {
        printf("main_base_data[%d] == %d\n",i,main_base_data[i]);
    }*/

    pthread_mutex_unlock(&mutex);
}
