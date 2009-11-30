#include "header.h"

/* functions */
void realtime_result(void);
void turn_terminated_show(void);
void turn_terminated_by_time_show(void);
void search_terminated_by_time_show(void);
int * get_solution_path(void);
double get_all_cost_by_graph(int * cities);
double get_best_cost(void);
int check_manneri(int type);

void realtime_result(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("DEL:all_cost == %f,",get_all_cost_by_graph(get_solution_path()));
        printf("best == %.2f,",get_best_cost());
        if(modep->tabu_mode == ON) {
            printf("tabu ON,");
        }
        else {
            printf("tabu OFF,");
        }

        if(check_manneri(SHORTMODE) == YES) {
            printf("mannneri ON\n");
        }
        else {
            printf("mannneri OFF\n");
        }
    /* DEL EN */
    }
}

void turn_terminated_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("Turn is terminated by turn_loop_times()\n");
    }
    /* DEL EN */
}

void turn_terminated_by_time_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("Turn is terminated by timer()\n");
    }
    /* DEL EN */
}

void search_terminated_by_time_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("Search is terminated by timer()\n");
    }
    /* DEL EN */
}
