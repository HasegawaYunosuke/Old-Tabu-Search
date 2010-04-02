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
void set_middle_mannneri(int on_or_off);
void create_2opt_tabulist(int tsp_size, int mode);
int get_tsp_size(void);
int * get_solution_path(void);
void tell_terminate_to_visualize(void);
void tell_terminate_to_parallel(void);

int loop_terminate(void)
{
    int return_num = NO;

    if(turn_loop_times(CHECK) >= get_tsp_size() * 10 && get_tabu_mode() == YES && modep->pole_mode == OFF) {
        turn_loop_times(INIT);
        turn_terminated_show();
        return_num = YES;
        set_tabu_mode(OFF);
        if(modep->hasegawa_mode == ON || modep->tozaki_mode == ON) {
            if((num_counter(SEARCH_COUNTER, CHECK) % 3) == 0) {
                create_2opt_tabulist(get_tsp_size(), CLEAR);
            }
            set_middle_mannneri(OFF);
        }
    }
    if(timer(CHECK) == OFF) {
        if(modep->pole_mode == OFF)
            {
            turn_loop_times(INIT);
            }
        turn_terminated_by_time_show();
        return_num = YES;
    }

    return return_num;
}

int search_terminate(void)
{
    int return_num = NO;

    if(timer(CHECK) == OFF) {
        if(modep->visual_mode == ON) {
            tell_terminate_to_visualize();
            //pthread_join(visual_thread, NULL);
        }
        if(modep->parallel_mode == ON) {
            tell_terminate_to_parallel();
        }
        search_terminated_by_time_show();
        return_num = YES;
        search_is_done(INIT);
    }
    search_loop_times(CHECK);

    return return_num;
}

void tell_terminate_to_visualize(void)
{
    int * solution_path = get_solution_path();

    solution_path[0] = 0;
}

void tell_terminate_to_parallel(void)
{
    int * solution_path = get_solution_path();

    solution_path[0] = 0;
}
