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
void set_tabu_clear_count(void);
#ifdef MPIMODE
int get_group_reader(void);
int get_process_number(void);
int * get_tabulist_data(void);
int * get_tabulist_data_buffer(void);
void group_reader_send_thread(int type);
void initialize_share_tabulist(void);
void create_2opt_share_tabulist(void);
void create_send_recv_tabulist(void);
int is_2opt_share_tabu(int * cities1);
void add_2opt_share_tabulist(int * cities); /* It's only save the Other Group's tabulist */
void add_2opt_send_tabulist(int * cities);
void free_tabu_share(void);
void share_tabu_matching(int * cities);
void share_flag_set(void);
int * get_send_tabulist(void);
int ** get_all_share_tabulist(void);
int get_send_recv_element_num(void);
int get_num_of_addtion_to_share_tabulist(int tsp_size);
int get_share_tabulist_2opt_index(void);
void copy_to_share_tabulist(void);
#endif
#ifdef DEBUG
void tabu_matching_loging(int type);
#endif

/* grobal variable */
int * tabulist_2opt[4];
int tabulist_2opt_index;
int list_size = 1;
int find_out_flag;
pthread_mutex_t match_mutex;
#ifdef MPIMODE
int share_tabulist_2opt_index;
int send_share_tabulist_index;          /* send_tabulist_data[]'s index and How many added the share-list by one's self */
int find_out_share_flag = NO;
int share_list_size_over_limit = NO;
int * share_tabulist_2opt[4];
int * send_tabulist_data;
int * pre_send_tabulist_data;
int * recv_tabulist_data;
int * recv_tabulist_data_buffer;
pthread_mutex_t share_match_mutex;      /* use only when share-tabulist's is under matching procedure */
pthread_mutex_t share_tabulist_lock;
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
    int start_i = 0;

    /*if((start_i = list_size - DEFAULT_TABU_MATCH_NUM) < 0) {
        start_i = 0;
    }*/

    for(i = start_i; i < list_size; i++) {
        if(tabulist_2opt[0][i] == cities[0] && tabulist_2opt[1][i] == cities[1] &&
             tabulist_2opt[2][i] == cities[2] && tabulist_2opt[3][i] == cities[3]) {

            pthread_mutex_lock(&match_mutex);
            find_out_flag = YES;
            pthread_mutex_unlock(&match_mutex);
#ifdef MPIMODE
#ifdef SEND_AMONGGROUP
            if(get_group_reader() == get_process_number()) {
                add_2opt_send_tabulist(cities);
            }
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
        pthread_mutex_init(&match_mutex, NULL);
    }
    else if(mode == CLEAR) {
        list_size = get_list_size(tsp_size, DEFAULT);
        for(i = 0; i < 4; i++) {
            for(j = 0; j < list_size; j++) {
                tabulist_2opt[i][j] = 0;
            }
        }
    }
    set_tabu_clear_count();
    tabulist_2opt_index = 0;
}

int get_list_size(int tsp_size, int type)
{
    int return_num;

    switch(type) {
        case DEFAULT:
            return_num = tsp_size * tsp_size;
            break;
        case SHARE:
            return_num = tsp_size * tsp_size;
            break;
    }

    return return_num;
}

void flag_set(void)
{
    find_out_flag = NO;
}
#ifdef SEND_AMONGGROUP
void share_flag_set(void)
{
    find_out_share_flag = NO;
}
#endif
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

    share_flag_set();

    if(get_group_reader() == get_process_number()) {
        /* Check the cities that is tabu or not by using thread in oder to Speedy Procedure */
        for(i = 0; i < 4; i++) {
            pthread_create(&matching_thread[i], NULL, (void *) share_tabu_matching, (void *) cities[i]);
        }
        for(i = 0; i < 4; i++) {
            pthread_join(matching_thread[i], NULL);
        }
    }

    return find_out_share_flag;
}

void share_tabu_matching(int * cities)
{
    int i;
    int start_i;
    int now_saved_share_list;

    pthread_mutex_lock(&share_tabulist_lock);
    now_saved_share_list = share_tabulist_2opt_index;
    pthread_mutex_unlock(&share_tabulist_lock);

    if((start_i = now_saved_share_list - DEFAULT_SHARE_TABU_MATCH_NUM) < 0) {
        start_i = 0;
    }

    for(i = start_i; i < now_saved_share_list; i++) {
        if(share_tabulist_2opt[0][i] == cities[0] && share_tabulist_2opt[1][i] == cities[1] &&
             share_tabulist_2opt[2][i] == cities[2] && share_tabulist_2opt[3][i] == cities[3]) {

            pthread_mutex_lock(&share_match_mutex);
            find_out_share_flag = YES;
            pthread_mutex_unlock(&share_match_mutex);
        }
        if(find_out_share_flag == YES) {
            break;
        }
    }
}

