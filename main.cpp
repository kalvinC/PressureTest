#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>

typedef struct Event_PL{
    int epoll_fd;
}

int SetNonBlocking(int fd)
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

int add_conn()
{

	return 0;
}

int del_conn()
{
	return 0;
}

int main(int argc, char * argv[])
{
	return 0;
}	
