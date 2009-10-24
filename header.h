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

/* macro variable */
#define INIT 0
#define CLEAR 5
#define ON 1
#define OFF 0
#define YES ON
#define NO OFF
#define CHECK -1
#define READONLY -2
#define DEFAULT 0
#define PERMITWORSE 10
#define SMART2OPT 20

/* macro parameter */
#define TSPMAXSIZE 2000
#define DEFAULT_PERMITWORSE 10
#define DEFAULT_SEARCHTIME 10
#define DEFAULT_CITYPOINT 5
#define DEFAULT_2OPTLOOP 100

/* grobal variable */
struct mode {
    int visual_mode;
    int graph_mode;
    int euclid_mode;
    int parallel_mode;
    int hasegawa_mode;
    int pole_mode;
    int tozaki_mode;
    int tabu_mode;
};

struct mode * modep;
pthread_mutex_t mutex;
