/* header files */
#include "header.h"

#ifdef CROSSOVER_BEF_AFT
int * get_ga_solution_path(void);
int get_tsp_size(void);
int get_process_number(void);
void error_procedure(char * message);

void output_x_sol_path(int *path_a, int *path_b, int before_after);
void open_loging_x_sol_path(void);
void close_loging_x_sol_path(void);

/* global variable */
char x_sol_log_name[128];
FILE * x_sol_fp;
#endif

#ifdef DISTANCE_LOG
int turn_loop_times(int type);
double get_all_cost_by_graph(int * cities);
double get_best_cost(void);
int * get_solution_path(void);

void output_distance_log(void);
void open_distance_log(void);
void close_distance_log(void);

/* global variable */
char dis_log_name[128];
FILE * dis_log_fp;
#endif

#ifdef CROSSOVER_BEF_AFT
void output_x_sol_path(int *path_a, int *path_b, int before_after)
{
    int i;

    fprintf(x_sol_fp, "*** CROSSOVER LOG START ***\n");

    if(before_after == 0) {
        fprintf(x_sol_fp, "=== BEFORE CROSSOVER ===\n\n");

        fprintf(x_sol_fp, "=== PARENT A ===\n");
        for(i = 0; i <= get_tsp_size(); i++) {
            fprintf(x_sol_fp, "No.%3d> city(%3d)\n",i, path_a[i]);
        }

        fprintf(x_sol_fp, "=== PARENT B ===\n");
        for(i = 0; i <= get_tsp_size(); i++) {
            fprintf(x_sol_fp, "No.%3d> city(%3d)\n",i, path_b[i]);
        }
    }

    else {
        fprintf(x_sol_fp, "=== AFTER CROSSOVER ===\n\n");

        fprintf(x_sol_fp, "=== CHILD A ===\n");
        for(i = 0; i <= get_tsp_size(); i++) {
            fprintf(x_sol_fp, "No.%3d> city(%3d)\n",i, path_a[i]);
        }

        fprintf(x_sol_fp, "=== CHILD B ===\n");
        for(i = 0; i <= get_tsp_size(); i++) {
            fprintf(x_sol_fp, "No.%3d> city(%3d)\n",i, path_b[i]);
        }
    }

    fprintf(x_sol_fp, "*** CROSSOVER LOG END ***\n");
}

void open_loging_x_sol_path(void)
{
    if(get_process_number() < 10) {
         sprintf(x_sol_log_name, "result_log/x_sol_path.0%d.log",get_process_number());
    }
    else {
         sprintf(x_sol_log_name, "result_log/x_sol_path.%d.log",get_process_number());
    }
    if((x_sol_fp = fopen(x_sol_log_name, "a")) == NULL) {
        error_procedure("can't find \"result_log\" directory");
    }

}

void close_loging_x_sol_path(void)
{
    fclose(x_sol_fp);
}
#endif


#ifdef DISTANCE_LOG
void output_distance_log(void)
{
    fprintf(dis_log_fp, "Search: %d    , All Cost:%f, Best Cost:%f\n", turn_loop_times(READONLY), get_all_cost_by_graph(get_solution_path()), get_best_cost());
}

void open_distance_log(void)
{
    if(get_process_number() < 10) {
         sprintf(dis_log_name, "result_log/distance_log.0%d.log",get_process_number());
    }
    else {
         sprintf(dis_log_name, "result_log/distance_log.%d.log",get_process_number());
    }
    if((dis_log_fp = fopen(dis_log_name, "a")) == NULL) {
        error_procedure("can't find \"result_log\" directory");
    }
    fprintf(dis_log_fp, "*** DISTANCE LOG START ***\n");
}

void close_distance_log(void)
{
    fprintf(dis_log_fp, "*** DISTANCE LOG END ***\n");
    fclose(dis_log_fp);
}

#endif
