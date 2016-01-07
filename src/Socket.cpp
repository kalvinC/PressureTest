#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int Socket::NewSocket(Protocol_Type protocol_type)
{
	if(protocol_type == 1)
            return  socket(AF_INET, SOCK_STREAM, 0);
	else
	    return socket(AF_INET, SOCK_DGRAM, 0);
}

void Socket::CloseSocket(int fd)
{
	if(fd >=0)	
		close(fd);
}

int Socket::SetNonBlocking(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if(flag < 0)
	{
		return -1;
	}
	if(fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		return -1;
	}
	return 0;
}

int Socket::Connect(int fd, char * ip, unsigned short port)
{
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	return connect(fd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr));

}

int Socket::Send(int fd, char * buffer, size_t buffer_size)
{
	return send(fd, buffer, buffer_size, 0);
}

int Socket::Recv(int fd, char * buffer, size_t buffer_size)
{
	return recv(fd, buffer, buffer_size, 0);
}

