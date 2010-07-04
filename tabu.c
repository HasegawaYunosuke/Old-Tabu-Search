/* header files */
#include "header.h"

/* functions */
int random_num(int maximum);
int is_2opt_tabu(int * cities);
void tabu_matching(int * cities);
void add_2opt_tabulist(int * cities);
void create_2opt_tabulist(int tsp_size, int mode);
int get_list_size(int tsp_size, int type);
void flag_set(void);
int * mallocer_ip(int list_size);
double * mallocer_dp(int size);
int get_tsp_size(void);
void free_tabu(void);
int tabulist_counter(int field_type, int use_type);
void set_tabu_clear_count(void);
#ifdef MPIMODE
int prev_city(int target, int maximum);
int next_city(int target, int maximum);
int * get_smart_init_path(int * return_data);
int get_smart_init_city(int choiced_city, int * visited_cities);
int check_visited(int next_city, int * visited_cities);
int choice_non_visited(int * visited_cities);
int get_smart_random_city(int maximum);
int get_smart_city(int choiced_city);
int get_never_visited_city(int choiced_city);
void set_L(void);
int get_L(void);
int get_y_by_i(int b, int a, int i);
int get_MPI_group_data(void);
int get_group_reader(void);
int get_process_number(void);
double get_added_MPI_same_group_tabulist_per_all(void);
int * get_tabulist_data(void);
int * get_tabulist_data_buffer(void);
void group_reader_send_thread(int type);
void initialize_share_tabulist(void);
void create_2opt_share_tabulist(void);
void create_send_recv_tabulist(void);
void create_same_group_tabulist(int tsp_size);
void add_MPI_same_group_tabulist(int add_mode, int * add_data);
void add_branch_to_MPI_same_group_tabulist(int target, int next);
void insert_data_to_same_group_tabulist(double * graph_data, int tsp_size);
void graph_data_to_dis_of_selected_city_and_pre_cities(int tsp_size, double * graph_data, double * distance_of_selected_city, int * pre_sorted_cities);
void bubble_sort(int tsp_size, double * distance_of_selected_city, int * pre_sorted_cities);
void insert_data_to_near_cities(int selected_city, int * pre_sorted_cities);
void free_same_group_tabulist(void);
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
#ifdef SAMEGROUP_COMUNICATION_DEBUG
void out_put_MPI_same_tabulist(FILE * fp);
#endif
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

struct MPI_same_group_tabulist {
    int * near_cities;
    int * visited_cities;
    int max_visited;
    int max_index;
    int smart_choice_num;
};

struct MPI_same_group_tabulist_counter {
    int all_list_num;
    int added_city_num;
    int L;
    /*DEL ST*/
    int choice_nearest;
    int choice_other;
    int miss_num;
    /*DEL EN*/
};

struct MPI_same_group_tabulist * MPI_same_group_tabulistp;
struct MPI_same_group_tabulist_counter MPI_same_group_tabulist_counter;
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

#ifdef MPIMODE
void create_same_group_tabulist(int tsp_size)
{
    int str_city, memb_city;
    int tabulist_num = (int)tsp_size / DEFAULT_SAMEGROUP_TABULISTSIZE_DONOMINATOR; /* 4 */

    /* allocate memory space ( +1 is tabulist_num) */
    MPI_same_group_tabulistp = (struct MPI_same_group_tabulist *)malloc(sizeof(struct MPI_same_group_tabulist) * (tsp_size + 1));
    for(str_city = 0; str_city <= tsp_size; str_city++) {
        MPI_same_group_tabulistp[str_city].near_cities = mallocer_ip(tabulist_num + 1);
        MPI_same_group_tabulistp[str_city].visited_cities = mallocer_ip(tabulist_num + 1);
    }

    /* initialize the data (city-number and visited-city to ZERO, both array's [0] is tabulist_num) */
    for(str_city = 0; str_city <= tsp_size; str_city++) {
        MPI_same_group_tabulistp[str_city].max_visited = 0;
        MPI_same_group_tabulistp[str_city].max_index = 0;
        MPI_same_group_tabulistp[str_city].near_cities[0] = tabulist_num;
        MPI_same_group_tabulistp[str_city].visited_cities[0] = tabulist_num;
        for(memb_city = 1; memb_city <= tabulist_num; memb_city++) {
            MPI_same_group_tabulistp[str_city].near_cities[memb_city] = 0;
            MPI_same_group_tabulistp[str_city].visited_cities[memb_city] = 0;
            MPI_same_group_tabulistp[str_city].smart_choice_num = 0;
        }
    }

    /* This parameter must be check! */
    set_L();

    /* counter */
    MPI_same_group_tabulist_counter.all_list_num = tabulist_num * tsp_size;
    MPI_same_group_tabulist_counter.added_city_num = 0;

    /*DEL ST*/
    MPI_same_group_tabulist_counter.choice_nearest = 0;
    MPI_same_group_tabulist_counter.choice_other = 0;
    MPI_same_group_tabulist_counter.miss_num = 0;
    /*DEL EN*/
}

