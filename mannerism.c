/* header files */
#include "header.h"

/* functions */
void mannneri_initialize(void);
void mannneri_finalize(void);
int check_manneri(int type);
int check_fill(void);
int check_historical_similar(void);
int get_counter(void);
void error_procedure(char * message);
void set_counter(void);
double get_best_cost(void);
int get_have_been_mid_mode(void);
void set_have_been_mid_mode(void);

/* global variable */
struct mid_manneri_parameter {
    int solution_count;
    double best_distance;
    int have_been_mid_mode;
};

struct mid_manneri_parameter * mid_parameterp;

void mannneri_initialize(void)
{
    if((mid_parameterp = (struct mid_manneri_parameter *)malloc(sizeof(struct mid_manneri_parameter))) == NULL) {
        error_procedure("mid_manneri_parameter malloc()");
    }
    set_counter();
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
    if(modep->ga_mode == ON || modep->pole_mode == ON) {
        if(get_counter() > GA_COUNTER) {
            return YES;
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

void mannneri_finalize(void)
{
    free(mid_parameterp);
}
