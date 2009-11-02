/* header files */
#include "header.h"

/* functions */
int check_manneri(int type);
int check_fill(void);
int check_historical_similar(void);

int check_manneri(int type)
{
    int return_num = NO;

    switch(type) {
        case SHORTMODE:
            return_num = short_manneri();
            break;
        case MIDDLEMODE:
            break;
        case LONGMODE:
            break;
    }

    return return_num;
}

int short_manneri(void)
{
    if(check_fill() == NO) {
        return NO;
    }

    return check_historical_similar();
}