void free_same_group_tabulist(void)
{
    int i;
    int tsp_size = get_tsp_size();

    for(i = 0; i < tsp_size; i++) {
        free(MPI_same_group_tabulistp[i].near_cities);
        free(MPI_same_group_tabulistp[i].visited_cities);
    }
    free(MPI_same_group_tabulistp);
}

void add_MPI_same_group_tabulist(int add_mode, int * add_data)
{
    int data_size = get_tsp_size();
    int i, target_city, next_city;

    if(add_mode == SOL_PATH) {
        for(i = 1; i <= data_size; i++) {
            if(i == data_size) {
                target_city = add_data[i]; next_city = add_data[1];
            }
            else {
                target_city = add_data[i]; next_city = add_data[i + 1];
            }
            add_branch_to_MPI_same_group_tabulist(target_city, next_city);
        }
    }
    else if(add_mode == OTHER_SOL_PATH) {
        for(i = 0; i < data_size; i++) {
            if(i == data_size) {
                target_city = add_data[i]; next_city = add_data[1];
            }
            else {
                target_city = add_data[i]; next_city = add_data[i + 1];
            }
            add_branch_to_MPI_same_group_tabulist(target_city, next_city);
        }
    }
    else if(add_mode == FOUR_CITIES) {
        add_branch_to_MPI_same_group_tabulist(add_data[0], add_data[2]); /* A-B */
        add_branch_to_MPI_same_group_tabulist(add_data[1], add_data[3]); /* A'-B' */
    }
}

void add_branch_to_MPI_same_group_tabulist(int target_city, int next_city)
{
    int list_size = MPI_same_group_tabulistp[0].near_cities[0];
    int i, j, temp;

    /* find out whether "next_city" is exist or not (target --> next, next --> target ) */
    for(j = 0; j < 2; j++) {
        for(i = 1; i <= list_size; i++) {
            if(next_city == MPI_same_group_tabulistp[target_city].near_cities[i]) {

                /* Don't effect to Search. Just Count How Many Added the Cities. */
                if(MPI_same_group_tabulistp[target_city].visited_cities[i] == 0) {
                    MPI_same_group_tabulist_counter.added_city_num++;
                }

                /* Add count */
                if(MPI_same_group_tabulistp[target_city].visited_cities[i] < INT_MAX) {
                    MPI_same_group_tabulistp[target_city].visited_cities[i]++;
                }

                /* Renew the Most-Visited-City, and Index-of-Most-Visited */
                if(MPI_same_group_tabulistp[target_city].visited_cities[i] > MPI_same_group_tabulistp[target_city].max_visited) {
                    MPI_same_group_tabulistp[target_city].max_visited = MPI_same_group_tabulistp[target_city].visited_cities[i];
                    MPI_same_group_tabulistp[target_city].max_index = i;
                }

                break;
            }
        }
        /* swap "next_city" <---> "target_city" */
        temp = target_city; target_city = next_city; next_city = temp;
    }
}

