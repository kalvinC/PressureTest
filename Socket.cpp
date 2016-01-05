#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/innet.h>
#include <netinet/in.h>
#include <errno.h>

Socket::Socket(Protocol_Type protocol_type)
{
	if(protocol_type == 1)
            fd = socket(AF_INET, SOCK_STREAM, 0);
	else
	    fd = socket(AF_INET, SOCK_DGRAM, 0);
}

Socket::~Socket()
{
	close(fd);
}

int Socket::Connect(char * ip, unsigned short port)
{
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	return connect(fd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr));

}

int Socket::Send(char * buffer, size_t buffer_size)
{
	return send(fd, buffer, buffer_size, 0);
}

int Socket::Recv(char * buffer, size_t bufffer_size)
{
	return recv(fd, buffer, buffer_size, 0);
}

std::string error_msg() const
{
	return strerror(errno);
}
