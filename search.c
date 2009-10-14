/* header files */
#include "header.h"

/* functions */
int * search(int * solution_path);
int * hasegawa_search(int * solution_path);
int * tozaki_search(int * solution_path);

/* grobal variable */

/* search() will share function both Euclid and Graph as possible as we can */
int * search(int * solution_path)
{
    int * return_data;

    if(modep->hasegawa_mode == ON) {
        return_data = hasegawa_search(solution_path);
        sleep(1);
    }
    else if(modep->tozaki_mode == ON) {
        return_data = tozaki_search(solution_path);
        sleep(1);
    }
    else {
        sleep(1);
    }

    return return_data;
}
