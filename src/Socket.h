#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <string>
typedef enum Protocol_Type{
	Protocol_TCP =1,
	Protocol_UDP
}Protocol_Type;

class Socket{
public:
        static int NewSocket(Protocol_Type protocol_type);
	static void CloseSocket(int fd);
	static int SetNonBlock(int fd);
	static int Connect(int fd, char * ip, unsigned short port);
	static int Send(int fd, char * buffer, size_t buffer_size);
	static int Recv(int fd, char * buffer, size_t buffer_size);
};
#endif
