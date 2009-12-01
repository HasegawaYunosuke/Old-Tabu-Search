/* header files */
#include "header.h"

/* functions */
void finalize(void);
void parallel_finalize(void);

void finalize(void)
{
    if(modep->parallel_mode) {
        parallel_finalize();
    }

    printf("Program is normally terminated.....\n");
}
