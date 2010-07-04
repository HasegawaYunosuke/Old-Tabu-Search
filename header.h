/* #define MPIMODE */
#define LINUXUSER
//#define DEBUG
//#define POLEDEBUG
//#define CROSSOVER_BEF_AFT
#define SEND_AMONGGROUP
#define ORDER_GA            /* Better GA (Pole's examination) */
//#define MERGE_GA
#ifdef MPIMODE
    #define SAMEGROUP_COMUNICATION
    #define SAMEGROUP_COMUNICATION_DEBUG
    #define NONLEADER_NOT_USE_TWOOPTONLY        /* !Only one choice! NONLEADER_NOT_USE_TWOOPTONLY available in case of MPI*/
    //#define BOTH_NOT_USE_TWOOPTONLY             /* !Only one choice! BOTH_NOT_USE_TWOOPTONLY available in case of MPI*/
    //#define LEADER_NOT_USE_TWOOPTONLY           /* !Only one choice! LEADER_NOT_USE_TWOOPTONLY available in case of MPI*/
#endif

#define DELDELDEL

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
#include <getopt.h>
#ifdef MPIMODE
#include <mpi.h>
#define BEST_SOLUTION 101
#define GROUP_SOLUTION 102
#define LOGFILENAME 103
#define SOL_PATH_SHARE 104
#define TABU_LIST_SHARE 105
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
#define SHARE -4
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
#define UPPER 90
#define DOWNER 95
#define TURN_COUNTER 100
#define SEARCH_COUNTER 101
#define SOL_PATH 102
#define OTHER_SOL_PATH 103
#define FOUR_CITIES 104
#define MERGECREATE 300 /* init-path*/
#define GROUPCREATE 301 /* init-path*/
#define MPI_SENDADD 302
#define MPI_RECVADD 303
#define SAMEGROUP_TABULIST_SMART_CHOICE 304
#define CHOICE_NEVER_CHOICED 305
#define SAMETABULIST 306 /* init-path*/

/* macro parameter */
#define TSPMAXSIZE 2000
#define DEFAULT_PERMITWORSE 0.05
#define DEFAULT_ADDPERMITWORSE 1
#define DEFAULT_SEARCHTIME 10
#define DEFAULT_CITYPOINT 5
#define DEFAULT_2OPTLOOP 100
#define DEFAULT_HISTORYSIZE 20
#define DEFAULT_SAMEPERCENTAGE 20
#define DEFAULT_NOTFOUNDLOOP 500
#define DEFAULT_MPIGROUPNUM 4
#define DEFAULT_MIDDLEMANNNERI 100
#define DEFAULT_SENDPARAMETERNUM 10
#define DEFAULT_GROUP_DATASTOCKNUM 10
#define DEFAULT_TABU_MATCH_NUM 10000
#define DEFAULT_SHARE_TABU_MATCH_NUM 15
#define DEFAULT_SAMEGROUP_TABULISTSIZE_DONOMINATOR 10
#define GA_CROSS_POINT 10
#define GA_COUNTER 100
#define TABU_CLEAR_COUNT 5
#define LONGMANNERI_INTERBAL 30

/* grobal variable */
struct mode {
    int visual_mode;
    int realtime_visual_mode;
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
