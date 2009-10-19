/* header files */
#include "header.h"

/* functions */
void set_mode(void);
void set_euclid_mode(void);
void set_visual_mode(void);
void set_parallel_mode(void);
void set_pole_mode(void);
void set_tozaki_mode(void);
void show_mode(void);
void show_on_off(int on_off);
void initial_parameter(int tsp_size);
int turn_loop_times(int type);

/* grobal variable */
struct parameter {
    int tsp_size;               /* TSP's Example Size */
    int city_point;
    double permit_worse;        /* Parcentage of permitting to choice toward worse */
    double search_time;         /* whole program running time */
};

struct parameter * parameterp;
int turn_times;
int search_times;

/* set all modes to OFF */
void set_mode(void)
{
    /* allocate "pointer of struct" memory */
    if((modep = malloc(sizeof(struct mode))) == NULL) {
        error_procedure("mode malloc()");
    }

    modep->graph_mode = ON;
    modep->hasegawa_mode = ON;
}

void set_euclid_mode(void)
{
    modep->euclid_mode = ON;
    modep->graph_mode = OFF;
}

void set_visual_mode(void)
{
    modep->visual_mode = ON;
}

void set_parallel_mode(void)
{
    modep->parallel_mode = ON;
}

void set_pole_mode(void)
{
    modep->pole_mode = ON;
    modep->tozaki_mode = OFF;
    modep->hasegawa_mode = OFF;
}

void set_tozaki_mode(void)
{
    modep->tozaki_mode = ON;
    modep->pole_mode = OFF;
    modep->hasegawa_mode = OFF;
}

void initial_parameter(int tsp_size)
{
    /* allocate "pointer of struct" memory */
    if((parameterp = malloc(sizeof(struct parameter))) == NULL) {
        error_procedure("parameter malloc()");
    }

    parameterp->tsp_size = tsp_size;
    parameterp->permit_worse = DEFAULT_PERMITWORSE;
    parameterp->city_point = DEFAULT_CITYPOINT;
    turn_times = 0;
    search_times = 0;
}

int turn_loop_times(int type)
{
    int return_num;

    if(type == CHECK) {
        turn_times++;

        return_num = turn_times;
    }
    else if(type == INIT) {
        turn_times = 0;
    }
    else if(type == READONLY) {
        return_num = turn_times;
    }

    return return_num;
}

int search_loop_times(int type)
{
    int return_num;

    if(type == CHECK) {
        search_times++;

        return_num = search_times;
    }
    else if(type == READONLY) {
        return_num = search_times;
    }

    return return_num;
}

void set_city_point(int * point_of_cities)
{
    int i;

    for(i = 1; i <= point_of_cities[0]; i++) {
        point_of_cities[i] = parameterp->city_point;
    }
}

void show_mode(void)
{
    printf("visual_mode == "); show_on_off(modep->visual_mode);
    printf("graph_mode == "); show_on_off(modep->graph_mode);
    printf("euclid_mode == "); show_on_off(modep->euclid_mode);
    printf("parallel_mode == "); show_on_off(modep->parallel_mode);
    printf("hasegawa_mode == "); show_on_off(modep->hasegawa_mode);
    printf("pole_mode == "); show_on_off(modep->pole_mode);
    printf("tozaki_mode == "); show_on_off(modep->tozaki_mode);
}

void show_on_off(int on_off)
{
    if(on_off == ON) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}
