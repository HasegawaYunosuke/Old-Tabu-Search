/*
    This file "pole.c" can edit by ONLY Pole.
*/

/* header files */
#include "header.h"

/* functions */
int * pole_search(int *);
int *order_one_cross(int *, int *);
void path_to_order(int *, double *);
void order_to_path(int *, double *);
double path_dinstance (int *, double *);
double * get_graph_data(void);
int * mallocer_ip(int size);
int * create_graph_path(int * path, double * graph_data, int create_mode);
int * graph_search(int * solution_path);
int check_manneri(int type);
void error_procedure(char * message);


int *two_opt(int*);
void set_now_parcentage(double before, double after);
int nowindex(int target, int maximum);
int nextindex(int target, int maximum);
int previndex(int target, int maximum);
double get_cost_of_branch(int a, int ad, int b, int bd);
double get_graph_cost(int a,int b);
void get_cities_by_indexes(int * cities, int * indexes, int * solution_path);
double before_after_distance(int * cities);
void exchange_branches(int * solution_path, int * indexes);

int is_2opt_tabu(int * cities);
void add_2opt_tabulist(int * cities);
int get_tabu_mode(void);
void set_tabu_mode(int type);
int permit_worse_distance(double bef_aft_distance);
void create_2opt_tabulist(int tsp_size, int mode);
int turn_loop_times(int type);
int not_found_looping(int * cities, int * indexs, int type);
int check_parcentage(double bef_aft_distance);

void set_ga_mode(int type);
int get_ga_mode(void);
void set_counter(void);
void create_2opt_tabulist(int tsp_size, int mode);
int * get_ga_solution_path(void);
int * get_other_solution_path_data(void);

void initialize_history(void);

/* global variable */
int create_mode;

int * pole_search(int * solution_path)
{ 
    int *solution_path_b;
    solution_path_b = get_ga_solution_path(); 
    /* Search Graph-Data */
    if(modep->graph_mode == ON) {   
   
        if(check_manneri(SHORTMODE) == YES) {            

        set_tabu_mode(ON);
        
        
            if(check_manneri(MIDDLEMODE) == YES){

                set_ga_mode(ON); 
                set_counter();
            
                if(modep->parallel_mode == ON){
                    solution_path_b = get_other_solution_path_data();
                    }
            order_one_cross(solution_path, solution_path_b);
            create_2opt_tabulist(get_tsp_size(), CLEAR);
            set_tabu_mode(OFF);
            initialize_history();
             }
        }     
                
        solution_path = two_opt(solution_path);
        solution_path_b = two_opt(solution_path_b);
        set_ga_solution_path(solution_path_b);
}                      
               
    /* Search Euclid-Data (non-available) */
    else if(modep->euclid_mode == ON) {
        error_procedure("pole_search() non-available");
    }
    /* Error */
    else {
        error_procedure("pole_search()");
    }
    
    return solution_path;
                   
}     
/*two opt search*/
int *two_opt(int * solution_path)
{
    int i, j, k, count;
    int tsp_size = solution_path[0];
    int cities[4];
    int indexes[4];
    int best_indexes[4];
    double now_distance = DBL_MAX * (-1);
    double maximum = DBL_MAX * (-1);

    if(get_tabu_mode() == OFF) {
        for(i = 1; i < tsp_size - 2; i++) {
            indexes[0] = i; indexes[1] = i + 1;
            for(j = i + 2; j < tsp_size; j++) {
                indexes[2] = j; indexes[3] = j + 1;
                get_cities_by_indexes(cities, indexes, solution_path);
                now_distance = before_after_distance(cities);
                if(now_distance > maximum) {
                    maximum = now_distance;
                    for(k = 0; k < 4; k++) {
                        best_indexes[k] = indexes[k];
                    }
                }
            }
        }

        for(i = 0; i < 4; i++) {
            if(indexes[i] > tsp_size) {
                printf("indexes[%d] == %d\n",i,best_indexes[i]);
                error_procedure("two_opt()");
            }
        }

        exchange_branches(solution_path, best_indexes);
        
}
    else //if(get_tabu_mode() == ON)
     {
    
    if(turn_loop_times(READONLY) % 2 == 0) {
    
     now_distance = DBL_MAX * (-1);
     maximum = DBL_MAX * (-1);
    
        for(i = 1; i < tsp_size - 2; i++) {
            indexes[0] = i; indexes[1] = i + 1;
            for(j = i + 2; j < tsp_size; j++) {
                indexes[2] = j; indexes[3] = j + 1;
                get_cities_by_indexes(cities, indexes, solution_path);
                now_distance = before_after_distance(cities);
                if(now_distance > maximum) {
                    maximum = now_distance;
                    for(k = 0; k < 4; k++) {
                        best_indexes[k] = indexes[k];
                    }
                }
            }
        }
        for(i = 0; i < 4; i++) {
            if(indexes[i] > tsp_size) {
                printf("indexes[%d] == %d\n",i,best_indexes[i]);
                error_procedure("two_opt()");
            }
        }  
        
        exchange_branches(solution_path, best_indexes);    
      
        }else{
        
        do{ 
            int a,b;
            int max = solution_path[0];

             a = random_num(max);
            do {
                b = random_num(max);
            } while(b == previndex(a, max) || b == a || b == nextindex(a, max));

            indexes[0] = a; indexes[1] = nextindex(a, max);
            indexes[2] = b; indexes[3] = nextindex(b, max);

            get_cities_by_indexes(cities, indexes, solution_path);

            if(not_found_looping(cities, indexes, COUNT) == YES) {
                    not_found_looping(cities, indexes, READONLY);
                    break;
                }              
        } while(permit_worse_distance(before_after_distance(cities)) == NO || is_2opt_tabu(cities) == YES);

         not_found_looping(cities, indexes, INIT);
         exchange_branches(solution_path, indexes);
        }
   }
    
    return solution_path;
}

