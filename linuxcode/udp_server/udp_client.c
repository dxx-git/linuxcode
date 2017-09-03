/**************************************
*文件说明:udp_server.c
*作者:段晓雪
*创建时间:Thu 27 Jul 2017 12:43:02 AM PDT
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

	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(argv[1]);
	local.sin_port = htons(atoi(argv[2]));

	//write--->read
	while(1)
	{
		struct sockaddr server;
		int len = sizeof(server);
		char buf[1024];
		printf("client enter:");
		fflush(stdout);
		ssize_t ss = read(0,buf,sizeof(buf)-1);
		if(ss > 0)
		{
			buf[ss-1] = '\0';
			sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&local,sizeof(local));
			ssize_t s = recvfrom(sock,buf,sizeof(buf)-1,0,\
				(struct sockaddr*)&server,&len); 	
			if(s > 0)
			{
				buf[s] = '\0';
				printf("server echo#%s\n",buf);
			}
			else if(s < 0)
			{
				perror("sendto");
				exit(4);
			}
		}
	    else if(ss < 0)
		{
			perror("read");
			exit(3);
		}
	}
	close(sock);
	return 0;
}

