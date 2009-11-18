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
	int waki = 0;
	char har_city_coordinate[2048];
	char city_solution[2000];
	int socket;
	//socket = clntSock();

	nt_city_coordinate = get_main_base_data();
	
	/*if((har_city_coordinate = (char *)malloc((size_t)tsp_size1)) == NULL) {
			fprintf(stderr,"DAMEDAKORYA\n");
			exit(1);
	}*/

	int k = 0,j = 0;

	for(;;) {
		if(nt_city_coordinate[k] < 10){
			//printf("k = %d\n",k);
			har_city_coordinate[j] = nt_city_coordinate[k] + 48;
			//printf("k = %d\n",k);
			//sprintf(har_city_coordinate, "%d", nt_city_coordinate);
			j++;
			k++;
		}
		else if(nt_city_coordinate[k] < 100){
			//printf("k = %d\n",k);
			har_city_coordinate[j] = nt_city_coordinate[k] / 10 + 48;
			//printf("k = %d\n",k);
			//itoa(nt_city_coordinate[k]/10,har_city_coordinate[j],10);
			j++;
			har_city_coordinate[j] = nt_city_coordinate[k] % 10 + 48;
			//itoa(nt_city_coordinate[k]%10,har_city_coordinate[j],10);
			j++;
			k++;
		}
		else if(nt_city_coordinate[k] < 1000){
			har_city_coordinate[j] = nt_city_coordinate[k] / 100 + 48;
			//itoa(nt_city_coordinate[k]/100,har_city_coordinate[j],10);
			j++;
			har_city_coordinate[j] = nt_city_coordinate[k] / 10 % 10 + 48;
			//itoa(nt_city_coordinate[k]/10%10,har_city_coordinate[j],10);
			j++;
			har_city_coordinate[j] = nt_city_coordinate[k] % 10 + 48;
			//itoa(nt_city_coordinate[k]%10,har_city_coordinate[j],10);
			j++;
			k++;
		}
		har_city_coordinate[j] = ',';
		j++;
		if(k == tsp_size * 2 + 2) {
				break;
		}
	}
	k = 0;
	j = 0;
	int kaigyo = 0;	

	/*for(waki=0; waki < (tsp_size + 1) * 11; waki++) {
		if(har_city_coordinate[waki] == ',') {
			kaigyo++;
			if(kaigyo == 1) {
				printf("%c",har_city_coordinate[waki]);
			}
			else if(kaigyo == 2) {
				printf("%c\n",har_city_coordinate[waki]);
				kaigyo = 0;
			}
		}
		else {
		    printf("%c",har_city_coordinate[waki]);
	    }
	}
	*/
	
	printf("Finish?\n");

	//send(socket, har_city_coordinate, strlen(har_city_coordinate),0);

    pthread_mutex_lock(&mutex);

	sleep(3);

	solu_path = get_solution_path();
	if(solu_path == NULL) {
		printf("get_tsp_size == %d\n",get_tsp_size());
	}
	else {
		printf("zakiiiiiiii\n");
	}


	for(waki=0;waki<1;waki++){
		printf("%d\n",solu_path[waki]);
	}

	printf("11111111\n");

	for(;;){
		
		printf("LOOO\n");

		if(solu_path[k] < 10){
			city_solution[j] = solu_path[k] + 48;
			j++;
			k++;
		}
		else if(solu_path[k] < 100){
			city_solution[j] = solu_path[k] / 10 + 48;
			j++;
			city_solution[j] = solu_path[k] % 10 + 48;
			j++;
			k++;
		}
		else if(solu_path[k] < 1000){
			city_solution[j] = solu_path[k] /100 + 48;
			j++;
			city_solution[j] = solu_path[k] / 10 % 10 + 48;
			j++;
			city_solution[j] = solu_path[k] % 10 + 48;
			j++;
			k++;
		}
		city_solution[j] = ',';
		j++;

		city_solution[j] = '\0';
		j = 0;
		k = 0;

		send(socket, city_solution, strlen(city_solution),0);
		sleep(1);
	}

    /*for(i = 0; i < 100; i++) {
        printf("main_base_data[%d] == %d\n",i,main_base_data[i]);
    }*/

    pthread_mutex_unlock(&mutex);
//	free((void *)har_city_coordinate);

	cleanSock(socket);
}


