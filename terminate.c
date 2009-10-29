/* header files */
#include "header.h"

/* functions */
int loop_terminate(void);
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

    if(turn_loop_times(CHECK) >= get_tsp_size() * 10 && get_tabu_mode() == YES) {
        turn_loop_times(INIT);

        /* DEL ST */
        printf("Turn is terminated by turn_loop_times()\n");
        /* DEL EN */

        return_num = YES;
        set_tabu_mode(OFF);

        if(modep->hasegawa_mode == ON) {
            create_2opt_tabulist(get_tsp_size(), CLEAR);
        }
    }
    if(timer(CHECK) == OFF) {
        turn_loop_times(INIT);

        /* DEL ST */
        printf("Turn is terminated by timer()\n");
        /* DEL EN */

        return_num = YES;
    }

    return return_num;
}

int search_terminate(void)
{
    int return_num = NO;

    if(timer(CHECK) == OFF) {

        /* DEL ST */
        printf("Search is terminated by timer()\n");
        /* DEL EN */

        return_num = YES;
    }
    /*else if(search_loop_times(CHECK) >= 3) {
        printf("Search is terminated by search_loop_times()\n");
        return_num = YES;
    }*/

    return return_num;
}