void insert_data_to_same_group_tabulist(double * graph_data, int tsp_size)
{
    int * pre_sorted_cities;
    double * distance_of_selected_city;
    int selected_city;
    int i;

    pre_sorted_cities = mallocer_ip(tsp_size);
    distance_of_selected_city = mallocer_dp(tsp_size);

    for(selected_city = 1; selected_city <= tsp_size; selected_city++) {
        distance_of_selected_city[0] = (double)selected_city; pre_sorted_cities[0] = selected_city;
        graph_data_to_dis_of_selected_city_and_pre_cities(tsp_size, graph_data, distance_of_selected_city, pre_sorted_cities);
        bubble_sort(tsp_size, distance_of_selected_city, pre_sorted_cities);
        insert_data_to_near_cities(selected_city, pre_sorted_cities);
    }

    free(pre_sorted_cities);
    free(distance_of_selected_city);
}

void graph_data_to_dis_of_selected_city_and_pre_cities(int tsp_size, double * graph_data, double * distance_of_selected_city, int * pre_sorted_cities)
{
    int city, index = 1;
    int selected_city = pre_sorted_cities[0];

    for(city = 1; city <= tsp_size; city++) {
        if(city != selected_city) {
            pre_sorted_cities[index] = city;
            distance_of_selected_city[index] = graph_data[index + selected_city * tsp_size];
            index++;
        }
    }
}

void bubble_sort(int tsp_size, double * distance_of_selected_city, int * pre_sorted_cities)
{
    double temp, index;
    int i, j;

    for(j = tsp_size - 2; j >= 1; j--) {
        for(i = 1; i <= j; i++) {
            if(distance_of_selected_city[i] > distance_of_selected_city[i + 1]) {
                temp = distance_of_selected_city[i];
                index = pre_sorted_cities[i];
                distance_of_selected_city[i] = distance_of_selected_city[i + 1];
                pre_sorted_cities[i] = pre_sorted_cities[i + 1];
                distance_of_selected_city[i + 1] = temp;
                pre_sorted_cities[i + 1] = index;
            }
        }
    }
}

void insert_data_to_near_cities(int selected_city, int * pre_sorted_cities)
{
    int i;
    int tabulist_num = MPI_same_group_tabulistp[selected_city].visited_cities[0];

    for(i = 1; i <= tabulist_num; i++) {
        MPI_same_group_tabulistp[selected_city].near_cities[i] = pre_sorted_cities[i];
    }
}

double get_added_MPI_same_group_tabulist_per_all(void)
{
    double return_num = 0;

    return_num = (double)MPI_same_group_tabulist_counter.added_city_num / (double)MPI_same_group_tabulist_counter.all_list_num * 100;

    return return_num;
}

int get_smart_random_city(int maximum)
{
    int city;
    int tsp_size = get_tsp_size();
    int return_num = -1;

    for(city = 1; city <= tsp_size; city++) {
        if(MPI_same_group_tabulistp[city].max_index != 1 || MPI_same_group_tabulistp[city].max_index == 0) {
            return_num = city;
            break;
        }
    }

    if(return_num < 0) {
        return_num = random_num(maximum);
    }

    return return_num;
}

/* return_data's format is followed.
 * TSP-Size, city1, city2, ... ,cityN, parameter1, ..., parameterM
 * 'N' means TSP-Size, 'M' is DEFAULT_SENDPARAMETERNUM - 1 (this '1' is First 'TSP-Size')
 * All data length is TSP-Size + DEFAULT_SENDPARAMETERNUM */
int * get_smart_init_path(int * return_data)
{
    int i;
    int tsp_size = get_tsp_size();
    int next_city, now_city = random_num(tsp_size);
    int * visited_cities;

    /* Data Initialize */
    visited_cities = mallocer_ip(tsp_size);
    return_data[0] = tsp_size;

    for(i = 0; i < tsp_size; i++) {
        visited_cities[now_city - 1] = YES;
        return_data[i + 1] = now_city;
        next_city = get_smart_init_city(now_city, visited_cities);
        now_city = next_city;
    }

    free(visited_cities);

    return return_data;
}

