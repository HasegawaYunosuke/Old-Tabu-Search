/* header files */
#include "header.h"

/* functions */
void set_time(int mode, int parametor);
double get_time(void);
void set_start_time(time_t start_time);

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
