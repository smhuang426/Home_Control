#ifndef __NETWORK_H__
#define __NETWORK_H__
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CONNECTION		10

typedef enum _NETWORK_STATUS{
	NETWORK_FAIL		=-1,
	NETWORK_SUCCESS 	= 0,
	NETWORK_DATA_INVALIDATE = 1,
	NETWORK_DATA_VALIDATE 	= 2
} NETWORK_STATUS;

typedef struct _NETWORK_CONFIG{
	int fd;
	struct sockaddr client_addr;
	struct sockaddr_in server_dest;
} NETWORK_CONFIG;

/*public function*/
int Network_server_setup(int* _sock_fd, int port);
int Network_client_setup(int* _sock_fd, struct sockaddr_in* _dest, char* ip_addr, int port);
ssize_t Network_recv(NETWORK_CONFIG config, void *buf, size_t size, int flags);
int Network_number_of_connection(void);
void Network_disconnect(int _fd);
int Network_window_size_adjust(int _fd,int *window_size);

#endif

