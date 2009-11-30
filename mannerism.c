/* header files */
#include "header.h"

/* functions */
int check_manneri(int type);
int check_fill(void);
int check_historical_similar(void);

void set_counter(void);
int get_counter(void);
double get_best_cost(void);

/* global variable */
int solution_count;
double best_distance;


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
void set_counter(void)
{
    solution_count = 0;
    best_distance = 0;
}

int get_counter(void)
{
    if (best_distance == get_best_cost())
        solution_count++;                
    
    else if (best_distance != get_best_cost())
        solution_count = 0;

        best_distance = get_best_cost();
        
        return(solution_count);
}

int middle_manneri(void)
{
    if(get_counter() > 100)
        return YES;
        
    else return NO;
}