int get_smart_init_city(int choiced_city, int * visited_cities)
{
    int next_city = -1;
    int i, y, L = get_L();

    if(MPI_same_group_tabulistp[choiced_city].max_index != 1) {
        next_city = MPI_same_group_tabulistp[choiced_city].near_cities[1];
        /* "NO" means that "next_city" havn't been visited. */
        if(check_visited(next_city, visited_cities) == NO) {
            return next_city;
        }
        /* Reset the next-city */
        else {
            next_city = -1;
        }
    }

    for(i = 2; i < L; i++) {
        y = get_y_by_i(MPI_same_group_tabulistp[choiced_city].max_visited, L, i);
        if(MPI_same_group_tabulistp[choiced_city].visited_cities[i] <= y) {
            next_city = MPI_same_group_tabulistp[choiced_city].near_cities[i];
            if(check_visited(next_city, visited_cities) == NO) {
                return next_city;
            }
            /* Reset the next-city */
            else {
                next_city = -1;
            }
        }
    }

    /* If not found */
    if(next_city == -1) {
        return choice_non_visited(visited_cities);
    }
    else {
        error_procedure("get_smart_init_city()");
    }
}

int choice_non_visited(int * visited_cities)
{
    int i, next_city = -1;
    int tsp_size = get_tsp_size();

    for(i = 0; i < tsp_size; i++) {
        if(visited_cities[i] == NO) {
            next_city = i + 1;
            return next_city;
        }
    }

    return next_city;
}

int check_visited(int next_city, int * visited_cities)
{
    if(visited_cities[next_city - 1] == NO) {
        return NO;
    }
    else {
        return YES;
    }
}

int get_never_visited_city(int choiced_city)
{
    int next_city = NO;
    int list_size = MPI_same_group_tabulistp[0].near_cities[0];
    int i;

    for(i = 1; i <= list_size; i++) {
        if(MPI_same_group_tabulistp[choiced_city].visited_cities[i] <= MPI_same_group_tabulistp[choiced_city].max_visited) {
            next_city = MPI_same_group_tabulistp[choiced_city].near_cities[i];
            break;
        }
    }

    return next_city;
}

int get_smart_city(int choiced_city)
{
    int i, y, L = get_L();
    int y_minus_now_visited[L];
    int random = 0;
    int sum_of_y_minus_now_visited = 0;
    int max = get_tsp_size();
    int smart_next_city;

    /*DEL ST*/
    MPI_same_group_tabulistp[choiced_city].smart_choice_num++;
    /*DEL EN*/

    /* Now Steragegy START */
    if(MPI_same_group_tabulistp[choiced_city].max_index != 1) {
        smart_next_city = MPI_same_group_tabulistp[choiced_city].near_cities[1];
        if(smart_next_city != prev_city(choiced_city, max) && smart_next_city != next_city(choiced_city, max)) {
            MPI_same_group_tabulist_counter.choice_nearest++;
            return smart_next_city;
        }
        else {
            return -1;
        }
    }
    else {
        for(i = 1; i < L; i++) {
            y = get_y_by_i(MPI_same_group_tabulistp[choiced_city].max_visited, L, i);
            if((y_minus_now_visited[i - 1] = y - MPI_same_group_tabulistp[choiced_city].visited_cities[i]) < 0) {
                y_minus_now_visited[i - 1] = 0;
            }
            sum_of_y_minus_now_visited += y_minus_now_visited[i - 1];
        }

        if(sum_of_y_minus_now_visited <= L) {
            return -1;
        }

        random = random_num(sum_of_y_minus_now_visited);

        for(i = 1; i < L; i++) {
            if(random < y_minus_now_visited[i - 1]) {
                smart_next_city = MPI_same_group_tabulistp[choiced_city].near_cities[i];
                if(smart_next_city != prev_city(choiced_city, max) && smart_next_city != next_city(choiced_city, max)) {
                    MPI_same_group_tabulist_counter.choice_other++;
                    return smart_next_city;
                }
                else {
                    return -1;
                }
            }
            else {
                random -= y_minus_now_visited[i - 1];
            }
        }
    }
    /* Now Steragegy END */

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    /* Prev Steragegy START */
    /* if the choiced city's nearest city haven't been visited */
    /*if(MPI_same_group_tabulistp[choiced_city].max_index != 1) {
        smart_next_city = MPI_same_group_tabulistp[choiced_city].near_cities[1];
        if(smart_next_city != prev_city(choiced_city, max) && smart_next_city != next_city(choiced_city, max)) {
            MPI_same_group_tabulist_counter.choice_nearest++;
            return smart_next_city;
        }
    }

    for(i = 2; i < L; i++) {
        y = get_y_by_i(MPI_same_group_tabulistp[choiced_city].max_visited, L, i);
        if(MPI_same_group_tabulistp[choiced_city].visited_cities[i] <= y) {
            smart_next_city = MPI_same_group_tabulistp[choiced_city].near_cities[i];
            if(smart_next_city != prev_city(choiced_city, max) && smart_next_city != next_city(choiced_city, max)) {
                MPI_same_group_tabulist_counter.choice_other++;
                return smart_next_city;
            }
        }
    }*/
    /* Prev Steragegy END */

    return -1;
}

