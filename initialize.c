/* header files */
#include "header.h"

/* functions */
double * make_graph(int * main_base_data);
double * mallocer_dp(int size);
double make_distance(int x1, int y1, int x2, int y2);
void set_graph_data(double * graph_data);

double * make_graph(int * main_base_data)
{
    double * return_data;
    double len;
    int i,j,xi,yi,xj,yj,size;

    size = main_base_data[0];

    return_data = mallocer_dp(size * (size + 1));

    return_data[0] = size;
    for(i = 1; i <= size; i++) {
        xi = main_base_data[i * 2];  yi = main_base_data[i * 2 + 1];
        for(j = 1; j <= size ; j++) {
            xj = main_base_data[j * 2];  yj = main_base_data[j * 2 + 1];
            len = make_distance(xi, yi, xj, yj);
            return_data[i + size * j] = len;
        }
    }

    set_graph_data(return_data);

    return return_data;
}
