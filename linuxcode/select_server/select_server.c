/**************************************
*文件说明:selet.c
*作者:段晓雪
*创建时间:Thu 27 Jul 2017 07:23:58 PM PDT
*开发环境:Kali Linux/g++ v6.3.0
****************************************/

#include<stdio.h>
#include<sys/select.h>
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

int fds[sizeof(fd_set)*8];//定义全局的数组来存储文件描述符

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("Usage:%s [local_ip] [local_port]\n",argv[0]);
		exit(1);
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));
	printf("sock:%d\n",listen_sock);

	int i = 0;
	int nums = sizeof(fds)/sizeof(fds[0]);//全局数组的长度
	for(; i < nums; i++)//给全局数组初始化
	{
		fds[i] = -1;
	}
	fds[0] = listen_sock;//将监听套接字存储在0号下标的位置

	while(1)
	{
		int max = -1;//最大的文件描述符
		fd_set rfd;//读文件描述符集
		FD_ZERO(&rfd);//清除rfd的全部位,及清空里面的所有文件描述符 
		struct timeval timeout = {5,0};//设置超时时间，每2秒记录一次超时时间
		for(i = 0; i < nums; i++)
		{
			if(fds[i] >= 0)//置位,说明此时有连接到来
				FD_SET(fds[i],&rfd);//设置rfd中的第fds[i]位
			if(max < fds[i])//更新最大的文件描述符值
				max = fds[i];
		}	

		switch(select(max+1,&rfd,NULL,NULL,&timeout))
		{
			case 0:
				printf("timeout...\n");
				break;
			case -1:
				perror("select");
				break;
			default:
				{   
					for(i = 0; i < nums; i++)
					{
						if(i==0 && FD_ISSET(fds[i],&rfd))//监听套接字获得新连接
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
							//accept成功之后把new_fd添加到文件描述符数组中
							int j = 1;
							for(; j < nums; j++)
							{
								if(fds[j] == -1)
								{
									fds[j] = new_sock;
									break;
								}
							}
							//文件描述符数组中没有空位置可用，\
							//即服务器监听的fd达到最大限度了
							if(j == nums)
								close(new_sock);
						}
						else if(i != 0 && FD_ISSET(fds[i],&rfd))//普通事件就绪
						{
					    //select专门负责I/O事件的等待，程序到此处一定有事件就绪
							while(1)
							{
								char buf[1024];
								ssize_t s = read(fds[i],buf,sizeof(buf)-1);
								if(s > 0)
								{
									buf[s] = '\0';
									printf("from client:%s\n",buf);
									write(fds[i],buf,strlen(buf));//echo
									break;
								}
								else if(s == 0)
								{
									printf("client is quit!!!\n");
									close(fds[i]);
									fds[i] = -1;
									break;
								}
								else
								{
									perror("read");
									close(fds[i]);
									fds[i] = -1;
									exit(6);
								}
							}//while(1)read--->write
						}
						else//没有连接就绪
						{
							continue;
						}
					}
				}//for
				break;
		}//switch
	}//while(1)
	close(listen_sock);
	return 0;
}
