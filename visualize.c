/* header files */
#include "header.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

/* functions */
void visualizer(int * visual_arg);
int search_is_done(int type);
int get_tsp_size(void);
int * get_solution_path(void);
int * get_main_base_data(void);
double get_all_cost_by_graph(int * cities);
int get_solution_data_flag(void);
int turn_loop_times(int type);

int clntSock(void)
{
    int sock;
    int sock_optval = 1;
    unsigned short ServPort;
    unsigned int StiringLen;
    struct sockaddr_in ServAddr;

    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr,"clntSock() Error.\n");
        exit(1);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sock_optval, sizeof(sock_optval));

    if(sock < 0){
        printf("error sock failed\n");
        exit(-1);
    }
    else {
        printf("Socket Success\n");
    }

    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr("192.168.11.10");
    ServAddr.sin_port = htons(10001);

    if(connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) == -1) {
        printf("error connect failed\n");
        exit(1);
    }
    else {
        printf("Connect Success\n");
    }

    return sock;
}

void cleanSock(int socket)
{
    printf("Socket Close\n");
    close(socket);
}

void visualizer(int * visual_arg)
{
    int tsp_size = get_tsp_size();
    int * nt_city_coordinate;
    int * solu_path;
    int socket;
    int i;
    int prev_loop = turn_loop_times(READONLY);
    int mainX_max = 0;
    int mainX_min = 0;
    int mainY_max = 0;
    int mainY_min = 0;
    int x = 0,y = 0;
    int a = 2,b = 3;

    socket = clntSock();

    nt_city_coordinate = get_main_base_data();
    mainX_min = nt_city_coordinate[2];
    mainY_min = nt_city_coordinate[3];

    for(i = 2; i < (nt_city_coordinate[0]+1)*2 ;i++){
        if(i%2 == 0){
            if(mainX_max < nt_city_coordinate[i]){
                mainX_max = nt_city_coordinate[i];
            }
            if(mainX_min > nt_city_coordinate[i]){
                mainX_min = nt_city_coordinate[i];
            }
        }
        else{
            if(mainY_max < nt_city_coordinate[i]){
                mainY_max = nt_city_coordinate[i];
            }
            if(mainY_min > nt_city_coordinate[i]){
                mainY_min = nt_city_coordinate[i];
            }
        }
    }

    if(mainX_max > 500){
        x = mainX_max / 470;
        for(i = 0; i < nt_city_coordinate[0]; i++){
            nt_city_coordinate[a] = nt_city_coordinate[a] / x;
            printf("X[%d]:%d\n", a,nt_city_coordinate[a]);
            a += 2;
        }
    }

    if(mainY_max > 300){
        y = mainY_max / 300;
        printf("%d\n",y);
        for(i = 0; i < nt_city_coordinate[0]; i++){
            nt_city_coordinate[b] = nt_city_coordinate[b] / y;
            printf("Y[%d]:%d\n",b,nt_city_coordinate[b]);
            b += 2;
        }
    }

    send(socket, nt_city_coordinate, (nt_city_coordinate[0]+1)*2*4,0);

    while((solu_path = get_solution_path()) == NULL) {
        printf("FUCK:\n");
    }

    //printf("maxX:%d\nminX:%d\nmaxY:%d\nminY:%d\n",mainX_max,mainX_min,mainY_max,mainY_min);

    solu_path[tsp_size+1] = (int)get_all_cost_by_graph(get_solution_path());

    for(;;){
        for(;;) {
            if(turn_loop_times(READONLY) != prev_loop) {
                prev_loop = turn_loop_times(READONLY);
                break;
            }
        }
        send(socket, solu_path, (solu_path[0]+2)*4,0);
        solu_path = NULL;
        solu_path = get_solution_path();
        solu_path[tsp_size+1] = (int)get_all_cost_by_graph(get_solution_path());
        if(search_is_done(READONLY) == YES) {
            printf("visualize.c:All Search is Done...\n");
            solu_path[0] = 0;
            send(socket, solu_path, (solu_path[0]+2)*4,0);
            break;
        }

    }

    cleanSock(socket);
}
