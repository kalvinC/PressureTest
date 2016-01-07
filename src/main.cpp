#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct Event_PL{
    int epoll_fd;
};

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

static int epoll_fd;
static struct epoll_event * events;
int main(int argc, char * argv[])
{
	epoll_fd = epoll_create(10240);
	if(epoll_fd == -1)
	{
		exit(1);
	}

    for(int i = 0; i < 1000; i++)
	{
		int fd = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(8000);
		server_addr.sin_addr.s_addr = inet_addr("192.168.0.118");
		
		SetNonBlocking(fd);
	
		struct epoll_event event;
		event.data.fd = fd;
		event.events = EPOLLIN | EPOLLOUT;
		
		printf("%d\n", fd);	
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, events);
 	
		if(connect(fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
		{
			if(errno == EINPROGRESS)
			{
				printf("waitting to connect\n");
				continue;
			}
			printf("connect error\n");
			exit(1);
		}

	}

        events = (struct epoll_event *)calloc(sizeof(struct epoll_event), 1000);

	while(1)
	{
		int n = epoll_wait(epoll_fd, events, 1000, -1);
	
		for(int j = 0; j < n; j++)
		{
			if(events[j].events & EPOLLIN)
			{
				printf("can read\n");
				char buffer[2048] ={0};
			        read(events[j].data.fd, buffer, 2048);
				printf("read data:%s", buffer);
				close(events[j].data.fd);
			}
			else if(events[j].events & EPOLLOUT)
			{
				printf("can write\n");
				char buffer[2048] = "hello world";
				write(events[j].data.fd, buffer, 2014);
				events[j].events = EPOLLIN;
				epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[j].data.fd, &events[j]);
				continue;
			}
			close(events[j].data.fd);
		}
	}
	
	return 0;
}	
