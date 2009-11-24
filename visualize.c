/* header files */
#include "header.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

/* functions */
void visualizer(int * visual_arg);
int get_tsp_size(void);
int * get_solution_path(void);
int * get_main_base_data(void);
double get_all_cost_by_graph(int * cities);
int get_solution_data_flag(void);
int turn_loop_times(int type);

int clntSock(void)
{
	int sock;
	unsigned short ServPort;
	unsigned int StiringLen;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		printf("error sock failed\n");
		exit(-1);
	}
	else
		printf("Socket Success\n");

	struct sockaddr_in ServAddr;
	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr("192.168.11.10");
	ServAddr.sin_port = htons(10001);

	if( connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) == -1)
	{
		printf("error connect failed\n");
		exit(1);
	}
	else
		printf("Connect Success\n");

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
	int tsp_size1 = (tsp_size+1)*4;
	int * nt_city_coordinate;		//座標を格納します
	int * solu_path;
	int * soiya;
	int waki = 0;
	char har_city_coordinate[10000];
	char city_solution[10000];
	int socket;
	double lll = 0;
	int prev_loop = turn_loop_times(READONLY);
	socket = clntSock();

	nt_city_coordinate = get_main_base_data();
	
	int k = 0,j = 0;

	send(socket, nt_city_coordinate, (nt_city_coordinate[0]+1)*2*4,0);

	solu_path = get_solution_path();
	solu_path[tsp_size+1] = (int)get_all_cost_by_graph(get_solution_path());
//	printf("fasdfafdadafadfaf\n");
//	printf("AllCost:%d\n", solu_path[tsp_size+1]);
//	printf("opppppppppppppppp\n");

	for(;;){
		
		j = 0;
		k = 0;

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
//		printf("fasdfafdadafadfaf\n");
//		printf("AllCost:%d\n", solu_path[tsp_size+1]);
//		printf("opppppppppppppppp\n");


	}

//    pthread_mutex_unlock(&mutex);
//	free((void *)har_city_coordinate);

	cleanSock(socket);
}
