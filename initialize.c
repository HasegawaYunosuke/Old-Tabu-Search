/* header files */
#include "header.h"

/* functions */
void option_checker(int argc, char ** argv);
void initial_parameter(int * main_base_data);
void visualize_procedure(void);
void visualizer(int * visual_arg);
void make_graph(int * main_base_data);
void MPI_com_among_group(int argc, char ** argv);
void MPI_com_among_reader(void);
void debug_initialize(void);
int timer(int sign);
int num_counter(int field_type, int use_type);
double * mallocer_dp(int size);
double make_distance(int x1, int y1, int x2, int y2);
void set_graph_data(double * graph_data);
void initialize(int argc, char ** argv);
void set_MPI_parameter(void);
int * read_data(void);

#ifdef MPIMODE
void best_MPI_recv(int * recv_process_number);
int * get_same_group_list(void);
int get_all_MPI_group_data(void);
int get_process_number(void);
int get_group_reader(void);
void free_MPI_recv_thread(void);
void MPI_same_group_tabulist_init(int recv_thread_num);
#ifdef SEND_AMONGGROUP
void group_reader_process(void);
#endif
#endif
#ifdef DEBUG
void open_loging_initial_path(void);
void tabu_matching_loging(int type);
#endif
#ifdef POLEDEBUG
void open_loging_other_sol_path(void);
#endif
#ifdef CROSSOVER_BEF_AFT
void open_loging_x_sol_path(void);
#endif

/* grobal variable */
pthread_t * MPI_recv_thread;

void initialize(int argc, char ** argv)
{
    /* comand-line short option check */
    option_checker(argc, argv);

    /* set parameter */
    initial_parameter(read_data());

    /* create thread for visual-mode */
    visualize_procedure();

    /* Communication among Group (all process do if use MPI) */
    MPI_com_among_group(argc, argv);

    /* Communication among Group-Readers (if use MPI) */
    MPI_com_among_reader();

    /* debug procedure (if DEBUG-MODE) */
    debug_initialize();

    /* timer start */
    timer(ON);

    /* initialize search/loop counter */
    num_counter(INIT, INIT);
}

void visualize_procedure(void)
{
    int visual_arg;

    if(modep->visual_mode == ON) {
        pthread_mutex_init(&mutex, NULL);
        pthread_create(&visual_thread, NULL, (void *) visualizer, (void *) &visual_arg);
    }
}


void MPI_com_among_group(int argc, char ** argv)
{
    int i, recv_thread_num;
    int * other_list;

    #ifdef MPIMODE
    if(modep->parallel_mode == ON) {
        MPI_Init(&argc, &argv);
        set_MPI_parameter();
        recv_thread_num = get_all_MPI_group_data() - 1;
        MPI_recv_thread = (pthread_t *)malloc(recv_thread_num * sizeof(pthread_t));
        other_list = get_same_group_list();
        MPI_same_group_tabulist_init(recv_thread_num);
        for(i = 0; i < recv_thread_num; i++) {
            pthread_create(&MPI_recv_thread[i], NULL, (void *)best_MPI_recv, (void *)&other_list[i]);
        }
    }
    #endif
}

void MPI_com_among_reader(void)
{
    #ifdef MPIMODE
        #ifdef SEND_AMONGGROUP
        if(modep->parallel_mode == ON) {
            if(get_group_reader() == get_process_number()) {
                group_reader_process();
            }
        }
        #endif
    #endif
}

void debug_initialize(void)
{
    #ifdef DEBUG
    open_loging_initial_path();
    #endif
    #ifdef MPIMODE
        #ifdef POLEDEBUG
        if(modep->parallel_mode == ON) {
            open_loging_other_sol_path();
        }
        #endif
    #endif
    #ifdef DEBUG
    tabu_matching_loging(INIT);
        #ifdef CROSSOVER_BEF_AFT
        open_loging_x_sol_path();
        #endif
    #endif
}

void free_MPI_recv_thread(void)
{
    free(MPI_recv_thread);
}

void make_graph(int * main_base_data)
{
    double * return_data;
    double len;
    int i,j,xi,yi,xj,yj,size;

    size = main_base_data[0];

    return_data = mallocer_dp((size + 1) * (size + 1));

    return_data[0] = (double)size;
    for(i = 1; i <= size; i++) {
        xi = main_base_data[i * 2];  yi = main_base_data[i * 2 + 1];
        for(j = 1; j <= size ; j++) {
            xj = main_base_data[j * 2];  yj = main_base_data[j * 2 + 1];
            len = make_distance(xi, yi, xj, yj);
            return_data[i + size * j] = len;
        }
    }

    set_graph_data(return_data);
}
