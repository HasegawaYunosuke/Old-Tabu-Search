/* header files */
#include "header.h"

/* functions */
void set_time(int mode, int parametor);
double get_time(void);
void set_start_time(time_t start_time);

#ifdef MPIMODE
int get_process_number(void);
#endif

/* grobal variable */
double search_time = DEFAULT_SEARCHTIME;
double start_time;

void set_time(int mode, int parametor)
{
    search_time = (double)parametor;
}

double get_time(void)
{
    return search_time;
}

int timer(int sign)
{
    struct timeval t;
    double now_time;
    int ack;

    /* get "now_time" */
    gettimeofday(&t, NULL);
    now_time = (double)t.tv_sec + (double)t.tv_usec * 1e-6;

    /* when the timer Start */
    if(sign == ON) {
        start_time = now_time;

        #ifdef MPIMODE
        /* temporaly data defines */
        MPI_Status stat;
        int element_num = 1;
        int root_process_number = 1;
        int buffer = (int)start_time;
        int ierror;

        if(get_process_number() != root_process_number) {
            MPI_Recv((void *)buffer, element_num, MPI_INT, &root_process_number, LOGFILENAME, MPI_COMM_WORLD, &stat);
            start_time = (time_t)buffer;
        }
        else {
            MPI_Bcast((void *)buffer, element_num, MPI_INT, root_process_number, MPI_COMM_WORLD, ierror);
        }
        #endif

        set_start_time(time(NULL));
    }
    /* if "now_time" is larger than "search_time", send "OFF" mean stop program */
    else if(sign == CHECK) {
        if((now_time - start_time) < search_time) {
            ack = ON;
        }
        else {
            ack = OFF;
        }
    }

    return ack;
}
