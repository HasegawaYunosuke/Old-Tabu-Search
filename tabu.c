/* header files */
#include "header.h"

/* functions */
int is_2opt_tabu(int * cities);
void tabu_matching(int * cities);
void add_2opt_tabulist(int * cities);
void create_2opt_tabulist(int tsp_size, int mode);
int get_list_size(int tsp_size);
void flag_set(void);
int * mallocer_ip(int list_size);
int get_tsp_size(void);
void free_tabu(void);
#ifdef DEBUG
void tabu_matching_loging(int type);
#endif
#ifdef MPIMODE
void create_2opt_share_tabulist(void);
void add_2opt_share_tabulist(int * cities);
void free_tabu_share(void);
void share_tabu_matching(int * cities);
void share_flag_set(void);
#endif

/* grobal variable */
int * tabulist_2opt[4];
int tabulist_2opt_index;
int list_size = 1;
int find_out_flag;
pthread_mutex_t match_mutex;
#ifdef MPIMODE
int share_list_size = 1;
int * share_tabulist_2opt[4];
int share_tabulist_2opt_index;
int find_out_share_flag = NO;
pthread_mutex_t share_match_mutex;
#endif

int is_2opt_tabu(int * cities1)
{
    int i;
    //int cities[4][4] = {{cities1[0],cities1[1],cities1[2],cities1[3]},  /* A, A', B, B' */
    //                    {cities1[2],cities1[3],cities1[0],cities1[1]},  /* B, B', A, A' */
    //                    {cities1[1],cities1[0],cities1[3],cities1[2]},  /* A', A, B', B */
    //                    {cities1[3],cities1[2],cities1[1],cities1[0]}}; /* B', B ,A', A,*/
    int cities[4][4] = {{cities1[0],cities1[2],cities1[1],cities1[3]},  /* A, B, A', B' */
                        {cities1[1],cities1[3],cities1[0],cities1[2]},  /* A', B', A, B */
                        {cities1[2],cities1[0],cities1[3],cities1[1]},  /* B, A, B', A' */
                        {cities1[3],cities1[1],cities1[2],cities1[0]}}; /* B', A', B, A,*/

    pthread_t matching_thread[4];

    pthread_mutex_init(&match_mutex, NULL);

    flag_set();

#ifdef DEBUG
    tabu_matching_loging(WHOLE);
#endif

    for(i = 0; i < 4; i++) {
        pthread_create(&matching_thread[i],
                        NULL,
                        (void *) tabu_matching,
                        (void *) cities[i]);
    }

    for(i = 0; i < 4; i++) {
        pthread_join(matching_thread[i], NULL);
    }

#ifdef DEBUG
    if(find_out_flag == YES) {
        tabu_matching_loging(MATCH);
    }
#endif

    return find_out_flag;
}

void tabu_matching(int * cities)
{
    int i;

    for(i = 0; i < list_size; i++) {
        if(tabulist_2opt[0][i] == cities[0] && tabulist_2opt[1][i] == cities[1] &&
             tabulist_2opt[2][i] == cities[2] && tabulist_2opt[3][i] == cities[3]) {

            pthread_mutex_lock(&match_mutex);
            find_out_flag = YES;
            pthread_mutex_unlock(&match_mutex);
            add_2opt_share_tabulist(cities);
        }
        if(find_out_flag == YES) {
            break;
        }
    }
}

void add_2opt_tabulist(int * cities)
{
    int i;

    for(i = 0; i < 4; i++) {
        tabulist_2opt[i][tabulist_2opt_index] = cities[i];
    }

    if(tabulist_2opt_index >= get_list_size(get_tsp_size())) {
        tabulist_2opt_index = 0;
    }

    tabulist_2opt_index++;
}

