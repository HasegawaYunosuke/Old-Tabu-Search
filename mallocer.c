/* header files */
#include "header.h"

/* functions */
int * mallocer_ip(int size);
double * mallocer_dp(int size);

/* allocate "pointer of int" memory */
int * mallocer_ip(int size)
{
    int * return_p;

    printf("mallocer.c:size == %d\n",size);
    if((return_p = calloc(size,sizeof(int))) == NULL) {
        error_procedure("mallocer_ip()");
    }

    return return_p;
}

double * mallocer_dp(int size)
{
    double * return_p;

    if((return_p = calloc(size,sizeof(double))) == NULL) {
        error_procedure("mallocer_dp()");
    }

    return return_p;
}
