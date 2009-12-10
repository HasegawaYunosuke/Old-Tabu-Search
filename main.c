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
void initialize(int argc, char ** argv);
int loop_terminate(void);
int search_terminate(void);
void initial_path(void);
void search(void);
void finalize(void);
void set_MPI_group(void);
int main(int argc, char ** argv)
{
    /* all in one initialize */
    initialize(argc, argv);

    /* timer start */
    timer(ON);

    /* whole-search loop */
    for(;;) {
        /* create initial-path by each mode */
        initial_path();
        /* search-turn loop */
        for(;;) {
            /* search */
            search();

            realtime_result();

            /* search-turn terminate */
            if(loop_terminate() == YES) {break;}
        }
        /* whole-search-terminate */
        if(search_terminate() == YES) {break;}
    }

    /* finalize procedure */
    finalize();

    sleep(5);

    return 0;
}
