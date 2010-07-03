#include "header.h"

/*DEL ST*/
void increase_test(void);
/*DEL EN*/

/* functions */
int num_counter(int field_type, int use_type);
void final_result_show(FILE * fp);
void show_mode(FILE *);
void show_on_off(FILE *, int on_off, char * buffer);
int get_tsp_size(void);
int get_num_of_all_proc(void);
int get_process_number(void);
char * get_process_name(void);
int * get_solution_path(void);
double get_all_cost_by_graph(int * cities);
double get_best_cost(void);
double get_time(void);
int check_manneri(int type);

#ifdef MPIMODE
int get_MPI_group_data(void);
void MPI_final_result_show(FILE * fp, int * recv_data, int send_data_num);
void show_chart_of_all_proc_result(FILE * showfp, int * recv_data, int send_data_num, int min_proc);
#endif
#ifdef DEBUG
int tabulist_counter(int field_type, int use_type);
#endif

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

#ifdef MPIMODE
void MPI_final_result_show(FILE * fp, int * recv_data, int send_data_num)
{
    int i;
    int min_dis = recv_data[2];
    int min_proc = recv_data[0];

    fprintf(fp, "*******************************************************\n");
    #ifdef SAMEGROUP_COMUNICATION
        fprintf(fp, "*** SAMEGROUP_COMUNICATION-MODE ***\n");
    #endif
    fprintf(fp, "TSP Size:%5d\n",get_tsp_size());
    fprintf(fp, "All Proces Num:%2d\n",get_num_of_all_proc());
    fprintf(fp, "Running Time:%f\n",get_time());
    for(i = 0; i < get_num_of_all_proc(); i++) {
        if(min_dis > recv_data[i * send_data_num + 2]) {
            min_proc = recv_data[i * send_data_num];
            min_dis = recv_data[i * send_data_num + 2];
        }
    }
    show_chart_of_all_proc_result(fp, recv_data, send_data_num, min_proc);
    fprintf(fp, "\nActive Modes--->\n");
    show_mode(fp);
    fprintf(fp, "<---Active Modes\n");
    fprintf(fp, "*******************************************************\n");
}

void show_chart_of_all_proc_result(FILE * showfp, int * recv_data, int send_data_num, int min_proc)
{
    int i;
#ifdef SAMEGROUP_COMUNICATION
    fprintf(showfp, "(ProcNum, GroupNum, BestCost, AddedTabu/All[%])--->\n");
    for(i = 0; i < get_num_of_all_proc(); i++) {
        if(i != min_proc) {
            fprintf(showfp, "  (PN, GN, BC, T/A) == (%2d, %d, %.1f, %.1f\%)\n",
                recv_data[i * send_data_num],
                recv_data[i * send_data_num + 1],
                (double)recv_data[i * send_data_num + 2] / 10,
                (double)recv_data[i * send_data_num + 6] / 10);
        }
        else {
            fprintf(showfp, "* (PN, GN, BC, T/A) == (%2d, %d, %.1f, %.1f\%)\n",
                recv_data[i * send_data_num],
                recv_data[i * send_data_num + 1],
                (double)recv_data[i * send_data_num + 2] / 10,
                (double)recv_data[i * send_data_num + 6] / 10);
        }
    }
#else
    fprintf(showfp, "(ProcNum, GroupNum, BestCost)--->\n");
    for(i = 0; i < get_num_of_all_proc(); i++) {
        if(i != min_proc) {
            fprintf(showfp, "  (PN, GN, BC) == (%2d, %d, %.1f)\n",
                recv_data[i * send_data_num],
                recv_data[i * send_data_num + 1],
                (double)recv_data[i * send_data_num + 2] / 10);
        }
        else {
            fprintf(showfp, "* (PN, GN, BC) == (%2d, %d, %.1f)\n",
                recv_data[i * send_data_num],
                recv_data[i * send_data_num + 1],
                (double)recv_data[i * send_data_num + 2] / 10);
        }
    }
#endif
}
#endif

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
