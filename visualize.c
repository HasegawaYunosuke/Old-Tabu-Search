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
int get_process_number(void);
#ifdef MPIMODE
int get_group_start_process(void);
#endif

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
    ServAddr.sin_addr.s_addr = inet_addr("192.168.1.10");
    ServAddr.sin_port = htons(10003);

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
    int flag = -1;
    int i;
    int prev_loop = turn_loop_times(READONLY);
    int mainX_max = 0;
    int mainX_min = 0;
    int mainY_max = 0;
    int mainY_min = 0;
    int x = 0,y = 0;
    int a = 2,b = 3;
    int start_para_num;
    int my_para_num;

    socket = -100;

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
            a += 2;
        }
    }

    if(mainY_max > 300){
        y = mainY_max / 300;
        for(i = 0; i < nt_city_coordinate[0]; i++){
            nt_city_coordinate[b] = nt_city_coordinate[b] / y;
            b += 2;
        }
    }

    if(tsp_size == 51){
        for(i = 0; i < nt_city_coordinate[0]; i++){
	    nt_city_coordinate[a] = nt_city_coordinate[a] * 2;
	    a += 2;
	}
    	for(i = 0; i < nt_city_coordinate[0]; i++){
	    nt_city_coordinate[b] = nt_city_coordinate[b] * 2;
	    b += 2;
	}
    }

    while((solu_path = get_solution_path()) == NULL) {
    }

    my_para_num = get_process_number();
#ifdef MPIMODE
    start_para_num = get_group_start_process();
    if(my_para_num == start_para_num){
	socket = clntSock();
    }
    else{
	flag = 10;
    }
#endif
    if(socket == -100){
	if(flag == -1){
	    socket = clntSock();
	}
    }

    if(socket != -100){
        send(socket, nt_city_coordinate, (nt_city_coordinate[0]+1)*2*4,0);
    }

    solu_path[tsp_size+1] = (int)get_all_cost_by_graph(get_solution_path());
    solu_path[tsp_size+2] = my_para_num;

    for(;;){
        for(;;) {
            if(turn_loop_times(READONLY) != prev_loop) {
                prev_loop = turn_loop_times(READONLY);
                break;
            }
        }
#ifdef MPIMODE
        if(my_para_num != start_para_num) {
            for(;;){
                if(search_is_done(READONLY) == YES) {
                    printf("visualize.c:All Search is Done...\n");
                    solu_path[0] = -1;
                    break;
                }
            }
            break;
        }
#endif

	send(socket, solu_path, (solu_path[0]+3)*4,0);
        
        solu_path = NULL;
        solu_path = get_solution_path();
        solu_path[tsp_size+1] = (int)get_all_cost_by_graph(get_solution_path());
	    solu_path[tsp_size+2] = my_para_num;
        
        if(search_is_done(READONLY) == YES) {
            printf("visualize.c:All Search is Done...\n");
            solu_path[0] = -1;
            send(socket, solu_path, (solu_path[0]+3)*4,0);
            break;
        }
    }
    cleanSock(socket);
}
