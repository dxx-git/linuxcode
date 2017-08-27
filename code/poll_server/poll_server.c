/**************************************
*文件说明:poll.c
*作者:段晓雪
*创建时间:Thu 27 Jul 2017 07:23:58 PM PDT
*开发环境:Kali Linux/g++ v6.3.0
****************************************/

#include<stdio.h>
#include<sys/poll.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>

int startup(const char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}

	//setsockopt端口号的复用
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(ip);
	local.sin_port = htons(port);
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
		printf("Usage:%s [local_ip] [local_port]\n",argv[0]);
		exit(1);
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));
	printf("sock:%d\n",listen_sock);

	struct pollfd pd[1024];
	pd[0].fd = listen_sock;
	pd[0].events = POLLIN;
	pd[0].revents = 0;

	int i = 1;
	int nums = 1024;
	int timeout = 1000;
	for(; i < nums; i++)//给全局数组初始化
	{
		pd[i].fd = -1;
	}
	pd[0].fd = listen_sock;//将监听套接字存储在0号下标的位置

	while(1)
	{
		switch(poll(pd,nums,timeout))
		{
			case 0:
				//printf("timeout...\n");
				break;
			case -1:
				perror("poll");
				break;
			default:
				{   
					for(i = 0; i < nums; i++)
					{
						if(i==0 && pd[i].revents & POLLIN)//监听套接字获得新连接
						{
							struct sockaddr_in client;
							int len = sizeof(client);
							int new_sock = accept(listen_sock,\
									(struct sockaddr*)&client,&len);
							if(new_sock < 0)
							{
								perror("accept");
								exit(5);
							}
							printf("client %s %d connect success!\n",\
									inet_ntoa(client.sin_addr),\
									ntohs(client.sin_port));
							//accept成功之后，将new_sock添加到结构体数组中
							int j = 1;
							for(; j < nums; j++)
							{
								if(pd[j].fd == -1)
								{
									pd[j].fd = new_sock;
									pd[j].events = POLLIN;
									break;
								}
							}
							//文件描述符数组中没有空位置可用，\
							//即服务器监听的pd达到最大限度了
							if(j == nums)
								break;	
						}
						else if(i != 0)//普通事件就绪
						{
							//poll负责I/O事件的等待，程序到此处一定有事件就绪
							if(pd[i].revents & POLLOUT)//写事件就绪
					   		{
					   			char buf[1024];
					   			printf("please enter:");
					   			fflush(stdout);
					   			read(0,buf,sizeof(buf)-1);
					   			write(pd[i].fd,buf,strlen(buf));
								//client echo
								ssize_t ss = read(pd[i].fd,buf,strlen(buf));
								if(ss > 0)
								{
									printf("client echo:%s\n",buf);
								}
					   			pd[i].events = POLLIN;
					   		}
							else if(pd[i].revents & POLLIN)//读事件就绪
							{
								char buf[1024];
								ssize_t s = read(pd[i].fd,buf,sizeof(buf)-1);
								if(s > 0)
								{
									buf[s] = '\0';
									printf("from client:%s\n\n",buf);
									write(pd[i].fd,buf,strlen(buf));//echo
									pd[i].events = POLLOUT;
								}
								else if(s == 0)
								{
									printf("client is quit!!!\n");
									close(pd[i].fd);
									pd[i].fd = -1;
								}
								else
								{
									perror("read");
									close(pd[i].fd);
									pd[i].fd = -1;
									exit(6);
								}
							}	
						}
						else//没有连接就绪
						{
							continue;
						}
					}//for
				}//default
				break;
		}//switch
	}//while(1)
	close(listen_sock);
	return 0;
}
