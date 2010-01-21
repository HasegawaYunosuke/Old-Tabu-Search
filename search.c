/* header files */
#include "header.h"

/* functions */
void search(void);
int * hasegawa_search(int * solution_path);
int * tozaki_search(int * solution_path);
int * pole_search(int * solution_path);
int * get_solution_path(void);
void set_solution_path(int * solution_path);
#ifdef POLEDEBUG
#ifdef MPIMODE
void output_other_sol_path(void)
#endif
#endif

/* grobal variable */

/* search() will share function both Euclid and Graph as possible as we can */
void search(void)
{
    int * solution_path = get_solution_path();

    if(modep->hasegawa_mode == ON) {
        solution_path = hasegawa_search(solution_path);
    }
    else if(modep->tozaki_mode == ON) {
        solution_path = tozaki_search(solution_path);
    }
    else if(modep->pole_mode == ON) {
        solution_path = pole_search(solution_path);
#ifdef POLEDEBUG
#ifdef MPIMODE
        output_other_sol_path();
#endif
#endif
    }
    else {
        error_procedure("Un-define-search()-mode is choiced");
    }

    set_solution_path(solution_path);
}
