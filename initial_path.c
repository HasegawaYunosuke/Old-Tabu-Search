/* header files */
#include "header.h"

/* functions */
int * initial_euclid_path(int * euclid_data);
int * create_euclid_path(int * return_data, int * base_data, int create_mode);
double * initial_graph_path(double * graph_data);
double * create_graph_path(double * path, int create_mode);
int random_num(int maximum);
double make_distance(int x1, int y1, int x2, int y2);
int already_visited(int * return_data, int city_num);
int search_loop_times(int type);
int * mallocer_ip(int size);
double * mallocer_dp(int size);

/* global variable */
int create_mode = DEFAULT;

int * initial_euclid_path(int * euclid_data)
{
    int * return_data;

    /* first time procedure (mainly, alocate memory) */
    if(search_loop_times(READONLY) == 0) {
        /* +10 is for something parameter */
        return_data = mallocer_ip(euclid_data[0] + 10);
        srand(time(NULL));
        /* set return_data[0] to 'TSP-example-size' */
        return_data[0] = euclid_data[0];
    }

    /* create the initial path */
    return_data = create_euclid_path(return_data, euclid_data, create_mode);

    return return_data;
}

double * initial_graph_path(double * graph_data)
{
    double * return_data;

    /* first time procedure */
    if(search_loop_times(READONLY) == 0) {
        return_data = mallocer_dp((int)graph_data[0]);
    }

    /* create the initial path */
    return_data = create_graph_path(return_data, create_mode);

    return return_data;
}

/* This fucntion makes 'initial-path' that type is 'Euclid' */
int * create_euclid_path(int * return_data, int * euclid_data, int create_mode)
{
    int return_data_index;
    int next_city;
    int min_index;
    int tsp_size = euclid_data[0];
    int first_point;
    int now_x, now_y;
    int next_x, next_y;
    double distance = DBL_MAX;
    double min_distance = DBL_MAX;

    /* Mode DEFAULT (nearest nearby) */
    if(create_mode == DEFAULT) {
        /* get first point by random (1 <= first_point <= tsp_size) */
        first_point = random_num(tsp_size);

        return_data[1] = first_point;
        now_x = euclid_data[first_point * 2];
        now_y = euclid_data[first_point * 2 + 1];

        for(return_data_index = 2; return_data_index <= tsp_size; return_data_index++) {
            for(next_city = 1; next_city <= tsp_size; next_city++) {
                /* check already choiced cities */
                if(already_visited(return_data, next_city) == YES) {
                    continue;
                }
                else {
                    next_x = euclid_data[next_city * 2];
                    next_y = euclid_data[next_city * 2 + 1];
                    distance = make_distance(now_x, now_y, next_x, next_y);
                    if(min_distance > distance) {
                        min_index = next_city;
                        min_distance = distance;
                    }
                }
                /* set the city of nearest at now-city */
                return_data[return_data_index] = next_city;
            }
        }
    }
    /* Mode another */
    else {
        /* Non-aveirable, yet */
    }

    return return_data;
}

double * create_graph_path(double * path, int create_mode)
{
    double * return_data;

    printf("Mr.Tozaki would write here\n");

    return return_data;
}

int already_visited(int * return_data, int city_num)
{
    int i;
    int return_num = NO;

    for(i = 1; i <= return_data[0]; i++) {
        if(return_data[i] == city_num) {
            return_num = YES;
            break;
        }
    }

    return return_num;
}
