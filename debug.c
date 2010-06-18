/* header files */
#include "header.h"

#ifdef DEBUG
/* global variable */
char debug_log_name[128];
char debug_other_sol_log_name[128];
FILE * debug_fp;
FILE * debug_other_sol_fp;

struct tabu_matching_log {
    int all_check_num;
    int matched_num;
};

struct tabu_matching_log tabu_matching_log;

#ifdef MPIMODE
struct mpi_comunication_log {
    int mpi_send_num;
    int mpi_recv_num;
    int max_num_of_match_num;
};

struct mpi_comunication_log mpi_com_log;
#endif

/* functions */
int get_tsp_size(void);
void open_loging_initial_path(void);
void close_loging_initial_path(void);
void loging_initial_path(int * path, int create_mode);
void test_debug_log(char message[128], int num);
int get_counter(void);
void tabu_matching_loging(int type);
void tabu_matching_init(void);
void add_whole_check_num(void);
void add_matched_num(void);
void loging_tabu_matched(void);
int * get_best_solution_path(void);
int tabulist_counter(int field_type, int use_type);
#endif
#ifdef MPIMODE
#ifdef SAMEGROUP_COMUNICATION_DEBUG
void MPI_same_group_tabulist_output(char * logfilename);
void out_put_MPI_same_tabulist(FILE * fp);
#endif
#endif
#ifdef DEBUG

#ifdef MPIMODE
void figure_of_match_num(int matched_num);
void mpi_comunication_log_manage(int type);
void mpi_comunication_log_init(void);   /* local function */
void mpi_send_num_add(void);            /* local function */
void mpi_recv_num_add(void);            /* local function */
void loging_mpi_com(void);              /* local function */
int get_num_of_all_proc(void);
int get_process_number(void);
#ifdef SEND_AMONGGROUP
void loging_share_tabulist(void);       /* local function */
int ** get_all_share_tabulist(void);
int get_share_tabulist_2opt_index(void);
#endif
#endif
#ifdef POLEDEBUG
void open_loging_other_sol_path(void);
void output_other_sol_path(void);
int * get_other_solution_path_data(void);
void close_loging_other_sol_path(void);
#endif

void open_loging_initial_path(void)
{
    if(get_process_number() < 10) {
         sprintf(debug_log_name, "debug_log/initial_path.0%d.log",get_process_number());
    }
    else {
         sprintf(debug_log_name, "debug_log/initial_path.%d.log",get_process_number());
    }

    if((debug_fp = fopen(debug_log_name, "a")) == NULL) {
        error_procedure("can\'t find \"debug_log\" directory");
    }
}

void loging_initial_path(int * path, int create_mode)
{
    int i;

    fprintf(debug_fp, "*** initial_path debug START ***\n");
    if(create_mode == DEFAULT) {
        fprintf(debug_fp, "=== DEFAULT MODE ===\n");
    }
    else if(create_mode == MERGECREATE) {
        fprintf(debug_fp, "=== MERGECREATE MODE ===\n");
    }
    else if(create_mode == GROUPCREATE) {
        fprintf(debug_fp, "=== GROUPCREATE MODE ===\n");
    }
    for(i = 0; i <= get_tsp_size(); i++) {
        fprintf(debug_fp, "No.%3d> city(%3d)\n",i, path[i]);
    }
    fprintf(debug_fp, "*** initial_path debug END ***\n");
}

void tabu_matching_loging(int type)
{
    switch(type) {
        case INIT:
            tabu_matching_init();
            break;
        case WHOLE:
            add_whole_check_num();
            break;
        case MATCH:
            add_matched_num();
            break;
        case CHECK:
            loging_tabu_matched();
            break;
#ifdef SEND_AMONGGROUP
        case SHARE:
            if(modep->parallel_mode == ON) {
                loging_share_tabulist();
            }
            break;
#endif
    }
}

void tabu_matching_init(void)
{
    tabu_matching_log.all_check_num = 0;
    tabu_matching_log.matched_num = 0;
}

void add_whole_check_num(void)
{
    tabu_matching_log.all_check_num++;
}

void add_matched_num(void)
{
    tabu_matching_log.matched_num++;
}

void loging_tabu_matched(void)
{
    fprintf(debug_fp, "*** TABU-LIST matched debug START ***\n");
    fprintf(debug_fp, "tabu_matched / whole matched : %d/%d\n",
        tabu_matching_log.matched_num, tabu_matching_log.all_check_num);
    tabu_matching_log.all_check_num = 0;
    tabu_matching_log.matched_num = 0;
    fprintf(debug_fp, "*** TABU-LIST matched debug END ***\n");
}

