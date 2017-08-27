/**************************************
*文件说明:main.c
*作者:段晓雪
*创建时间:Sat 05 Aug 2017 06:42:26 PM PDT
*开发环境:Centos Linux/g++ v6.5.0
***************************************/
#include"httpd.h"

void usage(char* arg)
{
	printf("\nUsage:\n\t%s [local_ip] [local_port]\n\n",arg);
}

int startup(const char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("listen");
		exit(2);
	}
	
	//复用端口号
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}

	if(listen(sock,10) < 0)
	{
		perror("listen");
		exit(4);
	}
	return sock;
}

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));
	printf("listen_sock:%d\n",listen_sock);
	int epfd = epoll_create(255);
	if(epfd < 0)
	{
		perror("epoll_create");
		exit(5);
	}

	struct epoll_event ev;
	ev.data.fd = listen_sock;
	ev.events =  EPOLLIN;

    if(epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev) < 0)
	{
		perror("epoll_ctl");
		exit(6);
	}
	struct epoll_event events[512];
	int timeout = -1;
	int max = 512;
	while(1)
	{
		int nums = 0;
		switch(nums = epoll_wait(epfd,events,max,timeout))
		{
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				printf("timeout\n");
				break;
			default:
				for(int i = 0; i < nums; i++)
				{
					int fd = events[i].data.fd;
					if(fd == listen_sock && events[i].events & EPOLLIN)
					{
						struct sockaddr_in client;
						int len = sizeof(client);
						int new_sock = accept(fd,(struct sockaddr*)&client,&len);
						if(new_sock < 0)
						{
							perror("accept");
							break;
						}
						ev.data.fd = new_sock;
						ev.events = EPOLLIN;
						if(epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev) < 0)
						{
							perror("epoll_ctl");
							close(new_sock);
							break;
						}
					}	
					else if(fd != listen_sock && events[i].events & EPOLLIN)
					{
						/*
						char buff[1024];
						ssize_t s = recv(fd,buff,sizeof(buff)-1,0);
						if(s > 0)
						{
							buff[s] = '\0';
							printf("client say:%s",buff);
							fflush(stdout);
						}*/
						handle_http(fd);
						close(fd);
						epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&events[i]);
					}
					else
					{}
				}//for
		}//switch
	}//while
	return 0;
}
