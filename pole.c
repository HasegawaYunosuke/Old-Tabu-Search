/*
    This file "pole.c" can edit by ONLY Pole.
*/

/* header files */
#include "header.h"

/* functions */
int * pole_search(int *);
void ga_procedure(int * solution_path, int * solution_path_b, int * solution_path_c);
#ifdef MPIMODE
int * get_smart_init_path(int * return_data);
#endif
int *order_one_cross(int *, int *);
int *pmx_one_cross(int *, int *);
void path_to_order(int *, double *);
void order_to_path(int *, double *);
double path_dinstance (int *, double *);
double * get_graph_data(void);
int * mallocer_ip(int size);
int * graph_search(int * solution_path);
int check_manneri(int type);
void error_procedure(char * message);
int get_tsp_size(void);

int * two_opt_only(int * solution_path);
int * two_opt_tabu(int * solution_path);
void set_tabu_mode(int type);
int get_ga_mode(void);
void set_counter(void);
void create_2opt_tabulist(int tsp_size, int mode);
int * get_ga_solution_path(void);
int * get_other_solution_path_data(void);
void initialize_history(void);
void set_have_been_mid_mode(void);
int check_other_solution_path_data(int *other_sol_path);
void transform_solution_path(int * other_solution_path, int * return_path);
int get_orderGA_start_point(int * sol_path, int * other_sol, int choiced, int * starti_and_maximum);

#ifdef CROSSOVER_BEF_AFT
void output_x_sol_path(int *path_a, int *path_b, int before_after);
#endif

int get_tabu_clear_count(void);

/* global variable */
int create_mode;

int * pole_search(int * solution_path)
{
    int * solution_path_b;
    int * solution_path_c;

    solution_path_c = mallocer_ip(get_tsp_size() + 1);
    solution_path_b = get_ga_solution_path();

    /* Search Graph-Data */
    if(check_manneri(SHORTMODE) == YES) {
        ga_procedure(solution_path, solution_path_b, solution_path_c);
    }
    solution_path = two_opt_tabu(solution_path);
    //solution_path_b = two_opt_only(solution_path_b);

    /* Set GA solution_path */
    if(get_ga_mode() == ON){
        set_ga_solution_path(solution_path_b);
    }

    free(solution_path_c);

    return solution_path;
}

void ga_procedure(int * solution_path, int * solution_path_b, int * solution_path_c)
{
    int i, tsp_size = get_tsp_size();

    set_tabu_mode(ON);
    set_have_been_mid_mode();

    if(check_manneri(MIDDLEMODE) == YES){
        set_counter();
        if(modep->parallel_mode == ON){
            transform_solution_path(get_other_solution_path_data(), solution_path_c);
            if(check_other_solution_path_data(solution_path_c) == YES) {
#ifdef MPIMODE
                solution_path_b = get_smart_init_path(solution_path_b);
#else
                for(i = 0; i < tsp_size + 1; i++){
                    solution_path_b[i] = solution_path_c[i];
                }
#endif
            }
        }
        if(get_ga_mode() == ON){
            /* Output before crossover path */
            #ifdef CROSSOVER_BEF_AFT
            output_x_sol_path(solution_path, solution_path_b, 0);
            #endif

            /* Choose way of crossover */
            #ifdef ORDER_GA
            order_one_cross(solution_path, solution_path_b);
            #else
            pmx_one_cross(solution_path, solution_path_b);
            #endif

            /* Output after crossover path */
            #ifdef CROSSOVER_BEF_AFT
            output_x_sol_path(solution_path, solution_path_b, 1);
            exit(0);
            #endif

            /* Clear Tabulist */
            if(get_tabu_clear_count() == TABU_CLEAR_COUNT){
                create_2opt_tabulist(get_tsp_size(), CLEAR);
            }
            set_tabu_mode(OFF);
            initialize_history();
        }
    }
}

/*transform other_solution_path type */
void transform_solution_path(int * other_solution_path, int * return_path)
{
    int tsp_size = get_tsp_size();
    int i;
    int *path_a;
    //int *path_b;
    //int *path_c;

    path_a = mallocer_ip(tsp_size + 1);
    //path_b = mallocer_ip(tsp_size + 1);
    //path_c = mallocer_ip(tsp_size + 1);

    path_a[0] = tsp_size;
    //path_b[0] = tsp_size;
    //path_c[0] = tsp_size;

    for(i = 0; i <= tsp_size; i++){
        path_a[i + 1] = other_solution_path[i];
        //path_b[i + 1] = other_solution_path[i + DEFAULT_SENDPARAMETERNUM + tsp_size];
       // path_c[i + 1] = other_solution_path[i + (DEFAULT_SENDPARAMETERNUM + tsp_size) * 2];
       }

    //int num = rand() % 3;

    //if(num == 0){
        for(i = 0; i < tsp_size + 1; i++){
            return_path[i] = path_a[i];
            }
   // }
   /*
    if(num == 1){
        for(i = 0; i < tsp_size + 1; i++){
            return_path[i] = path_b[i];
            }
    }
    if(num == 2){
        for(i = 0; i < tsp_size + 1; i++){
            return_path[i] = path_c[i];
            }
    }*/

    free(path_a);
    //free(path_b);
    //free(path_c);
}

