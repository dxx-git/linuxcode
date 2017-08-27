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
	printf("sock:%d\n",sock);

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(argv[1]);
	local.sin_port = htons(atoi(argv[2]));
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}

	//read--->write
	while(1)
	{
	    struct sockaddr_in client;
		int len = sizeof(client);
		char buf[1024];
		ssize_t s = recvfrom(sock,buf,sizeof(buf)-1,0,\
				(struct sockaddr*)&client,&len); 	
		if(s > 0)
		{
			buf[s] = '\0';
			printf("from client %d:%s\n",htons(client.sin_port),buf);
			sendto(sock,buf,strlen(buf),0,\
					(struct sockaddr*)&client,len);
		}
		else if(s < 0)
		{
			perror("recvfrom");
			exit(6);
		}
	}
	close(sock);
	return 0;
}

