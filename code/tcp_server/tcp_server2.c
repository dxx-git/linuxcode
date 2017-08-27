/**************************************
*文件说明:tcp_server1.c
*作者:段晓雪
*创建时间:Wed 26 Jul 2017 07:38:07 PM PDT
*开发环境:Kali Linux/g++ v6.3.0
****************************************/
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

int start_up(char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);//create socket
	if(sock < 0)
	{
		perror("sock");
		exit(2);
	}

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

	int listen_sock = start_up(argv[1],atoi(argv[2]));
	printf("sock:%d\n",listen_sock);

	while(1)
	{
		struct sockaddr_in client;
		int len = sizeof(client);
		int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
		if(new_sock < 0)
		{
			perror("accept");
			exit(5);
		}
		printf("client:%s %d connect success\n",\
				inet_ntoa(client.sin_addr),ntohs(client.sin_port));

		pid_t pid = fork();
		if(pid < 0)
		{
			perror("fork");
			exit(7);
		}
		else if(0 == pid)//child
		{
			if(fork() == 0)
			{
				//read--->write
				while(1)
				{
					char buf[1024];
					ssize_t s = read(new_sock,buf,sizeof(buf)-1);
					if(s > 0)
					{
						buf[s] = 0;
						printf("from client %s %d:%s\n",\
								inet_ntoa(client.sin_addr),\
								ntohs(client.sin_port),buf);
						write(new_sock,buf,strlen(buf));//aim to echo
					}
					else if(s == 0)
					{
						printf("client %s %d is quit!\n",\
								inet_ntoa(client.sin_addr),\
								ntohs(client.sin_port));
						close(new_sock);
						break;
					}
					else
					{
						perror("read");
						exit(6);
					}
				}
			}
			else//child exit
			{
				close(new_sock);
				exit(1);
			}
		}
		else//father
		{
			close(new_sock);
			waitpid(pid,NULL,0);
		}
	}
	close(listen_sock);
	return 0;
}
