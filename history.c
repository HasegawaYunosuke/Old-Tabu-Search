/* header files */
#include "header.h"

struct history {
    int top;
    int bottom;
    int search_times;
    int loop_times;
    double distance;
    struct history * next;
    struct history * prev;
};

/* functions */
void add_history(void);
void error_procedure(char * message);
void create_historys(void);
int get_history_size(void);
int * get_solution_path(void);
void add_infomation(struct history now);
double get_all_cost_by_graph(int * cities);
int turn_loop_times(int type);
int search_loop_times(int type);
void show_history(void);

/* grobal variable */
struct history * historyp;

void create_historys(void)
{
    int i;
    int history_size;

    history_size = get_history_size();

    historyp = (struct history *)malloc(sizeof(struct history) * history_size);

    for(i = 0; i < history_size; i++) {
        historyp[i].search_times = 0;
        historyp[i].loop_times = 0;
        historyp[i].distance = 0;
        if(i == 0) {
            historyp[i].prev == NULL;
            historyp[i].top = YES;
            historyp[i].bottom = NO;
        }
        else if(i == history_size - 1) {
            historyp[i].next == NULL;
            historyp[i].prev = &historyp[i - 1];
            historyp[i].top = NO;
            historyp[i].bottom = YES;
        }
        else {
            historyp[i - 1].next = &historyp[i];
            historyp[i].prev = &historyp[i - 1];
            historyp[i].top = NO;
            historyp[i].bottom = NO;
        }
    }
}

int get_history_size(void)
{
    return DEFAULT_HISTORYSIZE;
}

void add_history(void)
{
    int bottom, top;
    int flag = OFF;
    int history_size = get_history_size();

    for(bottom = 0; bottom < history_size; bottom++) {
        if(historyp[bottom].bottom == YES) {
            for(top = 0; top < history_size; top++) {
                if(historyp[top].top == YES) {
                    historyp[bottom].next = &historyp[top];
                    historyp[top].prev = &historyp[bottom];
                    historyp[bottom].top = YES;
                    historyp[bottom].bottom = NO;
                    historyp[top].top = NO;
                    (historyp[bottom].prev)->bottom = YES;
                    (historyp[bottom].prev)->next = NULL;
                    historyp[bottom].prev = NULL;

                    if(modep->graph_mode == ON) {
                        historyp[bottom].distance = get_all_cost_by_graph(get_solution_path());
                    }
                    historyp[bottom].loop_times = turn_loop_times(READONLY);
                    historyp[bottom].search_times = search_loop_times(READONLY);
                    flag = ON;
                    break;
                }
            }
        }
        if(flag == ON) {
            break;
        }
    }
    /* DEL ST */
    show_history();
    /* DEL EN */
}

void show_history(void)
{
    int i;
    struct history now;

    printf("DEL:history.c:HISTORYS\n");
    for(i = 0; i < get_history_size(); i++) {
        if(historyp[i].top == YES) {
            //printf("DEL:(No. 1, distance, loop, search) == (%.1f,%d,%d)\n"
            //    ,historyp[i].distance,historyp[i].loop_times,historyp[i].search_times);
            printf("DEL:dis == %f\n",historyp[0].distance);
            now = historyp[i];
            break;
        }
    }

    for(i = 2; i <= get_history_size(); i++) {
        printf("DEL:(No.%2d, distance, loop, search) == (%.1f,%d,%d)\n",
            i,(now.next)->distance,(now.next)->loop_times,(now.next)->search_times);
    }
    for(i = 0; i < get_history_size(); i++) {
        printf("DELDEL:%d,%f\n",i,historyp[i].distance);
    }

    sleep(1);
}
