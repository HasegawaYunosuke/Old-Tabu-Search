#include "header.h"

/* functions */
void realtime_result(void);
void turn_terminated_show(void);
void turn_terminated_by_time_show(void);
void search_terminated_by_time_show(void);
void final_result_show(void);
void show_mode(void);
void show_on_off(int on_off, char * buffer);
int get_num_of_all_proc(void);
int get_process_number(void);
int get_MPI_group_data(void);
char * get_process_name(void);
int * get_solution_path(void);
double get_all_cost_by_graph(int * cities);
double get_best_cost(void);
double get_time(void);
int check_manneri(int type);

void realtime_result(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("DEL:all_cost == %f,",get_all_cost_by_graph(get_solution_path()));
        printf("best == %.2f,",get_best_cost());
        if(modep->tabu_mode == ON) {
            printf("tabu ON,");
        }
        else {
            printf("tabu OFF,");
        }

        if(check_manneri(SHORTMODE) == YES) {
            printf("mannneri ON\n");
        }
        else {
            printf("mannneri OFF\n");
        }
    /* DEL EN */
    }
}

void turn_terminated_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("Turn is terminated by turn_loop_times()\n");
    }
    /* DEL EN */
}

void turn_terminated_by_time_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("Turn is terminated by timer()\n");
    }
    /* DEL EN */
}

void search_terminated_by_time_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        printf("Search is terminated by timer()\n");
    }
    /* DEL EN */
}

void final_result_show(void)
{
    printf("*******************************************************\n");
    printf("All Proces Num:%2d\n",get_num_of_all_proc());
    printf("Process Number:%2d\n",get_process_number());
    printf("Process Name:%s\n",get_process_name());
    printf("MPI Group:%2d\n",get_MPI_group_data());
    printf("Running Time:%f\n",get_time());
    printf("Best Cost:%.2f\n",get_best_cost());
    printf("\nActive Modes--->\n");
    show_mode();
    printf("<---Active Modes\n");
    printf("*******************************************************\n");
}

void show_mode(void)
{
    show_on_off(modep->visual_mode, "Visual_Mode");
    show_on_off(modep->realtime_visual_mode, "Realtime-Visual_Mode");
    show_on_off(modep->graph_mode, "Graph_Mode");
    show_on_off(modep->euclid_mode, "Euclid_Mode");
    show_on_off(modep->parallel_mode, "Parallel_Mode");
    show_on_off(modep->hasegawa_mode, "Hasegawa_Mode");
    show_on_off(modep->pole_mode, "Pole_Mode");
    show_on_off(modep->tozaki_mode, "Tozaki_Mode");
    show_on_off(modep->tabu2opt_mode, "Tabu-2opt_Mode");
    show_on_off(modep->only2opt_mode, "Only-2opt_Mode");
}

void show_on_off(int on_off, char * buffer)
{
    if(on_off == ON) {
        printf("\t%s ON\n", buffer);
    }
}
