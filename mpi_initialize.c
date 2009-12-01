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

void initialize(int argc, char ** argv)
{
    int * main_base_data;
    double * graph_data;
    int visual_arg; /* Please edit !! for Waki */
    pthread_t visual_thread;

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

    /* if Using Score System */
    if(modep->parallel_mode == ON) {
        MPI_Init(&argc, &argv);
        set_MPI_parameter();
    }
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
