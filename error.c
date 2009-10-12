/* header files */
#include "header.h"

/* functions */
void error_procedure(char * message);

void error_procedure(char * message)
{
    fprintf(stderr,
    "\n!!! Error Message !!!\n\n%s's error.\nTry to use \"-h\" mean \"Help Documetn\" that written how to use this program.\n",message);
    fprintf(stderr,"\nProgram is terminated...\n");
    exit(1);
}
