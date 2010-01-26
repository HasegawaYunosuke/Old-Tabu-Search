/* #define MPIMODE */
#define LINUXUSER
#define DEBUG
#define POLEDEBUG
#define CROSSOVER_BEF_AFT
//#define DISTANCE_LOG

/* include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <float.h>
#include <limits.h>
#ifdef MPIMODE
#include <mpi.h>
#define BEST_SOLUTION 101
#define LOGFILENAME 102
#endif

/* macro variable */
#define INIT 0
#define CLEAR 5
#define ADD 6
#define ON 1
#define OFF 0
#define YES ON
#define NO OFF
#define CHECK -1
#define READONLY -2
#define COUNT -3
#define DEFAULT 0
#define PERMITWORSE 10
#define SMART2OPT 20
#define TABU2OPT 30
#define SHORTMODE 40
#define MIDDLEMODE 50
#define FIRST_MIDDLEMODED 55
#define LONGMODE 60
#define WHOLE 70
#define MATCH 80
#define MERGECREATE 100
#define MPI_SENDADD 301
#define MPI_RECVADD 302

/* macro parameter */
#define TSPMAXSIZE 2000
#define DEFAULT_PERMITWORSE 0.05
#define DEFAULT_ADDPERMITWORSE 1
#define DEFAULT_SEARCHTIME 10
#define DEFAULT_CITYPOINT 5
#define DEFAULT_2OPTLOOP 100
#define DEFAULT_HISTORYSIZE 20
#define DEFAULT_SAMEPERCENTAGE 20
#define DEFAULT_NOTFOUNDLOOP 1000
#define DEFAULT_MPIGROUPNUM 4
#define DEFAULT_MIDDLEMANNNERI 100
#define DEFAULT_SENDPARAMETERNUM 10
#define GA_CROSS_POINT 10 
#define GA_COUNTER 100

/* grobal variable */
struct mode {
    int visual_mode;
    int realtime_visual_mode;
    int graph_mode;
    int euclid_mode;
    int parallel_mode;
    int hasegawa_mode;
    int pole_mode;
    int tozaki_mode;
    int tabu_mode;
    int tabu2opt_mode;
    int only2opt_mode;
    int ga_mode;
    int middle_manneri;
};

struct mode * modep;
pthread_mutex_t mutex;
pthread_t visual_thread;
