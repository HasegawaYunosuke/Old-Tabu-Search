/* header files */
#include "header.h"

/* functions */
void final_result_show(void);
void finalize(void);
void parallel_finalize(void);

void finalize(void)
{
    final_result_show();
    if(modep->parallel_mode) {
        parallel_finalize();
    }
    printf("Program is normally terminated.....\n");
}
