/* header files */
#include "header.h"

/* functions */
void set_time(int mode, int parametor);
double get_time(void);
void set_start_time(time_t start_time);

#ifdef MPIMODE
int get_process_number(void);
void set_logfile_name(int * buffer, int element_num);
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
        if(modep->parallel_mode == ON) {
            /* temporaly data defines */
            MPI_Status stat;
            int element_num = 6;
            int root_process_number = 0;
            int buffer[element_num];
            time_t tmm;
            struct tm *tms;

            if(get_process_number() != root_process_number) {
                MPI_Bcast((void *)buffer, element_num, MPI_INT, root_process_number, MPI_COMM_WORLD);
                set_logfile_name(buffer, element_num);
            }
            else {
                (void)time(&tmm); tms = gmtime(&tmm);
                buffer[0] = 1900 + tms->tm_year;
                buffer[1] = tms->tm_mon + 1;
                buffer[2] = tms->tm_mday;
                buffer[3] = tms->tm_hour;
                buffer[4] = tms->tm_min;
                buffer[5] = tms->tm_sec;
                MPI_Bcast((void *)buffer, element_num, MPI_INT, root_process_number, MPI_COMM_WORLD);
                set_logfile_name(buffer, element_num);
            }
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