void set_L(void)
{
    double MPI_group = (double)get_MPI_group_data(); /* 0~ */
    double list_size = (double)MPI_same_group_tabulistp[0].near_cities[0];

    MPI_same_group_tabulist_counter.L = (int)(list_size * (MPI_group + 5) / 100);
    if(MPI_same_group_tabulist_counter.L <= 4) {
        MPI_same_group_tabulist_counter.L = 5;
    }
}

/* 3~ */
int get_L(void)
{
    return MPI_same_group_tabulist_counter.L;
}

#ifdef SAMEGROUP_COMUNICATION_DEBUG
void out_put_MPI_same_tabulist(FILE * fp)
{
    int i, j;
    int list_size = MPI_same_group_tabulistp[1].near_cities[0];
    int tsp_size = get_tsp_size();

    fprintf(fp,"+++ Same Group Tabu-List Debug-log +++\n\n");
    fprintf(fp,"(nearest:other) == (%d:%d), (near+oth:miss) == (%d:%d), L == %d\n",
        MPI_same_group_tabulist_counter.choice_nearest, MPI_same_group_tabulist_counter.choice_other,
        (MPI_same_group_tabulist_counter.choice_nearest + MPI_same_group_tabulist_counter.choice_other), MPI_same_group_tabulist_counter.miss_num,
        get_L());
    fprintf(fp,"(near)<===============================\n");
    for(i = 1; i <= tsp_size; i++) {
        fprintf(fp,"str[%4d]'s ||max_visited%4d (index:%4d) || y == (%4d,%4d,%4d) || smart_choice == %4d|| ",
            i, MPI_same_group_tabulistp[i].max_visited, MPI_same_group_tabulistp[i].max_index,
            get_y_by_i(MPI_same_group_tabulistp[i].max_visited, get_L(), 1),
            get_y_by_i(MPI_same_group_tabulistp[i].max_visited, get_L(), 2),
            get_y_by_i(MPI_same_group_tabulistp[i].max_visited, get_L(), 3),
            MPI_same_group_tabulistp[i].smart_choice_num);
        fprintf(fp," y == ");
        for(j = 1; j <= get_L(); j++) {
            fprintf(fp,"%4d, ",get_y_by_i(MPI_same_group_tabulistp[i].max_visited, get_L(), j));
        }
        fprintf(fp," || ");
        for(j = 1; j <= list_size; j++) {
            fprintf(fp,"%4d, ", MPI_same_group_tabulistp[i].visited_cities[j]);
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"\n++++++++++++++++++++++++++++++++++++++\n");
}
#endif
#endif

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
#ifdef MPIMODE
//#ifdef SEND_AMONGGROUP
void share_flag_set(void)
{
    find_out_share_flag = NO;
}
//#endif
#endif
void free_tabu(void)
{
    int i;

    for(i = 0; i < 4; i++) {
        free(tabulist_2opt[i]);
    }
#ifdef MPIMODE
    if(get_group_reader() == get_process_number()) {
        free_tabu_share();
    }
#endif
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
