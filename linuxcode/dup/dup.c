/**************************************
*文件说明:dup.c
*作者:段晓雪
*创建时间:Thu 27 Jul 2017 02:21:26 AM PDT
*开发环境:Kali Linux/g++ v6.3.0
****************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	int fd = open("./log",O_CREAT | O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}
	close(1);

//	int new_fd = dup(fd);
	int new_fd = dup2(fd,1);
	if(new_fd < 0)
	{
		perror("dup");
		exit(2);
	}
	close(fd);

	char buf[1024] = {0};
	while(1)
	{
		fgets(buf,sizeof(buf),stdin);	
		if(strncmp("quit",buf,4) == 0)
			break;
		printf("%s",buf);
		fflush(stdout);
	}
	close(new_fd);
	return 0;
}
