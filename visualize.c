/* header files */
#include "header.h"

/* functions */
void visualizer(int * visual_arg);

/* grobal variable */
int * main_base_data;

void visualizer(int * visual_arg)
{
    int i;

    pthread_mutex_lock(&mutex);

    /*for(i = 0; i < 100; i++) {
        printf("main_base_data[%d] == %d\n",i,main_base_data[i]);
    }*/

    pthread_mutex_unlock(&mutex);
}
