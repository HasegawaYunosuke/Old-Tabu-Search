/* header files */
#include "header.h"

struct history {
    int top;
    int bottom;
    int search_times;
    int loop_times;
    int * solution_path;
    double distance;
    struct history * next;
    struct history * prev;
};

/* functions */
void error_procedure(char * message);
void create_historys(void);
int get_history_size(void);
int * get_solution_path(void);
void add_infomation(struct history now);

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
        historyp[i].solution_path = NULL;
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

void add_history()
{
    int bottom, top;
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
                    add_infomation(historyp[bottom]);
                }
            }
        }
    }
}

void add_infomation(struct history now)
{
    error_procedure("add_infomation() is non-available");
    now.solution_path = get_solution_path();
}
