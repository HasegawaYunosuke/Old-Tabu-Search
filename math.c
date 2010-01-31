/* header files */
#include "header.h"

/* functions */
double make_distance(int x1, int y1, int x2, int y2);
int random_num(int maximum);
int get_num_of_addtion_to_local_tabulist(int tsp_size);
#ifdef MPIMODE
#ifdef SEND_AMONGGROUP
int get_num_of_addtion_to_share_tabulist(int tsp_size);
#endif
#endif

double make_distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x1 - x2), 2.0) + pow((y1 - y2), 2.0));
}

int random_num(int maximum)
{
    int return_data;

    /* (1 <= return_data <= maximum) */
    if((return_data = rand() % maximum) == 0) {
        return_data = maximum;
    }
    if((return_data = (get_MPI_group_data() * get_process_number() + 1) * return_data % maximum) == 0) {
        return_data = maximum;
    }

    return return_data;
}

/*
 * This func is leaded by Examination of few times.
 * Based on the number of added local-tabu-list at Examination in 1 min.
 * Depend on PC-spec.
 * The more the num of argument ( tsp_size ) increase, the more this func's return-num decrease.
 * So, this func restrict Minimum-return_num to 1000. (its leaded by 'tsp_size == 1002')
 */
int get_num_of_addtion_to_local_tabulist(int tsp_size)
{
    int return_num;
    int minimum = 1000;

    return_num = (int)((tsp_size * tsp_size * 0.02045) - (38.1085 * tsp_size) + 18850.25803);
    if(return_num <= minimum) {
        return_num = minimum;
    }

    return return_num;
}

#ifdef MPIMODE
#ifdef SEND_AMONGGROUP
/*
 * This func is leaded by Examination of few times.
 * Based on the number of added share-tabu-list ( Duplicative added to local-tabu-list ) at Examination in 1 min.
 * Depend on PC-spec, the number of PC, and Network-trafic. ( 8 PCs, Score7.2 )
 * The more the num of argument ( tsp_size ) increase, the more this func's return-num decrease.
 * So, this func restrict Minimum-return_num to 100. (its leaded by 'tsp_size == 1002')
 */
int get_num_of_addtion_to_share_tabulist(int tsp_size)
{
    int return_num;
    int minimum = 100;

    return_num = (int)((tsp_size * tsp_size * 0.00682) - (11.34651 * tsp_size) + 4618.14888);
    if(return_num <= minimum) {
        return_num = minimum;
    }

    return return_num;
}
#endif
#endif
