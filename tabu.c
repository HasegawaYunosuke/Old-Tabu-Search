/* header files */
#include "header.h"

/* functions */
int is_2opt_tabu(int * cities);
void tabu_matching(int * cities);
void create_2opt_tabulist(int tsp_size);
void flag_set(void);
int * mallocer_ip(int list_size);

/* grobal variable */
int * tabulist_2opt[4];
int list_size = 1;
int find_out_flag;
pthread_mutex_t match_mutex;

int is_2opt_tabu(int * cities1)
{
    int i;
    int cities[4][4] = {{cities1[0],cities1[1],cities1[2],cities1[3]},
                        {cities1[2],cities1[3],cities1[0],cities1[1]},
                        {cities1[1],cities1[0],cities1[3],cities1[2]},
                        {cities1[3],cities1[2],cities1[1],cities1[0]}};
    pthread_t matching_thread[4];

    pthread_mutex_init(&match_mutex, NULL);

    flag_set();

    for(i = 0; i < 4; i++) {
        pthread_create(&matching_thread[i],
                        NULL,
                        (void *) tabu_matching,
                        (void *) cities[i]);
    }

    for(i = 0; i < 4; i++) {
        pthread_join(matching_thread[i], NULL);
    }

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
        }
        if(find_out_flag == YES) {
            break;
        }
    }
}

void create_2opt_tabulist(int tsp_size)
{
    int i, j;

    /* 'list_size' is tabulist size (only once) */
    if(list_size) {
        list_size = tsp_size * tsp_size;
    }

    /* */
    for(i = 0; i < 4; i++) {
        if((tabulist_2opt[i] = mallocer_ip(list_size)) == NULL) {
            error_procedure("tabulist_2opt's malloc()");
        }
    }
}

void flag_set(void)
{
    find_out_flag = NO;
}