#endif
#ifdef MPIMODE
#ifdef SAMEGROUP_COMUNICATION_DEBUG
void MPI_same_group_tabulist_output(char * baselogfilename)
{
    FILE * fp;
    time_t timer;
    struct tm * date;
    char logfilename[256];

    date = localtime(&timer);

    sprintf(logfilename,"debug_log/%s",baselogfilename);
    if((fp = fopen(logfilename, "w")) != NULL) {
        out_put_MPI_same_tabulist(fp);
        fclose(fp);
    }

}
#endif
#endif
#ifdef DEBUG
#ifdef MPIMODE
#ifdef SEND_AMONGGROUP
void loging_share_tabulist(void)
{
    int i;
    int ** list = get_all_share_tabulist();
    int share_list_size = get_share_tabulist_2opt_index();

    fprintf(debug_fp, "*** SHARE-TABU-LIST START ***\n");
    fprintf(debug_fp, "(list[0],[1],[2],[3])\n\n");
    for(i = 0; i < share_list_size; i++) {
        fprintf(debug_fp, "list[%5d] == (",i);
        fprintf(debug_fp, "%d,", list[0][i]);
        fprintf(debug_fp, "%d,", list[1][i]);
        fprintf(debug_fp, "%d,", list[2][i]);
        fprintf(debug_fp, "%d", list[3][i]);
        fprintf(debug_fp, ")\n");
    }
    fprintf(debug_fp, "*** SHARE-TABU-LIST END ***\n");
}
#endif

void mpi_comunication_log_manage(int type)
{
    switch(type) {
        case INIT:
            mpi_comunication_log_init();
            break;
        case MPI_SENDADD:
            mpi_send_num_add();
            break;
        case MPI_RECVADD:
            mpi_recv_num_add();
            break;
        case CHECK:
            loging_mpi_com();
            break;
    }
}

void loging_mpi_com(void)
{
    fprintf(debug_fp, "*** MPI send/recv communication debug START ***\n");
    fprintf(debug_fp, "Number of MPI_Send:%d\n", mpi_com_log.mpi_send_num);
    fprintf(debug_fp, "Number of MPI_Recv:%d\n", mpi_com_log.mpi_recv_num);
    fprintf(debug_fp, "Maximum Number of Merge-Matched:%d/%d\n", mpi_com_log.max_num_of_match_num, get_tsp_size());
    fprintf(debug_fp, "*** MPI send/recv communication debug END ***\n");
}

void mpi_comunication_log_init(void)
{
    mpi_com_log.mpi_send_num = 0;
    mpi_com_log.mpi_recv_num = 0;
    mpi_com_log.max_num_of_match_num = 0;
}

void mpi_send_num_add(void)
{
    mpi_com_log.mpi_send_num++;
}

void mpi_recv_num_add(void)
{
    mpi_com_log.mpi_recv_num++;
}

void figure_of_match_num(int matched_num)
{
    if(matched_num > mpi_com_log.max_num_of_match_num) {
        mpi_com_log.max_num_of_match_num = matched_num;
    }
}
#endif
#ifdef POLEDEBUG
void output_other_sol_path(void)
{
    int * other_sol = get_other_solution_path_data();
    int * my_best_sol = get_best_solution_path();
    int data_cell_num = get_tsp_size() + DEFAULT_SENDPARAMETERNUM;
    int group_num = get_num_of_all_proc() / DEFAULT_MPIGROUPNUM - 1;
    int i, j;

    fprintf(debug_other_sol_fp, "+++POLEDEBUG+++\n\n");
    fprintf(debug_other_sol_fp, "data_cell_num == %d, DEF == %d, group_num == %d\n\n", data_cell_num, DEFAULT_SENDPARAMETERNUM, group_num);
    if(group_num == 1) {
        for(i = 0; i < data_cell_num; i++) {
            fprintf(debug_other_sol_fp, "%3d:(my, sol[1]) == (%3d, %3d)\n",i , my_best_sol[i], other_sol[i]);
        }
    }
    else if(group_num == 3) {
        for(i = 0; i < data_cell_num; i++) {
            fprintf(debug_other_sol_fp, "%3d:(sol[1], sol[2], sol[3]) == (%3d, %3d, %3d)\n",i , other_sol[i], other_sol[i + data_cell_num], other_sol[i + 2 * data_cell_num]);
        }
    }
    else {
        fprintf(debug_other_sol_fp, "!!! group_num == %d !!!\n", group_num);
    }
    fprintf(debug_other_sol_fp, "\n\n+++++++++++++++\n");
}

void open_loging_other_sol_path(void)
{
    if(get_process_number() < 10) {
         sprintf(debug_other_sol_log_name, "debug_log/other_sol_path.0%d.log",get_process_number());
    }
    else {
         sprintf(debug_other_sol_log_name, "debug_log/other_sol_path.%d.log",get_process_number());
    }

    if((debug_other_sol_fp = fopen(debug_other_sol_log_name, "a")) == NULL) {
        error_procedure("can\'t find \"debug_log\" directory");
    }
}

void close_loging_other_sol_path(void)
{
    fclose(debug_other_sol_fp);
}

#endif

void test_debug_log(char message[128], int num)
{
    if(num < 0) {
        fprintf(debug_fp, "%s\n", message);
    }
    else {
        fprintf(debug_fp, "%s%d\n", message, num);
    }
}

void close_loging_initial_path(void)
{
    fclose(debug_fp);
}
#endif
