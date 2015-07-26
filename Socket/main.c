#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

/*add for socket*/
#include "../Library/Network/Network.h"

/*add for Link_list*/
#include "../Library/Link_list/Link_list.h"

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

//extern LINK_NODE* _network_link_data;
LINK_NODE* temp;
int main()
{
	int ret = 0;
	char* str1="test1";
	int addr_len = sizeof(client_addr);

	printf("socket initial\n");
	Network_server_setup(&sock_fd, 1250);

	while(1)
	{
		printf("wait for connect\n");
		//loop in sokcet accept
		client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &addr_len);
		printf("accept fd:%d\n",client_fd);

		//Network_server_add_link_data(client_fd, client_addr);
		//printf("the fd is :%d ,%p\n",((NETWORK_CONFIG*)temp->data)->fd,((NETWORK_CONFIG*)_network_link_data->data));	
		//printf("the fd is :%d ,%p\n",((NETWORK_CONFIG*)_network_link_data->data)->fd,((NETWORK_CONFIG*)_network_link_data->data));	
		ret = pthread_create(&thr1,NULL,Network_handle,&client_fd);
		if (ret !=0)
		{
			printf("thread create fail");
		}
	}

	pthread_join(thr1,NULL);
	pthread_join(thr2,NULL);
	//while loop
	while(1);
	return 0;


}

void *Network_handle(void* ptr)
{
	int fd=*(int*)ptr;
	//struct sockaddr client_addr;
	char buff[11] = "hello world",recv_buf[20];
	//Network_server_setup(&sock_fd, 1250);
	printf("[Server]thread start ,fd=%d!\n",client_fd);

	send(fd, buff, sizeof(buff), 0);
	while(1){
	/*wait for connect*/
		printf("wait for recv\n");
		int ret = recv(fd, recv_buf, sizeof(recv_buf), 0);
		if (ret <= 0)
		{
			printf("disconnect with client\n");

			close(fd);
			return ;
		}
		printf("[Server]recieve %d bytes:%s\n",ret,recv_buf);

		sleep(2);

		send(fd, buff, sizeof(buff), 0);
	//close(client_fd);
	}
}

void *Network_handle2(void* ptr)
{
	char recv_buf2[20]="server recv";
	printf("[Client1]connecting!\n");
	Network_client_setup(&sock_fd2, &dest2, "192.168.1.105", 1250);
	printf("[Client1]connect setup\n");
	int ret = recv(sock_fd2, recv_buf2, sizeof(recv_buf2), 0);

	printf("[Client1]recieve %d bytes:%s, prepare to send buffer\n",ret,recv_buf2);

	send(sock_fd2, recv_buf2, sizeof(recv_buf2), 0);

	//close(sock_fd2);
}
