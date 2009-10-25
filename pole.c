/* header files */
#include"header.h"


/* functions */
int * pole_search(int *, double *);
int *order_one_cross(int *, double *);
void path_to_order(int *, double *);
void order_to_path(int *, double *);
double path_dinstance (int *, double *);

int * pole_search(int * solution_path, double * graph_data)
{
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


int *order_one_cross(int * init_path_a, double * graph_data)
{
    //int init_path_b;
    int *child;
     
      
    int tsp_size = graph_data[0];
     child[0] = tsp_size;
     
    int path_a[tsp_size];
    int path_b[tsp_size];
    int path_c[tsp_size];
    int path_d[tsp_size];
    int i;

    for(i = 0; i< tsp_size; i++) {
        path_a[i] = init_path_a[i + 1];
        path_b[i] = init_path_a[i + 1];		
    }


    path_to_order(path_a, graph_data);
    path_to_order(path_b, graph_data);  

    int cross_point = rand() % (tsp_size / 10);
    
    cross_point = tsp_size - cross_point;

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


