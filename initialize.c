/* header files */
#include "header.h"

/* functions */
double * make_graph(int * main_base_data);
double * mallocer_dp(int size);
double make_distance(int x1, int y1, int x2, int y2);
void set_graph_data(double * graph_data);
void initialize(int argc, char ** argv);
void set_MPI_parameter(void);
void option_checker(int argc, char ** argv);
void visualizer(int * visual_arg);
int * read_data(void);

#ifdef MPIMODE
void best_MPI_recv(int * recv_process_number);
int * get_same_group_list(void);
int get_all_MPI_group_data(void);
int get_process_number(void);
int get_group_reader(void);
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
void initialize(int argc, char ** argv)
{
    int i;
    int visual_arg;
    int recv_thread_num;
    int * main_base_data;
    int * other_list;
    double * graph_data;
    pthread_t * MPI_recv_thread;

    /* comand-line short option check */
    option_checker(argc, argv);

    /* read TSPLIB's explain data */
    main_base_data = read_data();

    /* set parameter to 0 */
    initial_parameter(main_base_data[0]);

    /* set main_base_data to parameter.c */
    set_main_base_data(main_base_data);

    /* change data type ( Euclid -> Graph ) */
    if(modep->graph_mode == ON) {
        graph_data = make_graph(main_base_data);
    }

    /* create thread for visual-mode */
    if(modep->visual_mode == ON) {
        pthread_mutex_init(&mutex, NULL);
        pthread_create(&visual_thread,
                        NULL,
                        (void *) visualizer,
                        (void *) &visual_arg);
//        pthread_join(visual_thread, NULL);
    }

    #ifdef MPIMODE
    if(modep->parallel_mode == ON) {
        MPI_Init(&argc, &argv);
        set_MPI_parameter();
        recv_thread_num = get_all_MPI_group_data() - 1;
        MPI_recv_thread = (pthread_t *)malloc(recv_thread_num * sizeof(pthread_t));
        other_list = get_same_group_list();
        for(i = 0; i < recv_thread_num; i++) {
            pthread_create(&MPI_recv_thread[i],
                            NULL,
                            (void *)best_MPI_recv,
                            (void *)&other_list[i]);
        }
    }
    #endif

    #ifdef DEBUG
    open_loging_initial_path();
    #endif
    #ifdef POLEDEBUG
    open_loging_other_sol_path();
    #endif
    #ifdef DEBUG
    tabu_matching_loging(INIT);
    #endif
    #ifdef CROSSOVER_BEF_AFT
    open_loging_x_sol_path();
    #endif
    #ifdef DISTANCE_LOG
    open_distance_log();
    #endif

    /* Communication among Group-Readers */
    #ifdef MPIMODE
    #ifdef SEND_AMONGGROUP
    if(get_group_reader() == get_process_number()) {
        group_reader_process();
    }
    #endif
    #endif

}

double * make_graph(int * main_base_data)
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

    return return_data;
}
