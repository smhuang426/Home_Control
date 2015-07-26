#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

/*add for socket*/
#include "../Library/Network/Network.h"

/*define parameter*/

pthread_t thr1,thr2,thr3;

/*=======declare variable=========*/
/*socket variable*/
struct sockaddr_in dest,dest2;
int sock_fd,sock_fd2,client_fd;
struct sockaddr client_addr;
/*declare function*/
void *Network_handle(void* ptr);
void *Network_handle2(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1";
	int addr_len = sizeof(client_addr);;

	printf("start thread2\n");
	ret = pthread_create(&thr2,NULL,Network_handle2,str1);
	if (ret !=0)
	{
		printf("thread create fail");
	}

	sleep(5);

	pthread_join(thr1,NULL);
	pthread_join(thr2,NULL);
	//while loop
	while(1);
	return 0;


}

void *Network_handle2(void* ptr)
{
	NETWORK_CONFIG _config;
	char recv_buf2[20]="server recv";
	printf("[Client1]connecting!\n");

	Network_client_setup(&sock_fd2, &dest2, "192.168.1.104", 1250);
	printf("[Client1]connect setup\n");
	_config.fd = sock_fd2;
	_config.server_dest = dest2;
	while(1)
	{
		//int ret = recv(sock_fd2, recv_buf2, sizeof(recv_buf2), 0);
		int ret = Network_recv(_config, recv_buf2, sizeof(recv_buf2), 0);
		printf("[Client1]recieve %d bytes:%s, prepare to send buffer\n",ret,recv_buf2);
		sleep(2);
		send(sock_fd2, recv_buf2, sizeof(recv_buf2), 0);
	}
	//close(sock_fd2);
}
