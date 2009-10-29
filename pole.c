/*
    This file "pole.c" can edit by ONLY Pole.
*/

/* header files */
#include "header.h"

/* functions */
int * pole_search(int *);
int *order_one_cross(int *, double *);
void path_to_order(int *, double *);
void order_to_path(int *, double *);
double path_dinstance (int *, double *);
double * get_graph_data(void);
int * mallocer_ip(int size);
int * create_graph_path2(int * path, double * graph_data, int create_mode);

/* global variable */
int create_mode;

int * pole_search(int * solution_path)
{
	double * graph_data = get_graph_data();

    /* Search Graph-Data */
    if(modep->graph_mode == ON) {
        solution_path = order_one_cross(solution_path, graph_data);
    }
    /* Search Euclid-Data (non-available) */
    else if(modep->euclid_mode == ON) {
        error_procedure("pole_search() non-available");
    }
    /* Error */
    else {
        error_procedure("pole_search()");
    }        
}

/* one point crossover of ordinal representation  */
int *order_one_cross(int * init_path_a, double * graph_data)
{
    int i, j;
    int tsp_size = graph_data[0];    
    int *init_path_b;    
    int *child;
    int path_a[tsp_size];
    int path_b[tsp_size];
    int path_c[tsp_size];
    int path_d[tsp_size];
    
    child = mallocer_ip(tsp_size + 1);
    child[0] = tsp_size;
    
    init_path_b = mallocer_ip(tsp_size + 1);    
    init_path_b = create_graph_path2(init_path_b, graph_data, create_mode);

    int start_point = rand() % (tsp_size - 1);
    
    j = 0;

    for(i = start_point; i < tsp_size; i++) {
        path_a[j] = init_path_a[i + 1];
        j++;
        }       

    for(i = 0; i != start_point; i++) {
        path_a[j] = init_path_a[i + 1];
        j++;
        }

    for(i = 0; i< tsp_size; i++) 
        path_b[i] = init_path_b[i + 1];		
     
    for(i = 0; i< tsp_size; i++) 
        child[i + 1] = path_a[i];
    
    path_to_order(path_a, graph_data);
    path_to_order(path_b, graph_data);  

    int cross_point = rand() % (tsp_size / 5);    
    cross_point = tsp_size - cross_point - 1;

    for (i = 0; i < cross_point; i++){
    path_c[i] = path_a[i];
    path_d[i] = path_b[i]; 
    }

    for (i = cross_point; i < tsp_size; i++){
    path_c[i] = path_b[i];
    path_d[i] = path_a[i]; 
    }
    
    order_to_path(path_c, graph_data);
    order_to_path(path_d, graph_data);
    
    if(path_dinstance(path_c, graph_data) < path_dinstance(path_d, graph_data))
    {
        for(i = 0; i< tsp_size; i++) 
           child[i + 1] = path_c[i];
    }

    if(path_dinstance(path_d, graph_data) <= path_dinstance(path_c, graph_data))
    {
        for(i = 0; i< tsp_size; i++) 
            child[i + 1] = path_d[i];
    }

    return (child);
}

/* transform from path representation to ordinal representation  */
void path_to_order(int *path, double * graph_data)
{
    int tsp_size = graph_data[0];
        
    int choice_town[tsp_size];
    int order[tsp_size];	 
    int choice_town_num = tsp_size;

    int i,j,k;

    for(i = 0; i < tsp_size; i++)
        choice_town[i] = i + 1;

    for(i = 0; i < tsp_size; i++) {
        for(j = 0; j < choice_town_num; j++){
            if(path[i] == choice_town[j]){
                order[i] = j + 1;

            for(k = j; k < choice_town_num; k++)
                choice_town[k] = choice_town[k+1];
                choice_town_num--;
                break;
            }
        }
    }
    
    for(i = 0; i< tsp_size; i++) 
    path[i] = order[i];
}

/* transform from ordinal representation to path representation  */
void order_to_path(int *order, double * graph_data)
{
    int tsp_size = graph_data[0];
    int choice_town[tsp_size];
    int path[tsp_size];	 
    int choice_town_num = tsp_size;
    int i,j;

    for(i = 0; i < tsp_size; i++)
        choice_town[i] = i + 1;

    for(i = 0; i < tsp_size; i++) {
        path[i] = choice_town[order[i] - 1];

        for(j = order[i] - 1; j < choice_town_num; j++)
            choice_town[j] = choice_town[j + 1];
    }

    for(i = 0; i< tsp_size; i++) 
        order[i] = path[i];
}

/* calculate path dinstance */
double path_dinstance (int * solution_path, double * graph_data)
{
    int tsp_size = graph_data[0];        
    int i,now_city,next_city;    
    double distance = 0;

    for(i = 0; i < tsp_size - 1; i++) {
        now_city = solution_path[i];
        next_city = solution_path[i+1];
        distance += graph_data[now_city + tsp_size * next_city];       
    }
    
    now_city = solution_path[tsp_size - 1];
    next_city = solution_path[0];
    distance += graph_data[now_city + tsp_size * next_city];

    return(distance);	
}

/* create the second initial path */
int * create_graph_path2(int * return_data, double * graph_data, int create_mode)
{
    int i, j;
    int mini_index;
    int second_index;
    int first_point;
    int tsp_size = (int)graph_data[0];
    int now_city, next_city;
    int is_choiced[TSPMAXSIZE] = {NO};
    double distance = DBL_MAX;
    double min_distance = DBL_MAX;

    first_point = random_num(tsp_size);

    now_city = first_point;
    is_choiced[first_point] = YES;
    return_data[1] = first_point;

    for(i = 2; i < tsp_size; i++) {
        for(j =1; j <= tsp_size; j++) {
            if(is_choiced[j] == NO) {
                next_city = j;
                distance = graph_data[now_city + tsp_size * next_city];
                if(min_distance >= distance) {
                    mini_index = next_city;
                    min_distance = distance;
                }
            }
            else {
                continue;
            }
        }

        is_choiced[mini_index] = YES;
        min_distance = DBL_MAX;
        
        for(j =1; j <= tsp_size; j++) {
            if(is_choiced[j] == NO) {
                next_city = j;
                distance = graph_data[now_city + tsp_size * next_city];
                if(min_distance >= distance) {
                    second_index = next_city;
                    min_distance = distance;
                }
            }
            else {
                continue;
            }
        }
                
        is_choiced[mini_index] = NO;
        is_choiced[second_index] = YES;
        return_data[i] = second_index;
        min_distance = DBL_MAX;
        now_city = second_index;
    }
    
    return_data[tsp_size] = mini_index;
    
    return (return_data);
}
