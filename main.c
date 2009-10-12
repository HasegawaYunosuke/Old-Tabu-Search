/*
    *** READ ME ***

    This program is made by Yunosuke Hasegawa, Yuya Tanaka, Takashi Waki, and Yoshihito Tozaki.
    The role of this program is solving TSP using Tabu-Search that is one of the most famous meta-heuristic method.
    Written by C language, and Java. (Java only use in case of "graphical-mode")
    I'll show you to how to compile it. You would find out some C-document.
    So, type to comand-line according to following.

        $ ./compile.sh

    If compile is terminated by something error, please complain to writer.
*/

/* header files */
#include "header.h"

/* functions */
void option_checker(int argc, char ** argv);
int * read_data(void);
double * make_graph(int * main_base_data);
void visualizer(int * visual_arg);
void initial_parameter(int tsp_size);
int loop_terminate(void);
int search_terminate(void);
int * initial_euclid_path(int * euclid);
int * initial_graph_path(double * graph);
void search(void);
void finalize(void);

/* grobal variable (for visual mode thread) */
int * main_base_data;
int * solution_path_for_visual;

int main(int argc, char ** argv)
{
    double * graph_data;
    int visual_arg; /* Please edit !! for Waki */
    pthread_t visual_thread;
    int * solution_path;
    int test;

    /* comand-line short option check */
    option_checker(argc, argv);

    /* read TSPLIB's explain data */
    main_base_data = read_data();

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
        pthread_join(visual_thread, NULL);
    }

    /* set parameter to 0 */
    initial_parameter(main_base_data[0]);

    /* timer start */
    timer(ON);

    /* whole-search loop */
    for(;;) {
        /* create initial-path by each mode */
        if(modep->euclid_mode == ON) {
            solution_path = initial_euclid_path(main_base_data);
        }
        else {
            solution_path = initial_graph_path(graph_data);
        }

        /* search-turn loop */
        for(;;) {
            /* search */
            search();

            /* search-turn terminate */
            if(loop_terminate() == YES) {
                break;
            }
        }
        /* whole-search-terminate */
        if(search_terminate() == YES) {
            break;
        }
    }
    /* finalize procedure */
    finalize();

    return 0;
}
