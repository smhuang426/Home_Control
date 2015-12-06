#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <unistd.h>
#include "Network.h"
#include "../Link_list/Link_list.h"

#define NETWORK_WAIT_RECONNECT		5
#define NETWORK_RETRY_TIMES		10

/*variable define*/
LINK_NODE *_network_link_data = NULL;

/*implement function*/
LINK_NODE* Network_get_link_data(void)
{
	return (LINK_NODE*)_network_link_data;
}

int8_t Network_server_add_link_data(int fd, struct sockaddr client_addr)
{
	NETWORK_CONFIG config;
	config.fd = fd;
	config.client_addr = client_addr;
	
	if (_network_link_data == NULL)
	{
		//Link_list_init_with_LinkList(&config, _network_link_data);
		_network_link_data = Link_list_init(&config);

		printf("sector1\n");
		printf("_network_link_data = %i\n",_network_link_data->fd);
		printf("sector2\n");
	}
	else
	{
		Link_list_insert_end(_network_link_data, &config);
	}
}


int8_t Network_remove_link_data_by_fd(int fd)
{
	LINK_NODE* temp = _network_link_data;
	printf("_network_link_data = %d\n",_network_link_data->data->fd);

	while (temp != NULL)
	{
		printf("((NETWORK_CONFIG*)(temp->data))->fd = %d\n",((NETWORK_CONFIG*)(_network_link_data->data))->fd);
		if ((((NETWORK_CONFIG*)temp->data)->fd) == fd)
		{
			Link_list_remove_node(_network_link_data, temp);
			
			printf("[Neteork_remove_link_data_by_fd]remove one data");
			return NETWORK_DATA_VALIDATE;
		}

		temp = temp->next;
	}

	printf("[Neteork_remove_link_data_by_fd]no match file\n");
	return NETWORK_DATA_INVALIDATE;
}

int Network_server_setup(int* _sock_fd, int port)
{
	struct sockaddr_in _dest;
        *_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
 
        /*initial variable dest*/
        bzero(&_dest , sizeof(struct sockaddr_in));
        _dest.sin_family = AF_INET;
        _dest.sin_port = htons(port);
        _dest.sin_addr.s_addr = INADDR_ANY;  //for server
 
        /*assign a port to socket*/
        if (bind(*_sock_fd, (struct sockaddr*)&_dest, sizeof(struct sockaddr_in)))
	{
		perror("bind error");
		exit(1);
	}

        /*make it to listen socket*/
        if (listen(*_sock_fd, MAX_CONNECTION) != 0)
	{
		perror("listen error");
		exit(1);
	}
}


int Network_connect(int _sock_fd, struct sockaddr_in* _dest, socklen_t address_len, uint8_t _retry)
{
	uint8_t current_retry = 0;

	while (current_retry <= _retry)
	{
		if (connect(_sock_fd, (struct sockaddr*)_dest, address_len) != NETWORK_SUCCESS)
		{
			//perror("connect error");
			printf("[Network_connect]connect fiail, has retry %d times\n",++current_retry);
		}
		else
		{
			//connect successful!
			printf("[Network_connect]Connect Success\n");
			return NETWORK_SUCCESS;
		}
		sleep(NETWORK_WAIT_RECONNECT);
	}
	return NETWORK_FAIL;
}

int Network_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int client_fd ;
    
    client_fd = accept(sock_fd, addr, addr_len);
    
    if (client_fd < 0)
    {
        return client_fd;
    }
    
    //TODO: add client fd to link list
    
    return client_fd;
    
}

int Network_client_setup(int* _sock_fd, struct sockaddr_in* _dest, char* ip_addr, int port)
{	
        *_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
 
        /*initial variable dest*/
        bzero(_dest , sizeof(struct sockaddr_in));
        _dest->sin_family = AF_INET;
        _dest->sin_port = htons(port);
        _dest->sin_addr.s_addr = inet_addr(ip_addr);

	/*connect to server*/
	if (Network_connect(*_sock_fd, _dest, sizeof(struct sockaddr_in), NETWORK_RETRY_TIMES) != NETWORK_SUCCESS)
	{
		perror("[Network_client_setup]connect error");
		exit(1);
	}
	return NETWORK_SUCCESS;
}


ssize_t Network_recv(NETWORK_CONFIG config, void *buf, size_t size, int flags)
{
	int ret = recv(config.fd, buf, size, 0);
	if (ret <= 0 )
	{
		close(config.fd); //change to Network_disconnect() later.
		printf("[Network_recv]disconnect occur,re-connect start\n");
		//	someday need to re-desing this ip address ,not hard code
		Network_client_setup(&config.fd, &config.server_dest, "192.168.1.104", 1250);
	}
	return ret;
}

void Network_disconnect(int _fd)
{
    close(fd);
    
    //TODO: remove client fd from link list
}

int Network_number_of_connection(void)
{
	printf("_network_link_data = %d\n",_network_link_data->data->fd);
	return Link_list_get_size(_network_link_data);
}

int Network_window_size_adjust(int _fd,int *window_size)
{
    return setsockopt(_fd, SOL_TCP, TCP_WINDOW_CLAMP, window_size, sizeof(*window_size));
}

