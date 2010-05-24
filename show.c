#include "header.h"

/*DEL ST*/
void increase_test(void);
/*DEL EN*/

/* functions */
int num_counter(int field_type, int use_type);
void turn_terminated_show(void);
void turn_terminated_by_time_show(void);
void search_terminated_by_time_show(void);
void final_result_show(FILE * fp);
void show_mode(FILE *);
void show_on_off(FILE *, int on_off, char * buffer);
int get_tsp_size(void);
int get_num_of_all_proc(void);
int get_process_number(void);
int get_MPI_group_data(void);
char * get_process_name(void);
int * get_solution_path(void);
double get_all_cost_by_graph(int * cities);
double get_best_cost(void);
double get_time(void);
int check_manneri(int type);

#ifdef DEBUG
int tabulist_counter(int field_type, int use_type);
#endif

void turn_terminated_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
        //printf("Turn is terminated by turn_loop_times()\n");
    }
    /* DEL EN */
}

void turn_terminated_by_time_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
       // printf("Turn is terminated by timer()\n");
    }
    /* DEL EN */
}

void search_terminated_by_time_show(void)
{
    /* DEL ST */
    if(modep->parallel_mode != YES) {
       // printf("Search is terminated by timer()\n");
    }
    /* DEL EN */
}

void final_result_show(FILE * fp)
{
    fprintf(fp, "*******************************************************\n");
    fprintf(fp, "TSP Size:%5d\n",get_tsp_size());
    fprintf(fp, "All Proces Num:%2d\n",get_num_of_all_proc());
    fprintf(fp, "Process Number:%2d\n",get_process_number());
    fprintf(fp, "Process Name:%s\n",get_process_name());
    fprintf(fp, "MPI Group:%2d\n",get_MPI_group_data());
    fprintf(fp, "Running Time:%f\n",get_time());
    fprintf(fp, "%.2f\n",get_best_cost());
    /*if(modep->pole_mode == ON) {
        fprintf(fp, "Search Count:%d\n",turn_loop_times(READONLY));
    }*/
    fprintf(fp, "Search Count Num:%d\n",num_counter(SEARCH_COUNTER, CHECK));
    fprintf(fp, "Tunr Count Num:%2d\n",num_counter(TURN_COUNTER, CHECK));
#ifdef DEBUG
    fprintf(fp, "Local Tabulist Num:%d\n",tabulist_counter(DEFAULT, READONLY));
#ifdef MPIMODE
    if(modep->parallel_mode == ON) {
        fprintf(fp, "Share Tabulist Num:%d\n",tabulist_counter(SHARE, READONLY));
    }
#endif
#endif
    fprintf(fp, "\nActive Modes--->\n");
    show_mode(fp);
    fprintf(fp, "<---Active Modes\n");
    fprintf(fp, "*******************************************************\n");
}

void show_mode(FILE * fp)
{
    show_on_off(fp,modep->visual_mode, "Visual_Mode");
    show_on_off(fp,modep->realtime_visual_mode, "Realtime-Visual_Mode");
    show_on_off(fp,modep->parallel_mode, "Parallel_Mode");
    show_on_off(fp,modep->hasegawa_mode, "Hasegawa_Mode");
    show_on_off(fp,modep->pole_mode, "Pole_Mode");
    show_on_off(fp,modep->tozaki_mode, "Tozaki_Mode");
    show_on_off(fp,modep->tabu2opt_mode, "Tabu-2opt_Mode");
    show_on_off(fp,modep->only2opt_mode, "Only-2opt_Mode");
    show_on_off(fp,modep->ga_mode, "GA_Mode");
}

void show_on_off(FILE * fp, int on_off, char * buffer)
{
    if(on_off == ON) {
        fprintf(fp, "\t%s ON\n", buffer);
    }
}
