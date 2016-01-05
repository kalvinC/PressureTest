#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <string>
typedef enum Protocol_Type{
	Protocol_TCP =1,
	Protocol_UDP
}Protocol_Type;

class Socket{
public:
	Socket(Protocol_Type protocol_type);
	~Socket();
private:
	int SetNonBlocking();
	
	int Connect(char * ip, unsigned short port);
	int Send(char * buffer, size_t buffer_size);
	int Recv(char * buffer, size_t buffer_size);

        std::string error_msg() const;

private:
	int fd;
};
#endif
