/* header files */
#include "header.h"

/* functions */
double make_distance(int x1, int y1, int x2, int y2);

double make_distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x1 - x2), 2.0) + pow((y1 - y2), 2.0));
}
