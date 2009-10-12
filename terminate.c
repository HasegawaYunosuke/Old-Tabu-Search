/* header files */
#include "header.h"

/* functions */
int loop_terminate(void);
int search_terminate(void);
int turn_loop_times(int type);
int search_loop_times(int type);
int timer(int type);

int loop_terminate(void)
{
    int return_num = NO;

    if(turn_loop_times(CHECK) >= 3) {
        turn_loop_times(INIT);
        printf("Turn is terminated by turn_loop_times()\n");
        return_num = YES;
    }
    else if(timer(CHECK) == OFF) {
        turn_loop_times(INIT);
        printf("Turn is terminated by timer()\n");
        return_num = YES;
    }

    return return_num;
}

int search_terminate(void)
{
    int return_num = NO;

    if(timer(CHECK) == OFF) {
        printf("Search is terminated by timer()\n");
        return_num = YES;
    }
    else if(search_loop_times(CHECK) >= 3) {
        printf("Search is terminated by search_loop_times()\n");
        return_num = YES;
    }

    return return_num;
}