void initialize_share_tabulist(void)
{
    create_2opt_share_tabulist();
    create_send_recv_tabulist();
    pthread_mutex_init(&share_match_mutex, NULL);
    pthread_mutex_init(&share_tabulist_lock, NULL);
}

void create_2opt_share_tabulist(void)
{
    int i;
    int share_list_size;

    share_list_size = get_list_size(get_tsp_size(), SHARE);
    for(i = 0; i < 4; i++) {
        share_tabulist_2opt[i] = mallocer_ip(share_list_size);
    }

    share_tabulist_2opt_index = 0;
    send_share_tabulist_index= 0;
}

/* This send_recv buffer data-format is (c1, c2, c3, c4, c1, c2, c3, c4, ... , c3, c4) */
void create_send_recv_tabulist(void)
{
    int send_recv_share_tabulist_size = get_num_of_addtion_to_share_tabulist(get_tsp_size()) * 4;

    send_tabulist_data = mallocer_ip(send_recv_share_tabulist_size);
    pre_send_tabulist_data = mallocer_ip(send_recv_share_tabulist_size);
    recv_tabulist_data = mallocer_ip(send_recv_share_tabulist_size);
    recv_tabulist_data_buffer = mallocer_ip(send_recv_share_tabulist_size);
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
    return (get_num_of_addtion_to_share_tabulist(get_tsp_size()) * 4);
}

/*
 * If reaching maximum of the send_tabulist_data,
 * this func automatically send the data at parallel.c's MPI_Send() by pthread.
 */
void add_2opt_send_tabulist(int * cities)
{
    int tsp_size = get_tsp_size();
    int max = (get_num_of_addtion_to_share_tabulist(get_tsp_size()) * 4);
    int i, check_sum = OFF;

    /* Automatically Send (if data is congested) */
    if(send_share_tabulist_index >= (max - 4)) {
        for(i = 0; i < max; i++) {
            send_tabulist_data[i] = pre_send_tabulist_data[i];
        }
        group_reader_send_thread(TABU_LIST_SHARE);
        send_share_tabulist_index = 0;
    }

    /* Check the data before addtion to the send_tabulist */
    for(i = 0; i < 4; i++) {
        if(cities[i] <= 0 || cities[i] > tsp_size) {
            check_sum = ON;
            break;
        }
    }
    if(check_sum == OFF) {
        /* Add to Tabulist */
        for(i = 0; i < 4; i++) {
            pre_send_tabulist_data[send_share_tabulist_index] = cities[i];
            send_share_tabulist_index++;
        }
    }
}

/* This func calling means Recieved Share data */
void copy_to_share_tabulist(void)
{
    int i,j, start_i = 0;
    int element_num = get_num_of_addtion_to_share_tabulist(get_tsp_size());
    int max = get_num_of_addtion_to_share_tabulist(get_tsp_size());

    pthread_mutex_lock(&share_tabulist_lock);
    for(i = 0; i < element_num; i++) {
        if(share_tabulist_2opt_index >= max) {
            share_tabulist_2opt_index = 0;
        }
        for(j = 0; j < 4; j++) {
            share_tabulist_2opt[j][share_tabulist_2opt_index] = recv_tabulist_data_buffer[j + 4 * i];
        }
        share_tabulist_2opt_index++;
        tabulist_counter(SHARE, ADD);
    }
    pthread_mutex_unlock(&share_tabulist_lock);
}

void add_2opt_share_tabulist(int * cities)
{
    int i;

    /* If the Share-Tabu-List is congested, the index is set to Zero */
    if(share_tabulist_2opt_index == (get_list_size(get_tsp_size(), SHARE) - 1)) {
        if(share_list_size_over_limit == NO) {
            share_list_size_over_limit = YES;
        }
        pthread_mutex_lock(&share_tabulist_lock);
        share_tabulist_2opt_index = 0;
        pthread_mutex_unlock(&share_tabulist_lock);
    }

    pthread_mutex_lock(&share_tabulist_lock);
    for(i = 0; i < 4; i++) {
        share_tabulist_2opt[i][share_tabulist_2opt_index] = cities[i];
    }
    share_tabulist_2opt_index++;
    pthread_mutex_unlock(&share_tabulist_lock);

    tabulist_counter(SHARE, ADD);
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

/* This func is used at debug.c*/
int get_share_tabulist_2opt_index(void)
{
    return share_tabulist_2opt_index;
}

/* This func is used at debug.c*/
int ** get_all_share_tabulist(void)
{
    return share_tabulist_2opt;
}

/* This func is ONLY used at parallel.c's group_reader_send() */
int * get_send_tabulist(void)
{
    return send_tabulist_data;
}
#endif