void get_cities_by_indexes(int * cities, int * indexes, int * solution_path)
{
    int i;

    for(i = 0; i < 4; i++) {
        if(indexes[i] > solution_path[0]) {
            error_procedure("get_cities_by_indexes's");
        }
        cities[i] = solution_path[indexes[i]];
    }
      
}

double before_after_distance(int * cities)
{
    double before, after;

    before = get_cost_of_branch(cities[0],cities[1],cities[2],cities[3]);
    after = get_cost_of_branch(cities[0],cities[2],cities[1],cities[3]);
   
    set_now_parcentage(before, after);

    return (before - after);
}
double get_cost_of_branch(int a, int ad, int b, int bd){
return (get_graph_cost(a, ad) + get_graph_cost(b, bd));    
}

/* (1 <= return_num <= Max) */
int nextindex(int target, int maximum)
{
    return nowindex((target % maximum + 1), maximum);
}

/* (1 <= return_num <= Max) */
int previndex(int target, int maximum)
{
    return nowindex(((target == 1) ? maximum : target -1), maximum);
}

/* (1 <= return_num <= Max) */
int nowindex(int target, int maximum)
{
    int return_num;

    if(target > 0) {
        return_num = (((return_num = target % maximum) == 0) ? maximum : return_num);
    }
    else {
        return_num = (((return_num = target * (-1) % maximum) == 0) ? maximum : return_num);
        if(target % maximum == 0) {
            return_num = maximum;
        }
        else {
            return_num = target % maximum + maximum;
        }
    }

    return return_num;
}

void exchange_branches(int * solution_path, int * indexes)
{
    int i, count;
    int tsp_size = solution_path[0];
    int * copy;
    int cities[4];

    for(i = 0; i < 4; i++) {
        if(indexes[i] > tsp_size) {
            printf("indexes[%d] == %d\n",i,indexes[i]);
            error_procedure("exchange_branches()");
        }
    }
    copy = mallocer_ip(tsp_size + 1);

    for(i = 0; i <= tsp_size; i++) {
        copy[i] = solution_path[i];
    }
    
     count = indexes[2] - indexes[1];
     
     if(count<0)
     count += tsp_size;
     
    
    for(i = 0; i <= count; i++) {
        solution_path[nowindex((indexes[2] - i), tsp_size)] = copy[nowindex((indexes[1] + i), tsp_size)];
    }
    
    if(get_tabu_mode() == ON) {         
    get_cities_by_indexes(cities, indexes, solution_path);
    add_2opt_tabulist(cities);
    }

   free(copy);     
}


/* permit exchange toward worse if under permit_baseline */
int permit_worse_distance(double bef_aft_distance)
{
    int return_num = NO;

    /* if exchange is toward better, permit */
    if(bef_aft_distance > 0) {
        return_num = YES;
    }
    else{
        /* permit_worse discribed > 0 */
        if(check_parcentage(bef_aft_distance) == NO) {
            return_num = NO;
        }
        else {
            return_num = YES;
        }
    }

    return return_num;
}
/* one point crossover of ordinal representation  */
int *order_one_cross(int * init_path_a, int * init_path_b)
{
    double * graph_data = get_graph_data();
    int i, j;
    int tsp_size = graph_data[0];    
//    int *child;
    int path_a[tsp_size];
    int path_b[tsp_size];
    int path_c[tsp_size];
    int path_d[tsp_size];
    
//    child = mallocer_ip(tsp_size + 1);
//    child[0] = tsp_size;

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
     
//    for(i = 0; i< tsp_size; i++) 
//        child[i + 1] = path_a[i];
    
    path_to_order(path_a, graph_data);
    path_to_order(path_b, graph_data);  

    int cross_point = /*rand() %*/ (/*tsp_size /*/ GA_CROSS_POINT);    
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

