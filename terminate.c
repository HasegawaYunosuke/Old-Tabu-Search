/* header files */
#include "header.h"

/* functions */
int search_is_done(int type);
int loop_terminate(void);
void turn_terminated_show(void);
void turn_terminated_by_time_show(void);
void search_terminated_by_time_show(void);
int search_terminate(void);
int turn_loop_times(int type);
int search_loop_times(int type);
int get_tsp_size(void);
int timer(int type);
void create_2opt_tabulist(int tsp_size, int mode);
int get_tsp_size(void);

int loop_terminate(void)
{
    int return_num = NO;

    if(turn_loop_times(CHECK) >= get_tsp_size() * 10 && get_tabu_mode() == YES && modep->pole_mode == OFF) {
        turn_loop_times(INIT);
        turn_terminated_show();
        return_num = YES;
        set_tabu_mode(OFF);
        if(modep->hasegawa_mode == ON || modep->tozaki_mode == ON) {
            create_2opt_tabulist(get_tsp_size(), CLEAR);
        }
    }
    if(timer(CHECK) == OFF) {
        turn_loop_times(INIT);
        turn_terminated_by_time_show();
        return_num = YES;
    }

    return return_num;
}

int search_terminate(void)
{
    int return_num = NO;

    if(timer(CHECK) == OFF) {
        search_terminated_by_time_show();
        return_num = YES;
        search_is_done(INIT);
    }

    search_loop_times(CHECK);

    return return_num;
}
