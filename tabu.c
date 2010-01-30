/* header files */
#include "header.h"

/* functions */
int is_2opt_tabu(int * cities);
void tabu_matching(int * cities);
void add_2opt_tabulist(int * cities);
void create_2opt_tabulist(int tsp_size, int mode);
int get_list_size(int tsp_size, int type);
void flag_set(void);
int * mallocer_ip(int list_size);
int get_tsp_size(void);
void free_tabu(void);
int tabulist_counter(int field_type, int use_type);
#ifdef DEBUG
void tabu_matching_loging(int type);
#endif
#ifdef MPIMODE
void initialize_share_tabulist(void);
void create_send_recv_tabulist(void);
int is_2opt_share_tabu(int * cities1);
void create_2opt_share_tabulist(void);
void add_2opt_share_tabulist(int * cities);
void free_tabu_share(void);
void share_tabu_matching(int * cities);
void share_flag_set(void);
int share_tabulist_is_satisfactory(void);
int * get_share_tabulist(void);
int get_send_recv_element_num(void);
int * get_tabulist_data_buffer(void);
int * get_tabulist_data(void);
int * base_format_to_send_format_share_tabulist(int type);
int * recv_format_to_base_format_share_tabulist(void);
#endif

/* grobal variable */
int * tabulist_2opt[4];
int tabulist_2opt_index;
int list_size = 1;
int find_out_flag;
pthread_mutex_t match_mutex;
#ifdef MPIMODE
int share_list_size = 1;
int share_list_size_over_limit = NO;
int * share_tabulist_2opt[4];
int share_tabulist_2opt_index;
int find_out_share_flag = NO;
pthread_mutex_t share_match_mutex;
int * send_tabulist_data;
int * recv_tabulist_data;
int * recv_tabulist_data_buffer;
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

    if(find_out_flag == YES) {
#ifdef DEBUG
        tabu_matching_loging(MATCH);
#endif
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
#ifdef MPIMODE
#ifdef SEND_AMONGGROUP
            add_2opt_share_tabulist(cities);
#endif
#endif
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

    if(tabulist_2opt_index >= get_list_size(get_tsp_size(), DEFAULT)) {
        tabulist_2opt_index = 0;
    }

    tabulist_counter(DEFAULT, ADD);
    tabulist_2opt_index++;
}

void create_2opt_tabulist(int tsp_size, int mode)
{
    int i, j;

    /* 'list_size' is tabulist size (only once) */
    if(mode == INIT) {
        if(list_size == 1) {
            list_size = get_list_size(tsp_size, DEFAULT);
            for(i = 0; i < 4; i++) {
                tabulist_2opt[i] = mallocer_ip(list_size);
            }
        }
    }
    else if(mode == CLEAR) {
        list_size = get_list_size(tsp_size, DEFAULT);
        for(i = 0; i < 4; i++) {
            for(j = 0; j < list_size; j++) {
                tabulist_2opt[i][j] = 0;
            }
        }
    }

    tabulist_2opt_index = 0;
}

int get_list_size(int tsp_size, int type)
{
    int list_size;

    switch(type) {
        case DEFAULT:
            list_size = (tsp_size * tsp_size);
            break;
        case SHARE:
            list_size = tsp_size * tsp_size;
            break;
    }

    return list_size;
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

void initialize_share_tabulist(void)
{
    create_2opt_share_tabulist();
    create_send_recv_tabulist();
}

void create_2opt_share_tabulist(void)
{
    int i;

    share_list_size = get_list_size(get_tsp_size(), SHARE);
    for(i = 0; i < 4; i++) {
        share_tabulist_2opt[i] = mallocer_ip(share_list_size);
    }

    share_tabulist_2opt_index = 0;
}

/* This format is followed (city1 --> c1) */
/* c1, c2, c3, c4, c1, c2, c3, c4, ... , c3, c4
 * All data-size is
 * {1/2 x list_size x 4} == list_size x 2 */
void create_send_recv_tabulist(void)
{
    send_tabulist_data = mallocer_ip(get_list_size(get_tsp_size(), SHARE)  * 2);
    recv_tabulist_data = mallocer_ip(get_list_size(get_tsp_size(), SHARE)  * 2);
    recv_tabulist_data_buffer = mallocer_ip(get_list_size(get_tsp_size(), SHARE) * 2);
}

int * get_tabulist_data_buffer(void)
{
    return recv_tabulist_data_buffer;
}

int * get_tabulist_data(void)
{
    return recv_tabulist_data;
}

int get_send_recv_element_num(void)
{
    return (get_list_size(get_tsp_size(), SHARE) * 2);
}

/* adjust the data-format */
int * base_format_to_send_format_share_tabulist(int type)
{
    int i,j, start_i = 0;
    int element_num = get_send_recv_element_num();

    if(type == UPPER) {
        start_i = 0;
    }
    else if(type == DOWNER) {
        start_i = get_list_size(get_tsp_size(), SHARE) / 2;
    }

    for(i = 0; i < element_num; i++) {
        for(j = 0; j < 4; j++) {
            send_tabulist_data[j + 4 * i] = share_tabulist_2opt[j][i + start_i];
        }
    }

    return send_tabulist_data;
}

int * recv_format_to_base_format_share_tabulist(void)
{
    int i,j, start_i = 0;
    int element_num = get_send_recv_element_num();

    for(i = 0; i < element_num; i++) {
        for(j = 0; j < 4; j++) {
            share_tabulist_2opt[j][i + share_tabulist_2opt_index] = recv_tabulist_data[j + 4 * i];
            share_tabulist_2opt_index++;
            if(share_tabulist_2opt_index >= get_list_size(get_tsp_size(), SHARE)) {
                share_tabulist_2opt_index = 0;
            }
        }
    }
}

void add_2opt_share_tabulist(int * cities)
{
    int i;

    if(share_tabulist_2opt_index == (get_list_size(get_tsp_size(), SHARE) - 1)) {
        if(share_list_size_over_limit == NO) {
            share_list_size_over_limit = YES;
        }
        share_tabulist_2opt_index = 0;
    }

    for(i = 0; i < 4; i++) {
        share_tabulist_2opt[i][share_tabulist_2opt_index] = cities[i];
    }

    tabulist_counter(SHARE, ADD);
    share_tabulist_2opt_index++;
}

void free_tabu_share(void)
{
    int i;

    for(i = 0; i < 4; i++) {
        free(share_tabulist_2opt[i]);
    }
    free(send_tabulist_data);
    free(recv_tabulist_data);
    free(recv_tabulist_data_buffer);
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
        }
        if(find_out_flag == YES) {
            break;
        }
    }
}

int share_tabulist_is_satisfactory(void)
{
    return share_list_size_over_limit;
}

int alternative_type = UPPER;

int * get_share_tabulist(void)
{
    int * return_data;
    int type;

    if(alternative_type == ON) {
        alternative_type = OFF;
        type = UPPER;
    }
    else {
        alternative_type = ON;
        type = DOWNER;
    }

    return_data = base_format_to_send_format_share_tabulist(type);

    return return_data;
}
#endif