/* one point crossover of ordinal representation  */
int *order_one_cross(int * init_path_a, int * init_path_b)
{
    double * graph_data = get_graph_data();
    int i, j;
    int tsp_size = graph_data[0];
    int path_a[tsp_size];
    int path_b[tsp_size];
    int path_c[tsp_size];
    int path_d[tsp_size];
    int choiced, merge_flag = NO;
    int starti_and_maximum[2];

 //    int *child;   
//    child = mallocer_ip(tsp_size + 1);
//    child[0] = tsp_size;
    int start_point = rand() % (tsp_size - 1);

    #ifdef MPIMODE
        #ifdef MERGE_GA
        if(get_group_reader() != get_process_number()) {
            choiced = random_num(get_all_MPI_group_data() - 1) - 1;
            get_orderGA_start_point(init_path_a, init_path_b, choiced, starti_and_maximum);
            if((starti_and_maximum[0] + GA_CROSS_POINT) < tsp_size && starti_and_maximum[1] > GA_CROSS_POINT) {
                start_point = starti_and_maximum[0];
                merge_flag = YES;
            }
        }
        #endif
    #endif

    j = 0;

    for(i = start_point; i < tsp_size; i++) {
        path_a[j] = init_path_a[i + 1];
        j++;
    }

    for(i = 0; i != start_point; i++) {
        path_a[j] = init_path_a[i + 1];
        j++;
    }

    for(i = 0; i< tsp_size; i++) {
        path_b[i] = init_path_b[i + 1];
    }
//    for(i = 0; i< tsp_size; i++) 
//        child[i + 1] = path_a[i];
    path_to_order(path_a, graph_data);
    path_to_order(path_b, graph_data);

    int cross_point = /*rand() %*/ (/*tsp_size*/ GA_CROSS_POINT);
    cross_point = tsp_size - cross_point - 1;

    #ifdef MPIMODE
        #ifdef MERGE_GA
        if((starti_and_maximum[0] + starti_and_maximum[1]) < tsp_size && merge_flag == YES) {
            cross_point = starti_and_maximum[0] + starti_and_maximum[1];
        }
        #endif
    #endif

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

    for(i = 0; i< tsp_size; i++)
          init_path_a[i + 1] = path_c[i];

    for(i = 0; i< tsp_size; i++)
          init_path_b[i + 1] = path_d[i];
    /*
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
*/
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
    for(i = 0; i< tsp_size; i++) {
        path[i] = order[i];
    }
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

        for(j = order[i] - 1; j < choice_town_num; j++) {
            choice_town[j] = choice_town[j + 1];
        }
    }

    for(i = 0; i< tsp_size; i++) {
        order[i] = path[i];
    }
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

/* one point crossover of Partially Matched*/
int *pmx_one_cross(int * init_path_a, int * init_path_b)
{
    double * graph_data = get_graph_data();
    int i, j, k,l;
    int tsp_size = graph_data[0];
    int frag;
    int path_a[tsp_size];
    int path_b[tsp_size];
    int path_c[tsp_size];
    int path_d[tsp_size];
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

    for(i = 0; i< tsp_size; i++) {
        path_b[i] = init_path_b[i + 1];
    }

    int cross_point = (GA_CROSS_POINT);

    int *copy_a;
    copy_a = mallocer_ip(cross_point);

    int *copy_b;
    copy_b = mallocer_ip(cross_point);

    for(i = 0; i < cross_point; i++)
        copy_a[i] = path_a[tsp_size - cross_point + i];
    for(i = 0; i < cross_point; i++)
        copy_b[i] = path_b[tsp_size - cross_point + i];

    k = 0;

    for(i = 0; i < tsp_size - cross_point; i++){
        frag = 0;
        for(j = 0; j < cross_point; j++){
            if(copy_b[j] == path_a[i] && frag == 0){
                frag = 1;
                path_c[k] = copy_a[j];
             }
        }
        if(frag == 1){
            do{
                l = 0;
                for(j = 0; j < cross_point; j++){
                    if(copy_b[j] == path_c[k])
                    path_c[k] = copy_a[j];
                    else l++;
                  }
            }while(l < cross_point);
        }
        if(frag == 0){
            path_c[k] = path_a[i];
        }
        k++;
    }

    for(i = 0; i < cross_point; i++){
        path_c[k] = copy_b[i];
        k++;
        }

    k = 0;
    for (i = 0; i < tsp_size - cross_point; i++){
        frag = 0;
        for(j = 0; j < cross_point; j++){
            if(copy_a[j] == path_b[i] && frag == 0){
                frag = 1;
                path_d[k] = copy_b[j];
             }
        }
        if(frag == 1){
            do{
                l = 0;
                for(j = 0; j < cross_point; j++){
                    if(copy_a[j] == path_d[k])
                    path_d[k] = copy_b[j];
                    else l++;
                  }
            }while(l < cross_point);
        }
        if(frag == 0){
            path_d[k] = path_b[i];
        }
        k++;
    }

    for(i = 0; i < cross_point; i++){
        path_d[k] = copy_a[i];
        k++;
    }
    for(i = 0; i< tsp_size; i++) {
          init_path_a[i + 1] = path_c[i];
    }

    for(i = 0; i< tsp_size; i++) {
          init_path_b[i + 1] = path_d[i];
    }
          /*
                   printf("path_a:");
	
	            for(i = 0; i< tsp_size; i++) {
                printf("%d -> ",path_a[i]);
              }
              printf("\n");
              
                        printf("path_b:");
	
	            for(i = 0; i< tsp_size; i++) {
                printf("%d -> ",path_b[i]);
              }
                            printf("\n");
                                      printf("path_c:");
	
	            for(i = 0; i< tsp_size; i++) {
                printf("%d -> ",path_c[i]);
              }
                            printf("\n");
                                      printf("path_d:");
	
	            for(i = 0; i< tsp_size; i++) {
                printf("%d -> ",path_d[i]);
              }
              printf("\n");*/
    free(copy_a);
    free(copy_b);
}
