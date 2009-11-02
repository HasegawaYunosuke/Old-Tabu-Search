/* header files */
#include "header.h"

struct history {
    int index;
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
void initialize_history(void);
void data_initialize(int index);
void newest_data(int index);
void middle_data(int index);
void oldest_data(int index);
int get_newest(void);
int get_oldest(void);
int get_next(int target);
int get_prev(int target);
void insert_newest(int newest, int oldest);
void set_newest_data(int index);
int check_fill(void);
int check_historical_similar(void);
double * mallocer_dp(int size);

/* grobal variable */
struct history * historyp;

void create_historys(void)
{
    historyp = (struct history *)malloc(sizeof(struct history) * get_history_size());
    initialize_history();
}

void initialize_history(void)
{
    int i;
    int size = get_history_size();

    for(i = 0; i < size; i++) {
        data_initialize(i);
        if(i == 0) {
            newest_data(i);
        }
        else if(i == size - 1) {
            oldest_data(i);
            historyp[i].prev = &historyp[i-1];
        }
        else {
            middle_data(i);
        }
    }
}

void data_initialize(int index)
{
    historyp[index].search_times = 0;
    historyp[index].loop_times = 0;
    historyp[index].distance = 0;
    historyp[index].index = index;
}

void newest_data(int index)
{
    historyp[index].prev = NULL;
    historyp[index].top = YES;
    historyp[index].bottom = NO;
}

void middle_data(int index)
{
    historyp[index - 1].next = &historyp[index];
    historyp[index].prev = &historyp[index - 1];
    historyp[index].top = NO;
    historyp[index].bottom = NO;
}

void oldest_data(int index)
{
    historyp[index].next = NULL;
    historyp[index].top = NO;
    historyp[index].bottom = YES;
}

int get_history_size(void)
{
    return DEFAULT_HISTORYSIZE;
}

void add_history(void)
{
    int oldest, newest;

    newest = get_newest();
    oldest = get_oldest();
    insert_newest(newest, oldest);

    /* DEL ST */
    //show_history();
    /* DEL EN */
}
void insert_newest(int newest, int oldest)
{
    historyp[oldest].next = &historyp[newest];
    historyp[newest].prev = &historyp[oldest];
    historyp[newest].top = NO;

    oldest_data(get_prev(oldest));
    newest_data(oldest);
    set_newest_data(oldest);
}

void set_newest_data(int index)
{
    if(modep->graph_mode == ON) {
        historyp[index].distance = get_all_cost_by_graph(get_solution_path());
    }
    historyp[index].loop_times = turn_loop_times(READONLY);
    historyp[index].search_times = search_loop_times(READONLY);
}

int get_newest(void)
{
    int i;

    for(i = 0; i < get_history_size(); i++) {
        if(historyp[i].top == YES) {
            return i;
        }
    }
}

int get_oldest(void)
{
    int i;

    for(i = 0; i < get_history_size(); i++) {
        if(historyp[i].bottom == YES) {
            return i;
        }
    }
}

int get_next(int target)
{
    if(historyp[target].next == NULL) {
        return -1;
    }
    else {
        return (historyp[target].next)->index;
    }
}

int get_prev(int target)
{
    if(historyp[target].prev == NULL) {
        return 0;
    }
    else {
        return (historyp[target].prev)->index;
    }
}

void show_history(void)
{
    int i, index = get_newest();

    for(i = 1; i < get_history_size(); i++) {
        printf("No.%2d:(dis, loop, search) == (%.1f,%d,%d)\n",
            i,
            historyp[index].distance,
            historyp[index].loop_times,
            historyp[index].search_times);
        index = get_next(index);
    }
}

/* Check history data, whether the data is fill, or not */
int check_fill(void)
{
    /* if the data is filled, return "YES" */
    if(historyp[get_oldest()].distance == 0) {
        return NO;
    }
    else {
        return YES;
    }
}

int check_historical_similar(void)
{
    int base, target, same = 0;
    int size = get_history_size();

    for(base = 1; base <= size; base++) {
        for(target = 1; target <= size; target++) {
            if(historyp[base - 1].distance == historyp[target - 1].distance && base != target) {
                same++;
            }
        }
        if(many_sames(same) == YES) {
            return YES;
        }
    }

    return NO;
}

int many_sames(int same)
{
    if((same / get_history_size() * 100) > DEFAULT_SAMEPERCENTAGE) {
        return YES;
    }
    else {
        return NO;
    }
}
