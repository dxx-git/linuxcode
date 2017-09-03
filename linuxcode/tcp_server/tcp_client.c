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

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("Usage:%s [local_ip] [local_port]\n",argv[0]);
		exit(1);
	}
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(atoi(argv[2]));
	int len = sizeof(server);

	if(connect(sock,(struct sockaddr*)&server,len) < 0)
	{
		perror("connect");
		exit(3);
	}
	while(1)
	{
		char buf[1024];
		printf("client enter#");
		fflush(stdout);
		ssize_t s = read(0,buf,sizeof(buf)-1);
		if(s < 0)
		{
			perror("read1");
			exit(4);
		}
		else if(s > 0)
		{
			buf[s-1] = '\0';
		}
		write(sock,buf,strlen(buf));
		ssize_t ss = read(sock,buf,sizeof(buf)-1);
		if(ss < 0)
		{
			perror("read2");
		}
		printf("server echo:%s\n",buf);
	}
	close(sock);
	return 0;
}
