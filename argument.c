/* header files */
#include "header.h"

/* functions */
void option_checker(int argc, char ** argv);
void set_mode(void);
void set_euclid_mode(void);
void set_visual_mode(void);
void set_parallel_mode(void);
void set_pole_mode(void);
void set_tozaki_mode(void);
void help_document(char ** argv);
void error_procedure(char * message);
void set_time(int mode, int parametor);
void read_data_set(char * tspfilename);

/* option_checker()'s role is ONLY check "short option" */
void option_checker(int argc, char ** argv)
{
    int opt;

    /* check the number of argument */
    if(argc == 1) {
        error_procedure("number of argument");
    }

    /* set all modes to OFF */
    set_mode();

    /* check short options */
    while((opt = getopt(argc, argv, "mbevplzt:f:h")) != -1) {
        switch(opt) {
            case 'b':
                set_tabu2opt_mode();
                break;
            case 'm':
                set_parallel_mode();
                break;
            case 'e':
                set_euclid_mode();
                break;
            case 'v':
                set_visual_mode();
                break;
            case 'p':
                set_parallel_mode();
                break;
            case 'l':
                set_pole_mode();
                break;
            case 'z':
                set_tozaki_mode();
                break;
            case 't':
                set_time(INIT,atoi(optarg));
                break;
            case 'f':
                read_data_set(optarg);
                break;
            case 'h':
                if(argc == 2) {
                    help_document(argv);
                }
                break;
            case '?':
                error_procedure("comand-line option");
                break;
        }
    }
}

void help_document(char ** argv)
{
    printf("\n+++ Help Document +++\n\n");

    printf("\nIntroduction --->\n\n");
    printf("This program is made by Yunosuke Hasegawa, Yuya Tanaka, Takashi Waki, and Yoshihito Tozaki.\n");
    printf("The role of this program is solving TSP using Tabu-Search.\n");
    printf("Written by C language, and Java. Java only use in case of \"graphical-mode\".\n\n");
    printf("Usage: %s [-m] [-b] [-e] [-p] [-l] [-z] [-h] [-v] [-t search_time[s]] [-f tspfile]\n",argv[0]);

    printf("\nOption Detail --->\n\n");
    printf("%s -m\n",argv[0]);
    printf("\tUse SCore System\n");
    printf("%s -b\n",argv[0]);
    printf("\tChoice Search Strategy \"Tabu-Search\" instead of \"2-opt Only\"\n");
    printf("%s -e\n",argv[0]);
    printf("\tChoice \"Euclid Mode\". If you don't set this option, \"Graph Mode\" is chosen.\n");
    printf("%s -p\n",argv[0]);
    printf("\tChoice \"Parallel Mode\".\n");
    printf("%s -l\n",argv[0]);
    printf("\tChoice \"Pole Mode\".\n");
    printf("%s -z\n",argv[0]);
    printf("\tChoice \"Tozaki Mode\".\n");
    printf("%s -h\n",argv[0]);
    printf("\tThis Help Document.\n");
    printf("%s -v\n",argv[0]);
    printf("\tSwitch to Visualize Mode.\n");
    printf("%s -t [search_time[second]]\n",argv[0]);
    printf("\tSet the Tabu-Search running time.\n");
    printf("%s -f [tspfile]\n",argv[0]);
    printf("\tSet TSP Example File. (ONLY adaptive \"TSPLIB\" Benchmark Problems)\n");

    exit(0);
}
