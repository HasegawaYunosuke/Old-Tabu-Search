/* header files */
#include "header.h"

/* functions */
int * initial_euclid_path(int * euclid_data);
int * initial_graph_path(double * graph_data);
int search_loop_times(int type);
int * mallocer_ip(int size);

int * initial_euclid_path(int * euclid_data)
{
    int * return_data;

    if(search_loop_times(READONLY) == 0) {
        return_data = mallocer_ip(euclid_data[0]);
        printf("init_path by euclid\n");
    }

    return return_data;
}

int * initial_graph_path(double * graph_data)
{
    int * return_data;

    if(search_loop_times(READONLY) == 0) {
        return_data = mallocer_ip(graph_data[0]);
        printf("init_path by graph\n");
    }

    return return_data;
}
