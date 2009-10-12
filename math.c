/* header files */
#include "header.h"

/* functions */
double make_distance(int x1, int y1, int x2, int y2);
int random_num(int maximum);

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

    return return_data;
}
