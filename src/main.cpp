#include "Socket.h"

#include <stdio.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int epoll_fd;
static epoll_event * ep_events;
static volatile int send_count;
static volatile int recv_count;


void NewConn(int fd)
{	
	int result;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&result, sizeof(int)) < 0)
	{
		fprintf(stderr, "setscokopt socket reuse error\n");
		return ;
	}
	Socket::SetNonBlock(fd);

	struct epoll_event epevent;
	epevent.data.fd = fd;
	epevent.events = EPOLLIN | EPOLLOUT;
	   
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &epevent);

	if(Socket::Connect(fd,"117.74.136.118", 8802) < 0)
	{
		fprintf(stderr, "connect err:%s\n", strerror(errno));
		exit(1);
	}	
}

int main(int argc, char ** argv)
{
	epoll_fd = epoll_create(10240);
	
	if(epoll_fd < 0)
	{
		fprintf(stderr, "create epoll queue error\n");
		exit(1);
	}


        ep_events = (struct epoll_event *)calloc(sizeof(struct epoll_event), 1000);

	for(int i=0; i < 10000; i++)
	{
		int fd = Socket::NewSocket(Protocol_TCP);
  		
		if(fd < 0)
		{
			exit(1);
		}

       		 NewConn(fd);		
	}

	char write_buf[2048] = {0};
    	char read_buf[2048] = {0};

        struct timeval tmval;
	gettimeofday(&tmval, NULL);

	struct timeval nwval;
	while(1)
	{
		gettimeofday(&nwval, NULL);
	
		if(nwval.tv_sec - tmval.tv_sec > 300)
		{
			break;
		}
		
		int n = epoll_wait(epoll_fd, ep_events, 1000, 0);
		
		for(int j = 0; j < n; j++)
		{
			if(ep_events[j].events & EPOLLIN)
			{
				memset(read_buf, 0, 2048);
				fprintf(stderr, "Read\n");
			
				int result;
				socklen_t socklen = sizeof(result);
				if(getsockopt(ep_events[j].data.fd, SOL_SOCKET, SO_ERROR, &result, &socklen) == -1)	
				{
					fprintf(stderr, "getsockopt errori\n");	
					continue;
				}
				else
				{
					if(result != 0)
					{
						fprintf(stderr, "connect error\n");
						close(ep_events[j].data.fd);
						continue;
					}
				}

             			int ret = Socket::Recv(ep_events[j].data.fd, read_buf, 2048);
			        recv_count++;
	
				fprintf(stderr, "read content:%s\n", read_buf);
				
				close(ep_events[j].data.fd);
				
				int sockfd = Socket::NewSocket(Protocol_TCP);
				NewConn(sockfd);
	
			}
			else if(ep_events[j].events & EPOLLOUT)
			{
				memset(write_buf, 0, 1024);
				fprintf(stderr, "Write\n");
				
				struct epoll_event ep_event;
				ep_event.data.fd = ep_events[j].data.fd;
				ep_event.events = EPOLLIN;	
					
				epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ep_event.data.fd, &ep_event);
				
                                Socket::Send(ep_events[j].data.fd, "hello", 6);
				send_count++;
			}
			else
			{
				fprintf(stderr, "error\n");
				close(ep_events[j].data.fd);
			}
		}
	}
	
	close(epoll_fd);
	printf("send count in one minute:%d", send_count/5);
	printf("recv count in one minute:%d", recv_count/5);
	return 0;
}
