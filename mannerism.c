/* header files */
#include "header.h"

/* functions */
void mannneri_initialize(void);
void long_manneri_initialize(void);
int long_manneri(int type);
void mannneri_finalize(void);
double how_long_search_now(void);
int check_long_manneri(void);
int check_manneri(int type);
int check_fill(void);
int check_historical_similar(void);
int get_counter(void);
void error_procedure(char * message);
void set_counter(void);
double get_best_cost(void);
int get_have_been_mid_mode(void);
void set_have_been_mid_mode(void);
int get_tabu_clear_count(void);
void set_tabu_clear_count(void);


/* global variable */
struct mid_manneri_parameter {
    int solution_count;
    double best_distance;
    int have_been_mid_mode;
    int tabu_clear_count;
};

struct long_manneri_parameter {
    double best_distance;
    double prev_distance;
    int before_time;
};

struct mid_manneri_parameter * mid_parameterp;
struct long_manneri_parameter long_manneri_parameter;

void mannneri_initialize(void)
{
    if((mid_parameterp = (struct mid_manneri_parameter *)malloc(sizeof(struct mid_manneri_parameter))) == NULL) {
        error_procedure("mid_manneri_parameter malloc()");
    }
    set_counter();
    long_manneri_initialize();
}

void long_manneri_initialize(void)
{
    long_manneri_parameter.best_distance = DBL_MAX;
    long_manneri_parameter.prev_distance = DBL_MAX;
    long_manneri_parameter.before_time = 0; /* int */
}

/* return YES or NO */
int check_manneri(int type)
{
    int return_num = NO;

    switch(type) {
        case SHORTMODE:
            return_num = short_manneri();
            break;
        case MIDDLEMODE:
            return_num = middle_manneri();
            break;
        case FIRST_MIDDLEMODED:
            return_num = get_have_been_mid_mode();
            break;
        case LONGMODE:
            return_num = check_long_manneri();
            break;
    }

    return return_num;
}

int short_manneri(void)
{
    if(check_fill() == NO) {
        return NO;
    }

    return check_historical_similar();
}

/* return YES or NO */
int middle_manneri(void)
{
    if(modep->pole_mode == ON) {
        if(check_manneri(SHORTMODE) == YES){
            if(get_counter() > GA_COUNTER) {
                return YES;
            }
        }
        else {
            return NO;
        }
    }
    else {
        if(get_counter() > DEFAULT_MIDDLEMANNNERI) {
            if(get_have_been_mid_mode() == NO) {
                set_have_been_mid_mode();
            }
            return YES;
        }
        else {
            return NO;
        }
    }
}

/* return "YES" or "NO" */
int long_manneri(int type)
{
    int return_num = NO;

    switch(type) {
        case CHECK:
            return_num = check_manneri(LONGMODE);
            break;
    }

    return return_num;
}

/* return "YES" or "NO" */
int check_long_manneri(void)
{
    int return_num = NO;
    int next_check_time = long_manneri_parameter.before_time + 1;
    double running_time = how_long_search_now();
    double minutes;

    minutes = running_time / LONGMANNERI_INTERBAL;

    if(next_check_time < minutes) {

        /*DEL*/
        printf("LONG MANNERI TEST\n");
        /*DEL*/

        long_manneri_parameter.before_time++;
        long_manneri_parameter.best_distance = get_best_cost();
        if(long_manneri_parameter.prev_distance != long_manneri_parameter.best_distance) {
            long_manneri_parameter.prev_distance = long_manneri_parameter.best_distance;
        }
        else {
            return_num = YES;
        }
    }

    return return_num;
}


int get_have_been_mid_mode(void)
{
    return mid_parameterp->have_been_mid_mode;
}

void set_have_been_mid_mode(void)
{
    mid_parameterp->have_been_mid_mode = YES;
}

void set_counter(void)
{
    mid_parameterp->solution_count = 0;
    mid_parameterp->best_distance = 0;
    mid_parameterp->have_been_mid_mode = NO;
}

int get_counter(void)
{
    if(mid_parameterp->best_distance == get_best_cost()) {
        mid_parameterp->solution_count++;
    }
    else if(mid_parameterp->best_distance != get_best_cost()) {
        mid_parameterp->solution_count = 0;
    }

    mid_parameterp->best_distance = get_best_cost();

    return mid_parameterp->solution_count;
}

int get_tabu_clear_count(void)
{
    mid_parameterp->tabu_clear_count++;
    return mid_parameterp->tabu_clear_count;
}
void set_tabu_clear_count(void)
{
    mid_parameterp->tabu_clear_count = 0;
}
void mannneri_finalize(void)
{
    free(mid_parameterp);
}
