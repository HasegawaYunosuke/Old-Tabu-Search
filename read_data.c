/* header files */
#include "header.h"

/* functions */
void read_data_set(char * tspfilename);
int * read_data(void);
int * mallocer_ip(int tsp_size);

/* groubal variable */
char * readfilename;

/* check comand-line's input filename */
void read_data_set(char * tspfilename)
{
    struct stat st;

    /* if the file is NOT exist */
    if(stat(tspfilename,&st) != 0) {
        error_procedure("The file is not found");
    }
    /* if the file is exist */
    else {
        readfilename = tspfilename;
    }
}

int * read_data(void)
{
    FILE * f;
    int f_flag = 1;
    int  d, dd, i,j, xy_flag, n_flag, ng_flag, comma, tsp_size, index;
    int * coordinate;
    int * main_base_data;
    int array[TSPMAXSIZE][2];
    char buffer[256];
    char * c;
    char * pre_c;

    d = dd = i = xy_flag = n_flag = ng_flag = comma = tsp_size = index = 0;

    f = fopen(readfilename,"r");

    while((c = fgets(buffer, sizeof(buffer),f)) != NULL) {
        for(;;c++) {
            if(c == NULL) {
                break;
            }
            if(*c == ' ' || *c == '\n' || *c == '\t' || *c == '\r') {
                ng_flag = 0;
                if(*c == '\n') {
                    n_flag = 1;
                    comma = index = xy_flag = 0;
                }
                else if(f_flag == 0 && n_flag != 1) {
                    if(*pre_c != ' ' && *pre_c != '\n' && *pre_c != '\t' && *pre_c != '\r') {
                        comma++;
                        if(comma > 1) {
                            continue;
                        }
                    }
                    xy_flag++;
                    continue;
                }
                else {
                continue;
                }
            }
            else if(*c >= '0' && *c <= '9') {
                dd = d = atoi(c);
                d = ((ng_flag == 1) ? -1 : 1 ) * d;
                ng_flag = 0;

                for(i = 0;dd != 0; i++) {
                    dd = dd / 10;
                }

                if(n_flag == 1 || f_flag == 1) {
                    f_flag = n_flag = 0;
                    tsp_size = index = d;
                } if(xy_flag != 0) { array[tsp_size][((xy_flag == 1) ? 0 : 1)] = d;
                }

                comma = 0;
                pre_c = c;
                if(!i) {
                    i = 1;
                }
                c = c + i - 1;
            }
            else if(*c == '-') { /* 負の数の処理 */
                ng_flag = 1;
            }
            else {
                break;
            }
        }
    }

    array[0][0] = tsp_size; array[0][1] = tsp_size;
    main_base_data = mallocer_ip(tsp_size);
    for(i = 0; i <= tsp_size; i++) {
        for(j = 0; j < 2; j++) {
            if(j == 0) {
                main_base_data[i * 2] = array[i][j];
            }
            else {
                main_base_data[i * 2 + 1] = array[i][j];
            }
        }
    }

    return main_base_data;
}