void create_2opt_tabulist(int tsp_size, int mode)
{
    int i, j;

    /* 'list_size' is tabulist size (only once) */
    if(mode == INIT) {
        if(list_size == 1) {
            list_size = get_list_size(tsp_size);
            for(i = 0; i < 4; i++) {
                tabulist_2opt[i] = mallocer_ip(list_size);
            }
        }
    }
    else if(mode == CLEAR) {
        list_size = get_list_size(tsp_size);
        for(i = 0; i < 4; i++) {
            for(j = 0; j < list_size; j++) {
                tabulist_2opt[i][j] = 0;
            }
        }
    }

    tabulist_2opt_index = 0;
}

int get_list_size(int tsp_size)
{
    return (tsp_size * tsp_size);
}

void flag_set(void)
{
    find_out_flag = NO;
}

void free_tabu(void)
{
    int i;

    for(i = 0; i < 4; i++) {
        free(tabulist_2opt[i]);
    }
}

#ifdef MPIMODE
int is_2opt_share_tabu(int * cities1)
{
    int i;
    //int cities[4][4] = {{cities1[0],cities1[1],cities1[2],cities1[3]},  /* A, A', B, B' */
    //                    {cities1[2],cities1[3],cities1[0],cities1[1]},  /* B, B', A, A' */
    //                    {cities1[1],cities1[0],cities1[3],cities1[2]},  /* A', A, B', B */
    //                    {cities1[3],cities1[2],cities1[1],cities1[0]}}; /* B', B ,A', A,*/
    int cities[4][4] = {{cities1[0],cities1[2],cities1[1],cities1[3]},  /* A, B, A', B' */
                        {cities1[1],cities1[3],cities1[0],cities1[2]},  /* A', B', A, B */
                        {cities1[2],cities1[0],cities1[3],cities1[1]},  /* B, A, B', A' */
                        {cities1[3],cities1[1],cities1[2],cities1[0]}}; /* B', A', B, A,*/

    pthread_t matching_thread[4];

    pthread_mutex_init(&share_match_mutex, NULL);

    share_flag_set();

/*#ifdef DEBUG
    share_tabu_matching_loging(WHOLE);
#endif*/

    for(i = 0; i < 4; i++) {
        pthread_create(&matching_thread[i],
                        NULL,
                        (void *) share_tabu_matching,
                        (void *) cities[i]);
    }

    for(i = 0; i < 4; i++) {
        pthread_join(matching_thread[i], NULL);
    }

#ifdef DEBUG
    /*if(find_out_share_flag == YES) {
        share_tabu_matching_loging(MATCH);
    }*/
#endif

    return find_out_flag;
}

void share_flag_set(void)
{
    find_out_share_flag = NO;
}

/* Same format (Local-Tabulist) */
void create_2opt_share_tabulist(void)
{
    share_list_size = get_list_size(get_tsp_size());
    for(i = 0; i < 4; i++) {
        share_tabulist_2opt[i] = mallocer_ip(share_list_size);
    }

    share_tabulist_2opt_index = 0;
}

void add_2opt_share_tabulist(int * cities)
{
    int i;

    for(i = 0; i < 4; i++) {
        share_tabulist_2opt[i][share_tabulist_2opt_index] = cities[i];
    }

    if(share_tabulist_2opt_index >= get_list_size(get_tsp_size())) {
        share_tabulist_2opt_index = 0;
    }

    share_tabulist_2opt_index++;
}

void free_tabu_share(void)
{
    for(i = 0; i < 4; i++) {
        free(share_tabulist_2opt[i]);
    }
}

void share_tabu_matching(int * cities)
{
    int i;

    for(i = 0; i < share_list_size; i++) {
        if(share_tabulist_2opt[0][i] == cities[0] && share_tabulist_2opt[1][i] == cities[1] &&
             share_tabulist_2opt[2][i] == cities[2] && share_tabulist_2opt[3][i] == cities[3]) {

            pthread_mutex_lock(&share_match_mutex);
            find_out_share_flag = YES;
            pthread_mutex_unlock(&share_match_mutex);
            add_2opt_share_tabulist(cities);
        }
        if(find_out_flag == YES) {
            break;
        }
    }
}

#endif
